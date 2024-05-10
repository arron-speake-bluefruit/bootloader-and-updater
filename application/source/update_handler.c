#include "update_handler.h"
#include "flash_copy.h"
#include "buffered_usart.h"

// Symbols defined in the common linker script.
extern uint32_t UPDATE_REGION_SIZE;
extern uint32_t UPDATE_REGION_ORIGIN;

static size_t block_index;
static bool is_update_available;

void update_handler_initialize(void) {
    is_update_available = false;
}

void update_handler_prepare(void) {
    uint32_t start_address = (uint32_t)&UPDATE_REGION_ORIGIN;
    uint32_t byte_count = (uint32_t)&UPDATE_REGION_SIZE;
    uint32_t page_count = byte_count / flash_page_size;

    bool success = flash_erase_pages(start_address, page_count);

    (void)success; // TODO

    block_index = 0;
    is_update_available = false;
}

void update_handler_push_block(const void* block) {
    // TODO: Check `prepare` has been called?

    uint32_t offset = block_index * update_handler_block_size;
    uint32_t address = (uint32_t)&UPDATE_REGION_ORIGIN + offset;
    void* destination = (void*)address;

    size_t size_in_halfwords = update_handler_block_size / 2;

    bool success = flash_copy(block, destination, size_in_halfwords);

    (void)success; // TODO

    block_index += 1;
}

void update_handler_finish(void) {
    // TODO:
    // - verify written block are contiguous
    // - verify an image checksum
    // - verify all expected block have been written

    is_update_available = true;
}

bool update_handler_is_update_available(void) {
    return is_update_available;
}
