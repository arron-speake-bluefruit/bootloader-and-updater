#include "vector_table.h"
#include <stdint.h>

extern uintptr_t _app_vectors[];

void vector_table_set(vector_table_index_t index, vector_table_function_t function) {
    _app_vectors[index] = (uintptr_t)function;
}
