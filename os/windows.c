#include "os.h"

#include<stdlib.h>
#include<stdio.h>
#define WIN32_LEAN_AND_MEAN
#include<windows.h>

// #define dse_atomic_s64 LONG64
// #define dse_thread_proc_t LPTHREAD_START_ROUTINE

#define max_filenames 1000
char* list_of_filenames[max_filenames] = {0};
dse_u64 insert_index = 0;


#define dse_bool _Bool
#define dse_true  1
#define dse_false 0

char dse_to_lowercase(char c) {
  if('A' <= c && c <= 'Z') return c + 32;
  else return c;
}

dse_u64 internal_dse_string_length(const char* string) {
  dse_u64 length = 0;
  while(*string++ != '\0') length++;
  return length;
}

void dse_copy_string(const char* source, char* destination) {
  dse_u64 source_length = internal_dse_string_length(source);
  for(dse_u64 i = 0; i < source_length; i++) {
    destination[i] = source[i];
  }
}

void dse_concat_string(char* source, const char* string) {
  size_t source_len = internal_dse_string_length(source);
  size_t string_len = internal_dse_string_length(string);
  memcpy(source + source_len, string, string_len);
}

dse_bool dse_has_substring(const char* haystack, const char* needle) {
  dse_u64 haystack_length = internal_dse_string_length(haystack);
  dse_u64 needle_length = internal_dse_string_length(needle);
  if(needle_length == 0) return dse_true;
  if(haystack_length < needle_length) return dse_false;
  dse_u64 haystack_index = 0;
  dse_u64 needle_index = 0;

  while(haystack_index < haystack_length) {
    if(dse_to_lowercase(haystack[haystack_index]) == dse_to_lowercase(needle[needle_index])) {
      needle_index++;
      if(needle_index < needle_length) {
        haystack_index++;
      } else return dse_true;
    } else {
      needle_index = 0;
      haystack_index++;
    }
  }

  return dse_false;
}

void dse_execute_shell_cmd(char* cmd) {
  system(cmd);
}

dse_u64 dse_count_threads() {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
}

dse_thread_id dse_create_thread(dse_thread_proc_t thread_fn, void* thread_args) {
  LPSECURITY_ATTRIBUTES default_security_attr = NULL;
  dse_u64 default_stack_size = 0;
  HANDLE id = CreateThread(default_security_attr, default_stack_size, thread_fn.value, thread_args, CREATE_SUSPENDED, NULL);
  return (dse_thread_id){id};
}

void dse_start_thread(dse_thread_id id) {
  ResumeThread(id.value);
}

void dse_wait_all_threads(HANDLE* thread_array, dse_u64 total_threads) {
  WaitForMultipleObjects((DWORD)total_threads, thread_array, dse_true, INFINITE);
}

void dse_atomic_increment(dse_atomic_s64 n) {
  InterlockedIncrement64(&(n.value));
}

void dse_list_files_from_dir(const char* path) {
  char dir_name[MAX_PATH] = {0};
  dse_copy_string(path, dir_name);
  dse_concat_string(dir_name, "\\*");

  WIN32_FIND_DATA ffd;
  HANDLE find_file_handle = FindFirstFile(dir_name, &ffd);

  if(INVALID_HANDLE_VALUE == find_file_handle) {
    puts("Invalid file.");
    return;
  }

  do {
    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if(ffd.cFileName[0] != '.') {
        char dir_buffer[MAX_PATH] = {0};
        dse_copy_string(path, dir_buffer);
        dse_concat_string(dir_buffer, "\\");
        dse_concat_string(dir_buffer, ffd.cFileName);
        dse_list_files_from_dir(dir_buffer);
      }
    } else {
      LARGE_INTEGER filesize;
      filesize.LowPart = ffd.nFileSizeLow;
      filesize.HighPart = ffd.nFileSizeHigh;
      if(dse_has_substring(ffd.cFileName, ".test.")) {
        list_of_filenames[insert_index] = calloc(sizeof(char), (internal_dse_string_length(path) + internal_dse_string_length(ffd.cFileName) + 2)); // one slash between '..' and the file, and the null terminator.

        dse_concat_string(list_of_filenames[insert_index], path);
        dse_concat_string(list_of_filenames[insert_index], "/");
        dse_concat_string(list_of_filenames[insert_index], ffd.cFileName);

        /// @note: Convert backwards slash to forward slash
        int i = 0;
        size_t filename_length = internal_dse_string_length(list_of_filenames[insert_index]);
        while(i < filename_length) {
          if(list_of_filenames[insert_index][i] == '\\') {
            list_of_filenames[insert_index][i] = '/';
          }
          i++;
        }

        // list_of_filesizes[insert_index] = filesize.QuadPart;
        puts(list_of_filenames[insert_index]);
        insert_index++;
      }
    }
  } while(FindNextFile(find_file_handle, &ffd) != 0);
}
