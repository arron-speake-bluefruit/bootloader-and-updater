#ifndef APPLICATION_UPDATE_HANDLER_H
#define APPLICATION_UPDATE_HANDLER_H

#include <stdbool.h>

enum {
    update_handler_block_size = 128,
};

void update_handler_initialize(void);

void update_handler_prepare(void);

void update_handler_push_block(const void* block);

void update_handler_finish(void);

bool update_handler_is_update_available(void);

#endif // APPLICATION_UPDATE_HANDLER_H
