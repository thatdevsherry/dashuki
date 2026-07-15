#ifndef SAVE_H
#define SAVE_H

#include "gui/dashboard.h"
#include "state.h"

#ifndef DATA_FILE_PATH
#error "DATA_FILE_PATH variable needed"
#endif
#ifndef DATA_FILE_PATH_TMP
#error "DATA_FILE_PATH_TMP variable needed"
#endif
#ifndef SETTINGS_FILE_PATH
#error "SETTINGS_FILE_PATH variable needed"
#endif
#ifndef SETTINGS_FILE_PATH_TMP
#error "SETTINGS_FILE_PATH_TMP variable needed"
#endif

void save_to_disk(struct ParameterCtx *ctx);
void save_to_disk_app_state(struct AppSettings *app_settings);
int load_from_disk(struct ParameterCtx *ctx);
int load_from_disk_app_state(struct AppSettings *app_settings);

#endif
