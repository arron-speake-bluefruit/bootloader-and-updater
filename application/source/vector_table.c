#include "vector_table.h"
#include <stdint.h>

enum { vector_count = 48 };

__attribute__ ((section(".app_vectors")))
uintptr_t app_vectors[vector_count];

void vector_table_set(vector_table_index_t index, vector_table_function_t function) {
    app_vectors[index] = (uintptr_t)function;
}
