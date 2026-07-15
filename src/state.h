#ifndef STATE_H
#define STATE_H

#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

enum FuelAverageUnit { FUEL_AVG_UNIT_KMPL, FUEL_AVG_UNIT_LP100KM };

struct RpmCtx {
  uint16_t target;
  float display;
};

struct EctCtx {
  int8_t target;
  int8_t display;
};

struct ThrottleCtx {
  uint8_t target;
  uint8_t display;
};
struct IscCtx {
  uint8_t target;
  uint8_t display;
};
struct ThrotAngle {
  uint8_t target;
  uint8_t display;
};

struct StatusFlags {
  bool rad;
  bool psp;
  bool ac;
  bool el;
  bool ctp;
};

struct ParameterCtx {
  struct RpmCtx *rpm;
  struct EctCtx *ect;
  struct StatusFlags *flags;
  struct IscCtx *isc;
  uint8_t throt_abs;
  int8_t iat;
  uint16_t target_idle;
  uint8_t speed;
  uint32_t odometer_m;
  uint32_t trip_m;
  uint8_t throt_angle;
  struct ThrotAngle *throt_angle_ctx;
  float_t bat_volt;
  float_t inj_pw;
  float_t map;
  float_t baro;
  uint8_t load;
  int8_t ign_adv;
  float_t fuel_consumption_instant; // fuel consumption at current instance
  double fuel_consumption_total;    // fuel consumption total
  double fuel_flow_rate;            // fuel use at current instance
  uint64_t fuel_used_motion_ul;     // fuel use in motion
  uint64_t fuel_used_total_ul;      // fuel use total
};

/*
 * Settings for dashuki app.
 *
 * FWIW, this is thread-safe. It is only accessed and modified from the
 * rendering thread, so a mutex isn't required.
 *
 * Like, menu settings are changed by the user (in games). So it makes sense
 * they're only done in a single thread. Similar story here.
 */
struct AppSettings {
  bool show_diag_params;
  enum FuelAverageUnit fuel_avg_unit;
};

extern pthread_mutex_t g_lock_ctx;
extern struct ParameterCtx g_car_ctx;
extern struct AppSettings g_app_settings;

#endif
