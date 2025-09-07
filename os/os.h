#ifndef DSE_OS_H
#define DSE_OS_H

#include <stdbool.h>
#include <stdint.h>

typedef  uint8_t  s8;
typedef  int64_t s64;
typedef  uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;

#define null NULL

// Threads //
#define INFINITE_TIMEOUT 0xFFFFFFFF

#define dse_thread_handle void*
typedef void dse_thread_proc(void *args);

u32 dse_count_threads();

dse_thread_handle dse_create_thread(dse_thread_proc *thread_proc, void *args);
void dse_wait_for_thread(dse_thread_handle handle, u32 timeout_ms);
void dse_wait_for_all_threads(dse_thread_handle *handles, u64 threads_count, u32 timeout_ms);
// End Threads //

// Memory //

typedef enum DSE_AllocationType DSE_AllocationType;
enum DSE_AllocationType
{
  // This is actually a Window specific value.
  // TODO: Would be better to be something else to be crossplatform?
  RESERVE_MEMORY = 0x00002000,
  COMMIT_MEMORY  = 0x00001000,
};

void* dse_alloc(u64 capacity, DSE_AllocationType allocation_type);
void dse_commit_memory(void *memory, u64 capacity);
void* dse_mem_alloc(u64 capacity);
void* dse_mem_realloc(void *old_memory, u64 new_capacity);
void dse_free_memory(void *memory);
bool dse_has_freed_memory(void *memory);
// End Memory //


// Memory (move to another file) //
void dse_mem_set(void *memory, s8 value, u64 size);
void dse_mem_copy(void *destination, void *source, u64 size);
// End Memory (move to another file) //

// RNG //
u8 dse_generate_random_number();
// End RNG //

// Atomics //
void dse_atomic_increment(s64 *number);
// End Atomics //

// Timer //
u64 dse_get_cpu_timer();
// End Timer //

// File system //
void dse_list_files_from_dir(char *path);
// End File system //

#ifdef DSE_OS_IMPLEMENTATION

#include <windows.h>
#include <bcrypt.h>
// #include <intrin.h> // TODO: Should include this header for __rdtsc?
#pragma comment(lib, "bcrypt.lib")

// Generates a number between 0 and 255
u8 dse_generate_random_number()
{
  u8 buffer[1] = {0};
  
  NTSTATUS status = BCryptGenRandom(null, buffer, sizeof(buffer), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
  
  if(status != 0)
  {
    // printf("[ERROR]: Couldn't generate a random number, status code 0x%08x\n", status);
  }

  return buffer[0];
}

u32 dse_count_threads()
{
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwNumberOfProcessors;
}

dse_thread_handle dse_create_thread(dse_thread_proc *thread_proc, void *args)
{
  LPSECURITY_ATTRIBUTES security_attributes = null;
  SIZE_T stack_size                         = 0;
  u32 creation_flags                        = 0; // TODO: Can be created suspended.
  u32 *thread_id                            = null;

  return CreateThread(security_attributes, stack_size, thread_proc, args, creation_flags, thread_id);
}

void dse_wait_for_thread(dse_thread_handle handle, u32 timeout_ms)
{
  WaitForSingleObject(handle, timeout_ms);
}

void dse_wait_for_all_threads(dse_thread_handle *handles, u64 threads_count, u32 timeout_ms)
{
  bool wait_all  = true;
  WaitForMultipleObjects(threads_count, handles, wait_all, timeout_ms);
}

void dse_atomic_increment(s64 *number)
{
  InterlockedIncrement64(number);
}

u64 dse_get_os_timer_frequency()
{
  LARGE_INTEGER f;
  QueryPerformanceFrequency(&f);
  return f.QuadPart;
}

u64 dse_get_os_timer()
{
  LARGE_INTEGER c;
  QueryPerformanceCounter(&c);
  return c.QuadPart;
}

u64 dse_get_cpu_timer()
{
  return __rdtsc();
}

// TODO: Add option for recursive listing.
u64 depth = 0;
void dse_list_files_from_dir(char *path)
{
  char search_path[MAX_PATH] = {0};
  // sprintf(search_path, "%s\\*", path);

  WIN32_FIND_DATA ffd;
  HANDLE find_file_handle = FindFirstFile(search_path, &ffd);

  if(INVALID_HANDLE_VALUE == find_file_handle) return;

  do
  {
    char *filename = ffd.cFileName;

    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      if(filename[0] == '.') continue;
      if(strcmp(filename, ".") == 0) continue;
      if(strcmp(filename, "..") == 0) continue;
      char dir_buffer[MAX_PATH] = {0};
      // sprintf(dir_buffer, "%s\\%s", path, filename);
      // sprintf(dir_buffer, "%s", filename);
      for(u64 i = 0; i < depth; i++)
      {
        // printf("\t");
      }
      // printf("\t%s\n", dir_buffer);

      depth++;
      dse_list_files_from_dir(filename);
    }
    else
    {
      char file_buffer[MAX_PATH] = {0};
      // sprintf(file_buffer, "%s/%s", path, filename);
      // sprintf(file_buffer, "%s", filename);
      for(u64 i = 0; i < depth; i++)
      {
        // printf("\t");
      }
      // printf("\t%s\n", file_buffer);
    }
  } while(FindNextFile(find_file_handle, &ffd) != 0);

  depth--;
}

void* dse_alloc(u64 capacity, DSE_AllocationType allocation_type)
{
  // TODO: Docs recommend to use both reserve and commit for allocation. But it seems to work fine if its called with just commit.
  return VirtualAlloc(null, capacity, allocation_type, PAGE_READWRITE);
}

void* dse_mem_alloc(u64 capacity)
{
  // TODO: Docs recommend to use both reserve and commit for allocation. But it seems to work fine if its called with just commit.
  return VirtualAlloc(null, capacity, COMMIT_MEMORY, PAGE_READWRITE);
}

void dse_commit_memory(void *memory, u64 capacity)
{
  VirtualAlloc(memory, capacity, COMMIT_MEMORY, PAGE_READWRITE);
}

void* dse_mem_realloc(void *old_memory, u64 new_capacity)
{
  // NOTE: Assuming that new_capacity is greater than older capacity.
  void *new_memory = dse_mem_alloc(new_capacity);

  u64 old_size = 0;

  MEMORY_BASIC_INFORMATION mbi;
  if(VirtualQuery(old_memory, &mbi, sizeof(mbi)))
  {
    old_size = mbi.RegionSize;
  }
  else
  {
    // puts("Failed to query memory info.");
  }

  memcpy(new_memory, old_memory, old_size);

  dse_free_memory(old_memory);

  return new_memory;
}

void dse_free_memory(void *memory)
{
  VirtualFree(memory, 0, MEM_RELEASE);
}

bool dse_has_freed_memory(void *memory)
{
  MEMORY_BASIC_INFORMATION memory_info;
  VirtualQuery(memory, &memory_info, sizeof(memory_info));
  return memory_info.State == MEM_FREE;
}

// TODO: Its not part of OS!
void dse_mem_set(void *memory, s8 value, u64 size)
{
  u8 *m = (u8*)memory;

  for(u64 i = 0; i < size; i++)
  {
    m[i] = value;
  }
}

void dse_mem_copy(void *destination, void *source, u64 size)
{
  u8 *d = (u8*)destination;
  u8 *s = (u8*)source;

  for(u64 i = 0; i < size; i++)
  {
    d[i] = s[i];
  }
}

bool dse_compare_memory(void *a, void *b, u64 size)
{
  bool result = true;

  if(size == 0) result = false;

  for(u64 offset = 0; offset < size; offset++)
  {
    if(*((u8*)a + offset) != *((u8*)b + offset))
    {
      result = false;
    }
  }

  return result;
}

#endif // DSE_OS_IMPLEMENTATION

#ifdef DSE_OS_STRIP_PREFIX
  #define generate_random_number dse_generate_random_number
  #define count_threads          dse_count_threads
  #define thread_handle          dse_thread_handle
  #define thread_proc            dse_thread_proc
  #define create_thread          dse_create_thread
  #define wait_for_thread        dse_wait_for_thread
  #define wait_for_all_threads   dse_wait_for_all_threads
  #define atomic_increment       dse_atomic_increment
  #define get_os_timer_frequency dse_get_os_timer_frequency
  #define get_os_timer           dse_get_os_timer
  #define get_cpu_timer          dse_get_cpu_timer
  #define list_files_from_dir    dse_list_files_from_dir
  #define alloc                  dse_alloc
  #define mem_alloc              dse_mem_alloc
  #define commit_memory          dse_commit_memory
  #define mem_realloc            dse_mem_realloc
  #define free_memory            dse_free_memory
  #define has_freed_memory       dse_has_freed_memory
  #define mem_set                dse_mem_set
  #define mem_copy               dse_mem_copy
  #define compare_memory         dse_compare_memory
#endif // DSE_OS_STRIP_PREFIX

#endif // DSE_OS_H

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