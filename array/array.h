#ifndef DSE_ARRAY_H
#define DSE_ARRAY_H

#include "../base_types.h"

#define DSE_OS_IMPLEMENTATION
#include "../os/os.h"

#define DSE_ARENA_IMPLEMENTATION
#include "../arena/arena.h"

#ifndef DSE_MEM_ALLOC
  #define DSE_MEM_ALLOC(total_bytes) dse_mem_alloc(total_bytes)
#endif

#ifndef DSE_MEM_FREE
  #define DSE_MEM_FREE(memory) dse_free_memory(memory)
#endif

#ifndef DSE_MEM_REALLOC
  #define DSE_MEM_REALLOC(memory, total_bytes) dse_mem_realloc(memory, total_bytes)
#endif

#define DSE_CREATE_CUSTOM_ARRAY_TYPE_FUNCTIONS(prefix, suffix, type) \
                                                           \
typedef struct Array_##suffix Array_##suffix;              \
struct Array_##suffix                                      \
{                                                          \
  u32 size;                                                \
  u32 capacity;                                            \
  type *data;                                              \
};                                                         \
                                                           \
void prefix##destroy_array_##suffix(Array_##suffix *array) \
{                                                          \
  array->capacity = 0;                                     \
  DSE_MEM_FREE(array->data);                               \
  array->data = null;                                      \
}                                                          \
                                                           \
Array_##suffix prefix##create_array_##suffix(u32 capacity) \
{                                                          \
  u32 MAX_CAPACITY = 0x80000000ULL; /* 2GB */              \
                                                           \
  if(capacity < 1)            capacity = 1;                \
  if(capacity > MAX_CAPACITY) capacity = MAX_CAPACITY;     \
                                                           \
  Array_##suffix array = {0};                              \
  array.capacity = capacity;                               \
  array.data     = DSE_MEM_ALLOC(sizeof(type) * capacity); \
  return array;                                                       \
}                                                                     \
                                                                      \
void prefix##array_append_##suffix(Array_##suffix *array, type value) \
{                                                                  \
  if(array->size + 1 > array->capacity)                            \
  {                                                                \
    array->capacity *= 2;                                          \
    array->data = DSE_MEM_REALLOC(array->data, sizeof(type) * array->capacity); \
  }                                                                \
                                                                   \
  array->data[array->size] = value;                                \
  array->size++;                                                   \
}                                                                  \
                                                                              \
void prefix##array_remove_by_index_##suffix(Array_##suffix *array, u32 index) \
{                                                                  \
  if(index >= array->size) return;                                 \
                                                                   \
  array->size--;                                                   \
  for(u32 i = index; i < array->size; i++)                         \
  {                                                                \
    array->data[i] = array->data[i + 1];                           \
  }                                                                \
}                                                                  \

#endif // DSE_ARRAY_H

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - MIT License
Copyright (c) 2025 Douglas S. Elias
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/