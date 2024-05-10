#ifndef APPLICATION_UPDATE_HANDLER_H
#define APPLICATION_UPDATE_HANDLER_H

enum {
    update_handler_block_size = 128,
};

typedef enum update_handler_status {
    update_handler_status_none,
    update_handler_status_ready,
    update_handler_status_error,
} update_handler_status_t;

void update_handler_prepare(void);

void update_handler_push_block(const void* block) ;

void update_handler_finish(void);

update_handler_status_t update_handler_get_status(void);

#endif // APPLICATION_UPDATE_HANDLER_H
