#include "update_handler.h"
#include "flash_copy.h"
#include "buffered_usart.h"

// Symbols defined in the common linker script.
extern uint32_t UPDATE_REGION_SIZE;
extern uint32_t UPDATE_REGION_ORIGIN;

static size_t block_index;
static update_handler_status_t status;

void update_handler_prepare(void) {
    uint32_t start_address = (uint32_t)&UPDATE_REGION_ORIGIN;
    uint32_t byte_count = (uint32_t)&UPDATE_REGION_SIZE;
    uint32_t page_count = byte_count / flash_page_size;

    bool success = flash_erase_pages(start_address, page_count);

    block_index = 0;

    if (success) {
        status = update_handler_status_none;
    } else {
        status = update_handler_status_error;
    }
}

void update_handler_push_block(const void* block) {
    // TODO: Check `prepare` has been called?

    uint32_t offset = block_index * update_handler_block_size;
    block_index += 1;

    uint32_t address = (uint32_t)&UPDATE_REGION_ORIGIN + offset;
    void* destination = (void*)address;

    size_t size_in_halfwords = update_handler_block_size / 2;

    bool success = flash_copy(block, destination, size_in_halfwords);

    if (!success) {
        status = update_handler_status_error;
    }
}

void update_handler_finish(void) {
    // TODO:
    // - verify written block are contiguous
    // - verify an image checksum
    // - verify all expected block have been written

    if (status != update_handler_status_error) {
        status = update_handler_status_ready;
    }
}

update_handler_status_t update_handler_get_status(void) {
    return status;
}
