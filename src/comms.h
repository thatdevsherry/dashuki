#ifndef COMMS_H
#define COMMS_H

#include "suzuki_sdl/suzuki_sdl.h"
#include <stdatomic.h>

struct EcuThreadArgs {
  struct ParameterCtx *ctx;
  char *serial_port;
  atomic_bool *should_close;
};

void *get_ecu_data(void *arg);
struct SdlMessage dashboard_sdl_message_create(void);

#endif
