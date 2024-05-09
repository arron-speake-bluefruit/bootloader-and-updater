#ifndef DRIVERS_BOOT_REPORT_H
#define DRIVERS_BOOT_REPORT_H

typedef enum boot_report {
    // indicates that the bootloader booted to the application without perfoming an update
    boot_report_typical,

    // the bootloader successfully performed an update, then booted to the application
    boot_report_updated,

    // the bootloader attempted an update
    boot_report_failed,
} boot_report_t;

#endif // DRIVERS_BOOT_REPORT_H
