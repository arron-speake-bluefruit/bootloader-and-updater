# Event loop

Set up a minimal busy-waiting main event loop, with code support for adding new event types.

# USART Command line

Set up a USART-driven command line interface.

# Update-related commands

Add commands for:
- starting and update
- sending firmware images/image portions
- triggering update

# Application update controller

Module for receiving 'update packet' events over the event loop.

# Bootloader flash driver

Bootloader-side flash driver. Kept separate and simpler from the application flash driver.

# Checksumm driver

If there's hardware support, configure using that for checksumming memory regions.
Otherwise, use software checksum.

# 'update ready' flag

Application settable, bootloader resettable flag for performing an update on reset.

# Updating bootloader

Modify the bootloader to:
- check if a firmware update is planned.
- if so:
  - enable flash & checksum drivers
  - checksum validation for update image
  - copy update image onto application image
  - checksum validation for application image
- boot into application

# Bootloader report

Somehow, report the action taken by the bootloader to the application. One of:
- typical - normal boot process
- updated - successfully updated the application image
- bad checksum - acknowledged update request, refused because of bad image checksum

events:
- driver
  - usart_rx (byte)
  - usart_rx_failed (error data)
- component
  - command_parsed (command data)
  - command_parse_failed (error data)
  - command_dispatch_failed (error data)

components:
- command parser, parses user inputs into commands via USART-RX
  - receives usart_rx events
  - receives usart_rx_failed events
  - produces command_parsed events
  - produces command_parse_failed events
- command dispatcher, converts parsed inputs into MORE EVENTS
  - receives command_parsed events
  - produces events for commands (start update, cancel update, update packet, )
    - update_collect_start
    - update_collect_packet
    - update_collect_cancel
    - apply_update
    - reboot
    - version
  - produces command_dispatch_failed events
- reporter, reports errors and stuff to the user via USART-TX
  - produces command_parse_failed events
  - produces command_dispatch_failed events
- updater
  - receives update_collect_start events (expected payload size)
  - receives update_collect_packet events (packet Id, payload)
  - receives update_collect_cancel events
  - TODO: request missing packets?
  - produces update_cancelled events
  - produces update_ready events
