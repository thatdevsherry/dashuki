#define _DEFAULT_SOURCE

#include "comms.h"
#include "constants.h"
#include "interface.h"
#include "raylib.h"
#include "state.h"
#include "suzuki_sdl/vehicles.h"
#include "utils.h"
#include <string.h>
#include <unistd.h>

struct SdlMessage dashboard_sdl_message_create(void) {
  uint8_t map[] = {
      SDL_OBD_ADDRESS_BALENO_33920_65GP_RPM_HIGH,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_RPM_LOW,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_TARGET_IDLE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_VEHICLE_SPEED_SENSOR,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_ENGINE_COOLANT_TEMPERATURE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_INTAKE_AIR_TEMPERATURE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_TPS_ANGLE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_ABSOLUTE_THROTTLE_POSITION,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_INJECTOR_PULSE_WIDTH_HIGH,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_INJECTOR_PULSE_WIDTH_LOW,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_IGNITION_ADVANCE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_MANIFOLD_ABSOLUTE_PRESSURE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_BAROMETRIC_PRESSURE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_ISC_FLOW_DUTY,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_BATTERY_VOLTAGE,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_RADIATOR_FAN,
      SDL_OBD_ADDRESS_BALENO_33920_65GP_STATUS_FLAGS,
  };
  struct SdlObdAddress obd_addrs = {.len = 17};
  memcpy(obd_addrs.map, map, 17);
  struct SdlMessage msg =
      sdl_message_data_request_create(SDL_HEADER_MODULE_ECU, &obd_addrs);
  return msg;
}

void *get_ecu_data(void *arg) {
  struct EcuThreadArgs *args = arg;
  struct ParameterCtx *ctx_all = args->ctx;
  int fd = open_kkl(args->serial_port);

  struct SdlMessage msg = dashboard_sdl_message_create();
  double odo_accumulator = 0.0;
  double fuel_accumulator_l = 0.0;

  double dt;
  double last_time = GetTime();

  double el_polled_first = 0.0;
  int el_changed_times = 0;
  bool el_last_state = false;

  while (true) {
    if (atomic_load(args->should_close)) {
      break;
    }
    usleep(100 * 1000);
    uint8_t buf[255];
    uint8_t echo[255];
    uint8_t recv[255];

    uint8_t send_len = sdl_message_serialize(buf, &msg);
    pretty_print_msg(buf, send_len, "req");

    int written = interface_send(fd, buf, send_len);
    TraceLog(LOG_DEBUG, "wrote %d bytes", written);

    {
      // echo
      interface_recv(fd, echo, send_len);
      pretty_print_msg(echo, send_len, "echo");
    }

    // data
    interface_recv(fd, recv, send_len);
    pretty_print_msg(recv, send_len, "rsp");

    double now_time = GetTime();
    dt = now_time - last_time;
    last_time = now_time;

    // 0 and 1 are hdr and len. start from 2
    uint16_t ecu_rpm = sdl_calculate_engine_speed(recv[2], recv[3]);
    uint16_t ecu_target_idle = sdl_calculate_desired_idle(recv[4]);
    uint8_t ecu_vss = sdl_calculate_vehicle_speed(recv[5]);
    int8_t ecu_ect = sdl_calculate_engine_coolant_temperature(recv[6]);
    int8_t ecu_iat = sdl_calculate_engine_coolant_temperature(recv[7]);
    uint8_t ecu_tps_ang = sdl_calculate_tps_angle(recv[8]);
    uint8_t ecu_abs_throt = sdl_calculate_absolute_throttle_position(recv[9]);
    float_t ecu_inj_pw = sdl_calculate_injector_pulse_width(recv[10], recv[11]);
    int8_t ecu_adv = sdl_calculate_ignition_advance(recv[12]);
    float_t ecu_map = sdl_calculate_manifold_absolute_pressure(recv[13]);
    float_t ecu_baro = sdl_calculate_barometric_pressure(recv[14]);
    uint8_t ecu_isc = sdl_calculate_isc_flow_duty(recv[15]);
    float_t ecu_batt = sdl_calculate_battery_voltage(recv[16]);
    uint8_t ecu_load = sdl_calculate_engine_load(ecu_iat, ecu_map, ecu_baro);
    bool ecu_rad = sdl_calculate_radiator_fan_switch(recv[17]);
    bool ecu_ac = sdl_calculate_ac_switch(recv[18]);
    bool ecu_psp = sdl_calculate_psp_switch(recv[18]);
    bool ecu_el = sdl_calculate_electric_load_switch(recv[18]);
    bool ecu_ctp = sdl_calculate_closed_throttle_position_switch(recv[18]);

    // Reset trip by toggling EL.
    // Use it until a controller setup is built.
    {
      if (ecu_el != el_last_state) {
        if (el_changed_times == 0) {
          el_polled_first = GetTime();
        }
        el_changed_times++;

        if (GetTime() - el_polled_first > 5) {
          el_polled_first = 0.0;
          el_changed_times = 0;
        }

        if (el_changed_times == 6) {
          el_polled_first = 0.0;
          el_changed_times = 0;

          // reset
          pthread_mutex_lock(&g_lock_ctx);
          ctx_all->trip_m = 0;
          ctx_all->fuel_used_total_ul = 0;
          ctx_all->fuel_used_motion_ul = 0;
          pthread_mutex_unlock(&g_lock_ctx);
        }

        el_last_state = ecu_el;
      }
    }

    // fuel calculation
    double fuel_this_poll = 0.0;
    double fuel_flow_rate_litres_per_hour = 0.0;
    double instant_consumption = 0.0;
    if (ecu_rpm > 0) {
      double engine_cycle_time = 60000.0 / ecu_rpm;
      double duty_cycle = (double)ecu_inj_pw / engine_cycle_time;

      double actual_flow_per_injector = INJECTOR_FLOW_RATE * duty_cycle;
      double total_fuel_flow = actual_flow_per_injector * 4.0;
      fuel_flow_rate_litres_per_hour = total_fuel_flow * 60.0 / 1000.0;

      float_t fuel_flow_litres_per_second =
          fuel_flow_rate_litres_per_hour / 3600.0;

      fuel_this_poll = (double)fuel_flow_litres_per_second * dt;

      if (g_app_settings.fuel_avg_unit == FUEL_AVG_UNIT_KMPL) {
        instant_consumption = (ecu_vss / fuel_flow_rate_litres_per_hour);
      } else {
        instant_consumption =
            (fuel_flow_rate_litres_per_hour / ecu_vss) * 100.0;
      }
    }

    struct SdlMessage recv_msg;
    sdl_message_deserialize(recv, &recv_msg);
    if (sdl_message_is_valid(&recv_msg) != 0) {
      TraceLog(LOG_WARNING, "Got malformed ECU message, skipping");
      continue;
    }

    pthread_mutex_lock(&g_lock_ctx);
    ctx_all->fuel_consumption_instant = instant_consumption;
    ctx_all->fuel_flow_rate = fuel_flow_rate_litres_per_hour;

    ctx_all->rpm->target = ecu_rpm;
    ctx_all->target_idle = ecu_target_idle;
    ctx_all->speed = ecu_vss;
    ctx_all->ect->target = ecu_ect;
    ctx_all->iat = ecu_iat;
    ctx_all->throt_angle = ecu_tps_ang;
    ctx_all->throt_angle_ctx->target = ecu_tps_ang;
    ctx_all->throt_abs = ecu_abs_throt;
    ctx_all->inj_pw = ecu_inj_pw;
    ctx_all->ign_adv = ecu_adv;
    ctx_all->map = ecu_map;
    ctx_all->load = ecu_load;
    ctx_all->baro = ecu_baro;
    ctx_all->isc->target = ecu_isc;
    ctx_all->bat_volt = ecu_batt;
    ctx_all->flags->rad = ecu_rad;
    ctx_all->flags->ac = ecu_ac;
    ctx_all->flags->psp = ecu_psp;
    ctx_all->flags->ctp = ecu_ctp;
    ctx_all->flags->el = ecu_el;

    double distance_this_poll = ((double)ctx_all->speed / 3.6) * dt;

    odo_accumulator += distance_this_poll;
    fuel_accumulator_l += fuel_this_poll;

    if (odo_accumulator >= 1.0) {
      uint32_t whole_metres = (uint32_t)odo_accumulator;
      ctx_all->odometer_m += whole_metres;
      ctx_all->trip_m += whole_metres;
      odo_accumulator -= whole_metres;
    }

    uint64_t fuel_ul = (uint64_t)(fuel_accumulator_l * 1000000.0);
    if (fuel_ul > 0) {
      ctx_all->fuel_used_motion_ul += fuel_ul;
      ctx_all->fuel_used_total_ul += fuel_ul;
      fuel_accumulator_l -= (double)fuel_ul / 1000000.0;
    }

    if (ctx_all->fuel_used_motion_ul > 0) {
      if (g_app_settings.fuel_avg_unit == FUEL_AVG_UNIT_KMPL) {
        ctx_all->fuel_consumption_total =
            ((double)ctx_all->trip_m / 1000.0) /
            ((double)ctx_all->fuel_used_motion_ul / 1000000.0);
      } else {
        ctx_all->fuel_consumption_total =
            (((double)ctx_all->fuel_used_motion_ul / 1000000.0) /
             ((double)ctx_all->trip_m / 1000.0)) *
            100.0;
      }
    } else {
      ctx_all->fuel_consumption_total = 0.0;
    }

    pthread_mutex_unlock(&g_lock_ctx);
  }
  return 0;
}
