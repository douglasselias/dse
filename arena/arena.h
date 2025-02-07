#ifndef DSE_ARENA_H
#define DSE_ARENA_H

#include<stdbool.h>
#include<stdint.h>

#include <time.h>
#include<windows.h>

char* timestamp() {
  time_t ltime = time(NULL);
  return asctime(localtime(&ltime));
}

typedef int64_t dse_s64;
typedef uint8_t dse_u8;
typedef uint64_t dse_u64;

typedef struct Arena Arena;
struct Arena {
  Arena* previous;
  Arena* next;
  dse_s64 used;
  dse_s64 capacity;
  dse_u8* data;
};

Arena* dse_create_arena(dse_u64 capacity);
void dse_destroy_arena(Arena* arena);

void* dse_push_arena(Arena* arena, dse_u64 size);
void dse_pop_arena(Arena* arena, dse_u64 size);

void dse_reset_arena(Arena* arena);
void dse_set_position_back(Arena* arena, dse_u64 size);

dse_u64 dse_total_bytes_allocated(Arena* arena);

/// @todo: Do the same for pop
// #define dse_push_arena(arena, size) \
  // _dse_push_arena(arena, size); printf("At %s on %s:%d Used %zd byte(s)\n", timestamp(), __FILE__, __LINE__, arena->used * sizeof(dse_u8)); \

#define dse_push_arena(arena, size) \
  _dse_push_arena(arena, size); \

#ifdef DSE_ARENA_IMPLEMENTATION

Arena* dse_create_arena(dse_u64 capacity) {
  Arena* arena = calloc(sizeof(Arena), 1);
  arena->capacity = capacity * sizeof(dse_u8);
  // arena->data = calloc(sizeof(dse_u8), arena->capacity);

  /// @todo: No idea why this does not work!!!
  // arena->data = VirtualAlloc(NULL, sizeof(dse_u8) * arena->capacity, MEM_RESERVE, PAGE_READWRITE);
  arena->data = VirtualAlloc(NULL, sizeof(dse_u8) * arena->capacity, MEM_RESERVE, PAGE_READWRITE);
  if(arena->data == NULL) puts("Data is null");
  int err = GetLastError();
  // if(err != 0) printf("Error on creating arena: %d", err);

  LPVOID lpMsgBuf;
  FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | 
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    err,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &lpMsgBuf,
    0, NULL);
  // printf("Error message: %s\n", (LPCTSTR)lpMsgBuf);
  ////////////////////////////////

  arena->previous = calloc(sizeof(Arena), 1);
  arena->next = calloc(sizeof(Arena), 1);
  return arena;
}

void dse_destroy_arena(Arena* arena) {
  /// @todo: Update to chaining
  free(arena->data);
  free(arena);
}

void* _dse_push_arena(Arena* arena, dse_u64 size) {
  dse_u64 byte_size = size * sizeof(dse_u8);
  dse_s64 size_used = arena->used + byte_size;
  dse_s64 total_capacity = arena->capacity * sizeof(dse_u8);
  if(size_used > total_capacity) {
    /// @todo: Need to think about default size of arena
    Arena* new_arena = dse_create_arena(size * 2);
    arena->next = new_arena;
    new_arena->previous = arena;
    /// @todo: Somehow update the arena to new_arena
    return _dse_push_arena(arena, size);
  } else {
    /// @todo: This doenst work yet
    VirtualAlloc(arena->data, size * sizeof(dse_u8), MEM_COMMIT, PAGE_READWRITE);
    void* block = arena->data + (arena->used / sizeof(dse_u8));
    arena->used += size * sizeof(dse_u8);
    return block;
  }
}

void dse_pop_arena(Arena* arena, dse_u64 size) {
  dse_u64 byte_size = size * sizeof(dse_u8);
  dse_s64 size_used = arena->used - byte_size;
  if(size_used >= 0) {
    arena->used -= size * sizeof(dse_u8);
  } else {
    if(arena->previous != NULL) {
      dse_s64 current_remainder = byte_size - arena->used;
      arena = arena->previous;
      dse_pop_arena(arena, current_remainder);
    } else {
      arena->used = 0;
    }
  }
}

void dse_reset_arena(Arena* arena) {
  arena->used = 0;
}

void dse_set_position_back(Arena* arena, dse_u64 size) {
  dse_s64 byte_size = size * sizeof(dse_u8);
  dse_s64 total_capacity = arena->capacity * sizeof(dse_u8);
  if(byte_size >= total_capacity) {
    puts("[Arena Error] No memory left");
  }
  arena->used = size * sizeof(dse_u8);
}

dse_u64 dse_total_bytes_allocated(Arena* arena) {
  return arena->used * sizeof(dse_u8);
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