#include "../base_types.h"

#ifndef DSE_OS_H
#define DSE_OS_H

#define INFINITE_TIMEOUT 0xFFFFFFFF

#define dse_thread_handle void*
typedef void dse_thread_proc(void *args);

u8 dse_generate_random_number();
u32 dse_count_threads();
dse_thread_handle dse_create_thread(dse_thread_proc *thread_proc, void *args);
void dse_wait_for_thread(dse_thread_handle handle, u32 timeout_ms);
void dse_wait_for_all_threads(dse_thread_handle *handles, u64 threads_count, u32 timeout_ms);
void dse_atomic_increment(s64 *number);
u64 dse_get_cpu_timer();
void dse_list_files_from_dir(char *path);

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
    printf("[ERROR]: Couldn't generate a random number, status code 0x%08x\n", status);
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
  sprintf(search_path, "%s\\*", path);

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
      sprintf(dir_buffer, "%s", filename);
      for(u64 i = 0; i < depth; i++)
      {
        printf("\t");
      }
      printf("\t%s\n", dir_buffer);

      depth++;
      dse_list_files_from_dir(filename);
    }
    else
    {
      char file_buffer[MAX_PATH] = {0};
      // sprintf(file_buffer, "%s/%s", path, filename);
      sprintf(file_buffer, "%s", filename);
      for(u64 i = 0; i < depth; i++)
      {
        printf("\t");
      }
      printf("\t%s\n", file_buffer);
    }
  } while(FindNextFile(find_file_handle, &ffd) != 0);

  depth--;
}


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
#endif // DSE_OS_STRIP_PREFIX

#endif // DSE_OS_IMPLEMENTATION
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