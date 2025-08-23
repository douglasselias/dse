#ifndef DSE_ARRAY_H
#define DSE_ARRAY_H

typedef   signed long long dse_s64;
typedef unsigned char      dse_u8;
typedef unsigned long long dse_u64;

#ifndef DSE_MEM_ALLOC
  #include <stdlib.h>
  #define DSE_MEM_ALLOC(total_bytes) malloc(total_bytes)
#endif

#ifndef DSE_MEM_FREE
  #include <stdlib.h>
  #define DSE_MEM_FREE(pointer) free(pointer)
#endif

#ifndef DSE_MEM_REALLOC
  #include <stdlib.h>
  #define DSE_MEM_REALLOC(pointer, total_bytes) realloc(pointer, total_bytes)
#endif

typedef struct
{
  dse_u64 size;
  dse_u64 capacity;
  dse_u8 *data;
} DSE_Array;

DSE_Array dse_create_array(dse_u64 capacity);
void dse_destroy_array(DSE_Array *array);
void dse_reset_array(DSE_Array *array);
void dse_array_append(DSE_Array *array, dse_u8 value);
void dse_array_remove_by_index(DSE_Array *array, dse_u64 index);
void dse_print_array(DSE_Array array);

#ifdef DSE_ARRAY_IMPLEMENTATION

DSE_Array dse_create_array(dse_u64 capacity)
{
  dse_u64 MAX_CAPACITY = 0x80000000ULL; // 2GB

  if(capacity < 1)            capacity = 1;
  if(capacity > MAX_CAPACITY) capacity = MAX_CAPACITY;

  DSE_Array array = {0};
  array.capacity  = capacity;
  array.data      = (dse_u8*)DSE_MEM_ALLOC(sizeof(dse_u8) * capacity);
  return array;
}

void dse_destroy_array(DSE_Array *array)
{
  array->capacity = 0;
  DSE_MEM_FREE(array->data);
}

void dse_reset_array(DSE_Array *array)
{
  array->size = 0;
}

void dse_array_append(DSE_Array *array, dse_u8 value)
{
  if (array->size + 1 <= array->capacity)
  {
    array->data[array->size] = value;
  }
  else
  {
    array->capacity *= 2;
    array->data = (dse_u8*)DSE_MEM_REALLOC(array->data, sizeof(dse_u8) * array->capacity);
    array->data[array->size] = value;
  }

  array->size++;
}

void dse_array_remove_by_index(DSE_Array *array, dse_u64 index)
{
  dse_u64 on_the_right = array->size - index - 1;
  dse_u8 *dest = array->data + index;
  dse_u8 *src  = array->data + index + 1;
  memcpy(dest, src, sizeof(dse_u8) * on_the_right);
  array->size--;
}

#define DSE_CREATE_CUSTOM_ARRAY_TYPE_FUNCTIONS(type, name)      \
void array_append_##name(DSE_Array *array, type value)          \
{                                                               \
  dse_u64 size = sizeof(type);                                  \
  dse_u8 *memory = (dse_u8*)&value;                             \
                                                                \
  for(dse_u64 i = 0; i < size; i++)                             \
  {                                                             \
    dse_array_append(array, memory[i]);                         \
  }                                                             \
}                                                               \
                                                                \
type* array_get_by_index_##name(DSE_Array array, dse_u64 index) \
{                                                               \
  dse_u64 element_size = sizeof(type);                          \
  dse_u8 *cursor = array.data;                                  \
  cursor += index * element_size;                               \
  return (type*)cursor;                                         \
}                                                               \
                                                                \
void dse_print_array_##name(DSE_Array array)                    \
{                                                               \
  dse_u64 element_size = sizeof(type);                          \
  dse_u64 array_size = array.size / element_size;               \
  type *cursor = (type*)array.data;                             \
                                                                \
  printf("\n[\n");                                              \
                                                                \
  for(dse_u64 i = 0; i < array_size; i++)                       \
  {                                                             \
    print_##name(cursor);                                       \
    cursor++;                                                   \
  }                                                             \
                                                                \
  printf("]\n");                                                \
}                                                               \

#endif // DSE_ARRAY_IMPLEMENTATION
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