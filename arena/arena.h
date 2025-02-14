#ifndef DSE_ARENA_H
#define DSE_ARENA_H

#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

typedef int64_t  dse_s64;
typedef uint8_t  dse_u8;
typedef uint64_t dse_u64;

typedef struct Arena Arena;
struct Arena {
  Arena* previous;
  Arena* next;
  dse_s64 freelist[100];
  dse_s64 freelist_index;
  dse_s64 used;
  dse_s64 capacity;
  dse_u8* data;
};

Arena* dse_create_arena(dse_u64 capacity);
void dse_destroy_arena(Arena** arena);

void* dse_push_arena(Arena** arena, dse_u64 size);
void dse_pop_arena(Arena* arena, dse_u64 size);

#ifdef DSE_ARENA_IMPLEMENTATION

#define  __internal_is_freed(buffer) \
  do { \
    dse_u64 line = __LINE__; \
    MEMORY_BASIC_INFORMATION memory_info; \
    VirtualQuery(buffer, &memory_info, sizeof(memory_info)); \
    if(memory_info.State != MEM_FREE) { \
      printf("Memory not freed (%s) at line: %lld\n", #buffer, line); \
    } \
  } while(0) \


Arena* dse_create_arena(dse_u64 capacity) {
  Arena* arena = VirtualAlloc(NULL, sizeof(Arena), MEM_COMMIT, PAGE_READWRITE);
  arena->capacity = capacity;
  arena->data = VirtualAlloc(NULL, arena->capacity, MEM_RESERVE, PAGE_READWRITE);
  return arena;
}

void dse_destroy_arena(Arena** arena_ptr) {
  Arena* arena = *arena_ptr;
  Arena* current_arena = arena;
  for(; current_arena->previous != NULL;) {
    VirtualFree(current_arena->data, 0, MEM_RELEASE);
    __internal_is_freed(current_arena->data);

    Arena* temp = current_arena;
    current_arena = current_arena->previous;
    VirtualFree(temp, 0, MEM_RELEASE);
    __internal_is_freed(temp);
  }

  VirtualFree(current_arena->data, 0, MEM_RELEASE);
  __internal_is_freed(current_arena->data);
  VirtualFree(current_arena,       0, MEM_RELEASE);
  __internal_is_freed(current_arena);
}

void* dse_push_arena(Arena** arena, dse_u64 size) {
  dse_s64 new_capacity = (*arena)->used + (dse_s64)size;

  if(new_capacity <= (*arena)->capacity) {
    VirtualAlloc((*arena)->data, size, MEM_COMMIT, PAGE_READWRITE);
    void* block = (*arena)->data + (*arena)->used;
    (*arena)->used += size;
    return block;
    // dse_s64 index = arena->freelist[arena->freelist_index];
    // if(index != 0) {
    //   arena->freelist[arena->freelist_index++];
    // }
  } else {
    dse_s64 remaining = new_capacity - (*arena)->capacity;
    Arena* new_arena = dse_create_arena(remaining * 2);
    (*arena)->next = new_arena;
    new_arena->previous = *arena;
    *arena = new_arena;
    return dse_push_arena(arena, size);
  }
}

void dse_pop_arena(Arena* arena, dse_u64 size) {
  if((arena->used - (dse_s64)size) >= 0) {
    arena->used -= size;
    // arena->freelist[arena->freelist_index++] = arena->data - arena->used;
  } else {
    if(arena->previous != NULL) {
      dse_s64 current_remainder = size - arena->used;
      arena = arena->previous;
      dse_pop_arena(arena, current_remainder);
    } else {
      arena->used = 0;
    }
  }
}

#endif // DSE_ARENA_IMPLEMENTATION
#endif // DSE_ARENA_H

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