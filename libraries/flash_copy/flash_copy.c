#include "flash_copy.h"
#include "flash.h"

static void wait_until_flash_not_busy(void) {
    while (flash_is_busy()) {}
}

static bool erase_pages_unlocked(uint32_t page_address, uint32_t page_count) {
    flash_set_operation(flash_operation_page_erase);

    for (size_t i = 0; i < page_count; i++) {
        uint32_t page = page_address + (i * flash_page_size);

        flash_set_address(page);
        flash_operation_start();

        wait_until_flash_not_busy();
        uint32_t status = flash_status();
        flash_clear_status();

        bool success = (status & flash_status_bit_end_of_program) != 0;
        if (!success) {
            return false;
        }
    }

    return true;
}

static bool copy_unlocked(const uint16_t* source, uint16_t* destination, size_t count) {
    flash_set_operation(flash_operation_program);

    for (size_t i = 0; i < count; i++) {
        destination[i] = source[i];

        wait_until_flash_not_busy();
        uint32_t status = flash_status();
        flash_clear_status();

        bool success = (status & flash_status_bit_end_of_program) != 0;
        if (!success) {
            return false;
        }
    }

    return true;
}

bool flash_erase_pages(uint32_t page_address, uint32_t page_count) {
    flash_unlock(FLASH_KEY1, FLASH_KEY2);
    wait_until_flash_not_busy();

    bool success = erase_pages_unlocked(page_address, page_count);

    flash_lock();

    return success;
}

bool flash_copy(const uint16_t* source, uint16_t* destination, size_t count) {
    flash_unlock(FLASH_KEY1, FLASH_KEY2);
    wait_until_flash_not_busy();

    bool success = copy_unlocked(source, destination, count);

    flash_lock();

    return success;
}
