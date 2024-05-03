#ifndef DRIVERS_CRITICAL_SECTION_H
#define DRIVERS_CRITICAL_SECTION_H

// Enter a critical section, disabling maskable interrupts until `CRITICAL_SECTION_EXIT`.
#define CRITICAL_SECTION_ENTER() do { __asm__ volatile ("cpsid i"); } while (0)

// Exit a critical section, enabling maskable interrupts.
#define CRITICAL_SECTION_EXIT() do { __asm__ volatile ("cpsie i"); } while (0)

#endif // DRIVERS_CRITICAL_SECTION_H
