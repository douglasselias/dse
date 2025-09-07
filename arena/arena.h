#ifndef DSE_ARENA_H
#define DSE_ARENA_H

#include <stdbool.h>
#include <stdint.h>

typedef  uint8_t  s8;
typedef  int64_t s64;
typedef  uint8_t  u8;
typedef uint64_t u64;

#define null NULL

#ifndef DSE_ARENA_ALLOC_COMMIT
  #include <stdlib.h>
  #define DSE_ARENA_ALLOC_COMMIT(size)  calloc(1, size)
#endif

#ifndef DSE_ARENA_ALLOC_RESERVE
  #include <stdlib.h>
  #define DSE_ARENA_ALLOC_RESERVE(size) calloc(1, size)
#endif

#ifndef DSE_ARENA_COMMIT_MEMORY
  #include <stdlib.h>
  #define DSE_ARENA_COMMIT_MEMORY(memory, size)
#endif

#ifndef DSE_ARENA_FREE_MEMORY
  #include <stdlib.h>
  #define DSE_ARENA_FREE_MEMORY(memory) free(memory)
#endif

#ifndef DSE_ARENA_MEMSET
  #include <string.h>
  #define DSE_ARENA_MEMSET(memory, value, size) memset(memory, value, size)
#endif

typedef struct DSE_Arena DSE_Arena;
struct DSE_Arena
{
  DSE_Arena *previous;
  DSE_Arena *next;
  s64 *freelist;
  s64 freelist_index;
  s64 used;     // In bytes
  s64 capacity; // In bytes
  u8  *data;

  // TODO:
  // u64 stride;     ?
  // s64 bytes_used; ?
};

DSE_Arena* dse_arena_create (u64 capacity);
void       dse_arena_destroy(DSE_Arena *arena);

void* dse_arena_push(DSE_Arena **arena, u64 size);
void  dse_arena_pop (DSE_Arena **arena, u64 size);

// void dse_pop_from_index(DSE_Arena *arena, u64 index);

// void dse_arena_pop_byte_from_index(DSE_Arena *arena, u64 index);
// void dse_arena_pop_bytes_from_index(DSE_Arena *arena, u64 index, u64 size);
// void dse_arena_pop_from_index_with_memory_move(DSE_Arena *arena, u64 index, u64 size);

#ifdef DSE_ARENA_IMPLEMENTATION

DSE_Arena* dse_arena_create(u64 capacity)
{
  // DSE_Arena *arena = (DSE_Arena*)dse_alloc(sizeof(DSE_Arena), COMMIT_MEMORY);
  DSE_Arena *arena = (DSE_Arena*)DSE_ARENA_ALLOC_COMMIT(sizeof(DSE_Arena));

  arena->capacity = capacity;
  // arena->data     = (u8*) dse_alloc(capacity, RESERVE_MEMORY);
  arena->data     = (u8*) DSE_ARENA_ALLOC_RESERVE(capacity);
  arena->freelist = (s64*)DSE_ARENA_ALLOC_COMMIT(capacity * sizeof(s64));

  // dse_mem_set(arena->freelist, (s8)-1, capacity);
  DSE_ARENA_MEMSET(arena->freelist, (s8)-1, capacity);

  return arena;
}

void dse_arena_destroy(DSE_Arena *arena)
{
  DSE_Arena *current_arena = arena;

  for(; current_arena->previous != null;)
  {
    DSE_ARENA_FREE_MEMORY(current_arena->data);
    // __internal_is_freed(current_arena->data);

    DSE_ARENA_FREE_MEMORY(current_arena->freelist);
    // __internal_is_freed(current_arena->freelist);

    DSE_Arena *temp = current_arena;
    current_arena = current_arena->previous;

    DSE_ARENA_FREE_MEMORY(temp);
    // __internal_is_freed(temp);
  }

  DSE_ARENA_FREE_MEMORY(current_arena->data);
  // __internal_is_freed(current_arena->data);

  DSE_ARENA_FREE_MEMORY(current_arena);
  // __internal_is_freed(current_arena);
}

void* dse_arena_push(DSE_Arena **arena, u64 size)
{
  s64 new_capacity = (*arena)->used + (s64)size;

  if(new_capacity <= (*arena)->capacity)
  {
    // dse_commit_memory((*arena)->data, size);
    DSE_ARENA_COMMIT_MEM((*arena)->data, size);
    void *block = (*arena)->data + (*arena)->used;
    (*arena)->used += size;
    return block;
  }
  else
  {
    s64 freelist_index = (*arena)->freelist_index - 1;
    if(freelist_index != -1)
    {
      s64 index = (*arena)->freelist[freelist_index];
      (*arena)->freelist_index--;
      return (*arena)->data + index;
    }

    s64 remaining = new_capacity - (*arena)->capacity;
    DSE_Arena *new_arena = dse_create_arena(remaining * 2);
    (*arena)->next = new_arena;
    new_arena->previous = *arena;
    *arena = new_arena;
    return dse_arena_push(arena, size);
  }
}

void dse_arena_pop(DSE_Arena **arena, u64 size)
{
  s64 new_capacity = (*arena)->used - (s64)size;

  if(new_capacity >= 0)
  {
    (*arena)->used -= size;
  }
  else
  {
    if((*arena)->previous != null)
    {
      s64 remainder = size - (*arena)->used;
      *arena = (*arena)->previous;
      dse_arena_pop(arena, remainder);
    }
    else
    {
      (*arena)->used = 0;
    }
  }
}

// void dse_pop_from_index(DSE_Arena *arena, u64 index)
// {
//   arena->freelist[arena->freelist_index++] = index;
// }

#endif // DSE_ARENA_IMPLEMENTATION

#ifdef DSE_ARENA_STRIP_PREFIX
  #define Arena          DSE_Arena
  #define arena_create   dse_arena_create
  #define arena_destroy  dse_arena_destroy
  #define arena_push     dse_arena_push
  #define arena_pop      dse_arena_pop
  // #define pop_from_index dse_pop_from_index
#endif // DSE_ARENA_STRIP_PREFIX

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