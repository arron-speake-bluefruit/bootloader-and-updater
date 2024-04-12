#ifndef VECTOR_TABLE_H
#define VECTOR_TABLE_H

typedef void (*vector_table_function_t)(void);

typedef enum vector_table_index {
    vector_table_index_usart2 = 44,
} vector_table_index_t;

void vector_table_set(vector_table_index_t index, vector_table_function_t function);

#endif // VECTOR_TABLE_H
