#ifndef INFO_REGION_H
#define INFO_REGION_H

typedef enum info_status {
    info_status_no_update,
    info_status_update_ready,
} info_status_t;

void info_region_set_status(info_status_t status);
info_status_t info_region_get_status(void);

#endif // INFO_REGION_H
