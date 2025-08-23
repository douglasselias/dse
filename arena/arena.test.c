#include <stdio.h> // TODO: This should be on the header itself! Both arena and os.

#define DSE_ARENA_IMPLEMENTATION
#define DSE_ARENA_STRIP_PREFIX
#include "arena.h"

int main()
{
  Arena *arena = create_arena(10);

  u64 mem_size = 8;
  u64 *mem = push_arena(&arena, sizeof(u64) * mem_size);

  for(u64 i = 0; i < mem_size; i++)
  {
    mem[i] = (i + 1) * 10;
  }

  pop_arena(&arena, sizeof(u64));

  for(s64 i = 0; i < (s64)(arena->used/sizeof(s64)); i++)
  {
    printf("%lld\n", mem[i]);
  }

  destroy_arena(arena);

  return 0;
}