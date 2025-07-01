#include <windows.h>

#define null ((void*)0)

#ifndef __cplusplus
  #define bool  _Bool
  #define false 0
  #define true  1
#endif

typedef   signed long long dse_s64;
typedef unsigned char      dse_u8;
typedef unsigned int       dse_u32;
typedef unsigned long long dse_u64;

#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
void generate_random_numbers()
{
  UCHAR buffer[3];
  
  NTSTATUS status = BCryptGenRandom(NULL, buffer, sizeof(buffer), BCRYPT_USE_SYSTEM_PREFERRED_RNG);
  
  if(status == 0)
  {
    printf("Random bytes: ");

    for(int i = 0; i < sizeof(buffer); i++)
    {
      printf("%d ", buffer[i]);
    }

    printf("\n");
  }
  else
  {
    printf("Error: 0x%08x\n", status);
  }
}

uint32_t dse_count_available_threads()
{
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwNumberOfProcessors;
}

typedef HANDLE dse_thread_handle;
typedef void(*dse_thread_proc)(void *args);

typedef struct
{
  dse_thread_proc thread_proc;
  void *args;
} DSEThreadProcWrapperArgs;

DWORD __dse_thread_proc_wrapper(void *args)
{
  DSEThreadProcWrapperArgs wrapper_args = *(DSEThreadProcWrapperArgs*)args;
  wrapper_args.thread_proc(wrapper_args.args);
  return 0;
}

dse_thread_handle dse_create_thread(DSEThreadProcWrapperArgs* wrapper_args)
{
  return CreateThread(null, 0, __dse_thread_proc_wrapper, wrapper_args, 0, null);
}

void wait_all_threads(dse_thread_handle* thread_array, dse_u64 total_threads)
{
  WaitForMultipleObjects((DWORD)total_threads, thread_array, true, INFINITE);
}

void atomic_increment(dse_s64* number)
{
  InterlockedIncrement64(number);
}

#define __max_filenames 1000
char* __list_of_filenames[__max_filenames] = {0};
uint64_t __filename_insert_index = 0;

void list_files_from_dir(const char* path)
{
  char dir_name[MAX_PATH] = {0};
  strcat(dir_name, path);
  strcat(dir_name, "\\*");

  WIN32_FIND_DATA ffd;
  HANDLE find_file_handle = FindFirstFile(dir_name, &ffd);

  if(INVALID_HANDLE_VALUE == find_file_handle) return;

  do
  {
    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      if(ffd.cFileName[0] != '.')
      {
        char dir_buffer[MAX_PATH] = {0};
        strcat(dir_buffer, path);
        strcat(dir_buffer, "\\");
        strcat(dir_buffer, ffd.cFileName);
        __list_files_from_dir(dir_buffer);
      }
    }
    else
    {
      if(__has_substring(ffd.cFileName, ".test."))
      {
        __list_of_filenames[__filename_insert_index] = calloc(sizeof(char), (strlen(path) + strlen("/") + strlen(ffd.cFileName) + strlen(" \0")));

        strcat(__list_of_filenames[__filename_insert_index], path);
        strcat(__list_of_filenames[__filename_insert_index], "/");
        strcat(__list_of_filenames[__filename_insert_index], ffd.cFileName);

        /// @note: Convert backwards slash to forward slash
        uint64_t i = 0;
        size_t filename_length = strlen(__list_of_filenames[__filename_insert_index]);
        while(i < filename_length)
        {
          if(__list_of_filenames[__filename_insert_index][i] == '\\')
          {
            __list_of_filenames[__filename_insert_index][i] = '/';
          }

          i++;
        }

        __filename_insert_index++;
      }
    }
  } while(FindNextFile(find_file_handle, &ffd) != 0);
}

#include <intrin.h>
// #include <windows.h>

u64 os_timer_freq()
{
  LARGE_INTEGER f;
  QueryPerformanceFrequency(&f);
  return f.QuadPart;
}

u64 os_timer()
{
  LARGE_INTEGER c;
  QueryPerformanceCounter(&c);
  return c.QuadPart;
}

u64 cpu_timer()
{
  return __rdtsc();
}