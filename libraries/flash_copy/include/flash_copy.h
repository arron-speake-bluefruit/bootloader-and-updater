#ifndef FLASH_COPY_H
#define FLASH_COPY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum {
    // Number of bytes in a flash page.
    flash_page_size = 1024,
};

// Erase the `page_count` pages in flash, starting at `page_address`.
//
// If the operation succeeds, all pages in the given range will contain half-words of `0xFFFF`,
// returning `true`. If the operation fails, pages in the range will be in an undefined state,
// returning `false`.
bool flash_erase_pages(uint32_t page_address, uint32_t page_count);

// Copy `count` half-words from `source` into `destination`. The destination range MUST fit into
// an erased flash region.
//
// If the operation succeeds, all half-words in `[source, source+count)` will be equal to
// `[destination, destination+count)`, returning `true`. If the operation fails, the destination
// range will be in an undefined state, returnin `false`.
bool flash_copy(const uint16_t* source, uint16_t* destination, size_t count);

#endif // FLASH_COPY_H
