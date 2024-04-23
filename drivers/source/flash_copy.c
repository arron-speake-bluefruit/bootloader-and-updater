#include "flash_copy.h"
#include "flash.h"

static void wait_until_flash_not_busy(void) {
    while (flash_is_busy()) {}
}

bool flash_erase_pages(uint32_t page_address, uint32_t page_count) {
    flash_unlock(FLASH_KEY1, FLASH_KEY2);
    wait_until_flash_not_busy();

    for (size_t i = 0; i < page_count; i++) {
        uint32_t page = page_address + (i * flash_page_size);

        flash_set_operation(flash_operation_page_erase);
        flash_set_address(page);
        flash_operation_start();

        wait_until_flash_not_busy();
        uint32_t status = flash_status();
        flash_clear_status();

        bool programming_error = (status & flash_status_bit_programming_error) != 0;
        bool write_protect_error = (status & flash_status_bit_write_protect_error) != 0;
        if (programming_error || write_protect_error) {
            flash_lock();
            return false;
        }
    }

    flash_lock();
    return true;
}

void flash_copy(const uint16_t* source, uint16_t* destination, size_t count) {
    flash_unlock(FLASH_KEY1, FLASH_KEY2);
    wait_until_flash_not_busy(); // TODO?

    for (size_t i = 0; i < count; i++) {
        flash_set_operation(flash_operation_program);
        destination[i] = source[i];

        wait_until_flash_not_busy();

        // TODO: Check for failure.
    }

    flash_lock();
}
