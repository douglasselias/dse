#ifndef DSE_ARENA_H
#define DSE_ARENA_H

#include "../base_types.h"
#include "../os/os.h"

#include <windows.h>

Struct(DSE_Arena)
{
  DSE_Arena *previous;
  DSE_Arena *next;
  s64 *freelist;
  s64 freelist_index;
  s64 used;
  s64 capacity;
  u8 *data;
};

DSE_Arena* dse_create_arena (u64 capacity);
void   dse_destroy_arena(DSE_Arena *arena);

void* dse_push_arena(DSE_Arena **arena, u64 size);
void  dse_pop_arena (DSE_Arena **arena, u64 size);

#ifdef DSE_ARENA_IMPLEMENTATION

#define __internal_is_freed(buffer)                                   \
  do                                                                  \
  {                                                                   \
    u64 line = __LINE__;                                          \
    MEMORY_BASIC_INFORMATION memory_info;                             \
    VirtualQuery(buffer, &memory_info, sizeof(memory_info));          \
    if(memory_info.State != MEM_FREE)                                 \
    {                                                                 \
      printf("Memory not freed (%s) at line: %lld\n", #buffer, line); \
    }                                                                 \
  } while(0)                                                          \

DSE_Arena* dse_create_arena(u64 capacity)
{
  DSE_Arena *arena = (DSE_Arena*)VirtualAlloc(null, sizeof(DSE_Arena), MEM_COMMIT, PAGE_READWRITE);

  arena->capacity = capacity;
  arena->data     = (u8*) VirtualAlloc(null, capacity,                   MEM_RESERVE, PAGE_READWRITE);
  arena->freelist = (s64*)VirtualAlloc(null, capacity * sizeof(s64), MEM_COMMIT,  PAGE_READWRITE);

  memset(arena->freelist, -1, capacity);

  return arena;
}

void dse_destroy_arena(DSE_Arena *arena)
{
  DSE_Arena *current_arena = arena;

  for(; current_arena->previous != null;)
  {
    VirtualFree(current_arena->data,     0, MEM_RELEASE);
    __internal_is_freed(current_arena->data);

    VirtualFree(current_arena->freelist, 0, MEM_RELEASE);
    __internal_is_freed(current_arena->freelist);

    DSE_Arena *temp = current_arena;
    current_arena = current_arena->previous;

    VirtualFree(temp, 0, MEM_RELEASE);
    __internal_is_freed(temp);
  }

  VirtualFree(current_arena->data, 0, MEM_RELEASE);
  __internal_is_freed(current_arena->data);

  VirtualFree(current_arena,       0, MEM_RELEASE);
  __internal_is_freed(current_arena);
}

void* dse_push_arena(DSE_Arena **arena, u64 size)
{
  s64 new_capacity = (*arena)->used + (s64)size;

  if(new_capacity <= (*arena)->capacity)
  {
    VirtualAlloc((*arena)->data, size, MEM_COMMIT, PAGE_READWRITE);
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
    return dse_push_arena(arena, size);
  }
}

void dse_pop_arena(DSE_Arena **arena, u64 size)
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
      dse_pop_arena(arena, remainder);
    }
    else
    {
      (*arena)->used = 0;
    }
  }
}

void dse_pop_from_index(DSE_Arena *arena, u64 index)
{
  arena->freelist[arena->freelist_index++] = index;
}

// TODO: Should be outside of implementation.
#ifdef DSE_ARENA_STRIP_PREFIX
  // TODO: Should be typedef or #define?
  #define Arena         DSE_Arena
  #define create_arena  dse_create_arena
  #define destroy_arena dse_destroy_arena
  #define push_arena    dse_push_arena
  #define pop_arena     dse_pop_arena
#endif // DSE_ARENA_STRIP_PREFIX

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