#define _DEFAULT_SOURCE

#include "save.h"
#include "gui/dashboard.h"
#include "raylib.h"
#include "state.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void save_to_disk(struct ParameterCtx *ctx) {
  FILE *fp = fopen(DATA_FILE_PATH_TMP, "w");
  if (!fp) {
    perror("fopen");
    return;
  }

  fprintf(fp, "odometer_m=%d\n", ctx->odometer_m);
  fprintf(fp, "trip_m=%d\n", ctx->trip_m);
  fprintf(fp, "fuel_used_motion_ul=%ld\n", ctx->fuel_used_motion_ul);
  fprintf(fp, "fuel_used_total_ul=%ld\n", ctx->fuel_used_total_ul);

  if (fflush(fp) != 0) {
    perror("fflush");
    fclose(fp);
    return;
  }
  if (fsync(fileno(fp)) != 0) {
    perror("fsync");
    fclose(fp);
    return;
  }
  fclose(fp);

  if (rename(DATA_FILE_PATH_TMP, DATA_FILE_PATH) != 0) {
    perror("rename");
  }
}

void save_to_disk_app_state(struct AppSettings *app_settings) {
  FILE *fp = fopen(SETTINGS_FILE_PATH_TMP, "w");
  if (!fp) {
    perror("fopen");
    return;
  }

  fprintf(fp, "show_diag_params=%d\n", app_settings->show_diag_params);
  fprintf(fp, "fuel_avg_unit=%d\n", app_settings->fuel_avg_unit);

  if (fflush(fp) != 0) {
    perror("fflush");
    fclose(fp);
    return;
  }
  if (fsync(fileno(fp)) != 0) {
    perror("fsync");
    fclose(fp);
    return;
  }
  fclose(fp);

  if (rename(SETTINGS_FILE_PATH_TMP, SETTINGS_FILE_PATH) != 0) {
    perror("rename");
  }
}

int load_from_disk(struct ParameterCtx *ctx) {
  FILE *fp = fopen(DATA_FILE_PATH, "r");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  char line[128];
  int fields_found = 0;

  while (fgets(line, sizeof(line), fp)) {
    char key[64];
    uint64_t value;
    if (sscanf(line, "%63[^=]=%ld", key, &value) != 2) {
      fprintf(stderr, "Malformed line skipped: %s", line);
      continue;
    }

    if (strcmp(key, "odometer_m") == 0) {
      ctx->odometer_m = value;
      fields_found++;
    } else if (strcmp(key, "trip_m") == 0) {
      ctx->trip_m = value;
      fields_found++;
    } else if (strcmp(key, "fuel_used_motion_ul") == 0) {
      ctx->fuel_used_motion_ul = value;
      fields_found++;
    } else if (strcmp(key, "fuel_used_total_ul") == 0) {
      ctx->fuel_used_total_ul = value;
      fields_found++;
    }
  }
  fclose(fp);

  if (fields_found < 4) {
    TraceLog(LOG_WARNING, "expeced fields missing from %s", DATA_FILE_PATH);
    return -1;
  }
  return 0;
}

int load_from_disk_app_state(struct AppSettings *app_settings) {
  FILE *fp = fopen(SETTINGS_FILE_PATH, "r");
  if (!fp) {
    perror("fopen");
    return -1;
  }

  char line[128];
  int fields_found = 0;

  while (fgets(line, sizeof(line), fp)) {
    char key[64];
    uint64_t value;
    if (sscanf(line, "%63[^=]=%ld", key, &value) != 2) {
      fprintf(stderr, "Malformed line skipped: %s", line);
      continue;
    }

    if (strcmp(key, "show_diag_params") == 0) {
      app_settings->show_diag_params = value;
      fields_found++;
    } else if (strcmp(key, "fuel_avg_unit") == 0) {
      app_settings->fuel_avg_unit = value;
      fields_found++;
    }
  }
  fclose(fp);

  if (fields_found < 2) {
    TraceLog(LOG_WARNING, "expeced fields missing from %s", SETTINGS_FILE_PATH);
    return -1;
  }
  return 0;
}
