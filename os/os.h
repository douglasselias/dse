#ifndef OS_H
#define OS_H

#include<stdint.h>

/// @todo: Remove unused types.
// typedef int8_t   dse_s8;
// typedef int16_t  dse_s16;
// typedef int32_t  dse_s32;
typedef int64_t  dse_s64;
// typedef uint8_t  dse_u8;
// typedef uint16_t dse_u16;
// typedef uint32_t dse_u32;
typedef uint64_t dse_u64;

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
typedef struct { LONG64 value; } dse_atomic_s64;
typedef struct { HANDLE value; } dse_thread_id;
typedef struct { LPTHREAD_START_ROUTINE value; } dse_thread_proc_t;
#else
#endif



/// @todo: Maybe it should return a list of paths
void dse_list_files_from_dir(const char* path);

dse_u64 dse_count_threads();
dse_thread_id dse_create_thread(dse_thread_proc_t thread_fn, void* thread_args);
void dse_start_thread(dse_thread_id h);
void dse_wait_all_threads();
void dse_atomic_increment(dse_atomic_s64 n);

void dse_execute_shell_cmd(char* cmd);


#endif // OS_H