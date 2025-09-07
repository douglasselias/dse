#define DSE_OS_IMPLEMENTATION
#define DSE_OS_STRIP_PREFIX
#include "os.h"

#include <stdio.h>

void custom_thread_proc(void *args)
{
  printf("Printing args from thread: %d\n", *(u8*)args);
}

int main()
{
  u8 number = generate_random_number();
  printf("Random number: %d\n", number);

  u32 threads_count = count_threads();
  printf("Threads count: %d\n", threads_count);

  u8 args[] =
  {
    10,
    20,
    30,
  };
  
  thread_handle handles[] =
  {
    create_thread(custom_thread_proc, &(args[0])),
    create_thread(custom_thread_proc, &(args[1])),
    create_thread(custom_thread_proc, &(args[2])),
  };
  wait_for_all_threads(handles, 3, INFINITE_TIMEOUT);
  
  u8 arg = 100;
  thread_handle handle = create_thread(custom_thread_proc, &arg);
  wait_for_thread(handle, INFINITE_TIMEOUT);

  s64 atomic_number = 10;
  atomic_increment(&atomic_number);
  printf("Atomic number: %lld\n", atomic_number);

  list_files_from_dir(".");

  u64 cpu_timer = get_cpu_timer();
  printf("CPU Timer: %lld\n", cpu_timer);

  u64 freq     = get_os_timer_frequency();
  u64 os_timer = get_os_timer();
  printf("Freq: %lld, Timer: %lld\n", freq, os_timer);

  u64 *mem = alloc(sizeof(u64) * 3, RESERVE_MEMORY);

  for(u64 i = 0; i < 3; i++)
  {
    commit_memory(&mem[i], sizeof(u64));
    mem[i] = (i + 1) * 10;
  }

  for(u64 i = 0; i < 3; i++)
  {
    printf("%lld\n", mem[i]);
  }

  free_memory(mem);

  if(dse_has_freed_memory(mem))
  {
    puts("Memory freed!");
  }
  else
  {
    puts("Memory not freed! :(");
  }

  return 0;
}
