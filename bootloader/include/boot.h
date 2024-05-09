#ifndef BOOTLOADER_BOOT_H
#define BOOTLOADER_BOOT_H

#include "boot_report.h"
#include <stdnoreturn.h>

noreturn void boot_to_application(boot_report_t report);

#endif // BOOTLOADER_BOOT_H
