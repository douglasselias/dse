#define DSE_OS_IMPLEMENTATION
#include "../os/os.h"

#define DSE_ARENA_ALLOC_COMMIT(size)          dse_alloc(size, COMMIT_MEMORY)
#define DSE_ARENA_ALLOC_RESERVE(size)         dse_alloc(size, RESERVE_MEMORY)
#define DSE_ARENA_COMMIT_MEMORY(memory, size) dse_commit_memory(memory, size)
#define DSE_ARENA_FREE_MEMORY(memory)         dse_free_memory(memory)
#define DSE_ARENA_MEMSET(memory, value, size) dse_mem_set(memory, value, size)

#define DSE_ARENA_IMPLEMENTATION
#define DSE_ARENA_STRIP_PREFIX
#include "arena.h"

#include <stdio.h>

#define MEMORY_FREED_CHECK(has_freed_memory_proc, memory)                                   \
  do                                                                  \
  {                                                                   \
    u64 line = __LINE__;                                              \
    if(!has_freed_memory_proc(memory))                                \
    {                                                                 \
      printf("Memory not freed (%s) at line: %lld\n", #memory, line); \
    }                                                                 \
  } while(0)                                                          \

typedef struct Entity Entity;
struct Entity
{
  u64  id;
  char *name;
};

Arena* arena_create_entity(u64 size)
{
  Arena *arena = dse_arena_create(sizeof(Entity) * size);
  return arena;
}

void arena_push_entity(Arena *arena, Entity entity)
{
  Entity *e = arena_push(&arena, sizeof(Entity));
  dse_mem_copy(e, &user, sizeof(Entity));
}

void arena_pop_entity(Arena *arena, u64 index)
{
  // TODO: Index can be out of bounds.
  u64 size = sizeof(Entity);
  u64 offset = size * index;

  dse_mem_copy(arena->data + offset, arena->data + offset + size, size);
  arena->used -= size;
}

u64 arena_size_entity(Arena *arena)
{
  u64 size = arena->used / sizeof(Entity);
  return size;
}

Entity* arena_pointer_entity(Arena *arena)
{
  Entity *entity = (Entity*)arena->data;
  return entity;
}

void arena_iterate_entities(Arena *arena, void proc(Entity *entity))
{
  u64 size = arena_size_entity(arena);
  for(u64 i = 0; i < size; i++)
  {
    Entity *entity = arena_pointer_entity(arena);
    proc(entity);
  }
}

// User defined //
void print_ptr_entity(Entity *entity)
{
  printf("  %lld, %s\n", entity->id, entity->name);
}

void print_entities(Arena *arena)
{
  printf("\n[\n");

  u64 size = arena_size_entity(arena);
  for(u64 i = 0; i < size; i++)
  {
    print_ptr_entity(arena_pointer_entity(arena));
  }

  printf("]\n");
}

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

s32 arena_tests()
{
  // Arena *arena = create_arena(10);

  // u64 mem_size = 8;
  // u64 *mem = push_arena(&arena, sizeof(u64) * mem_size);

  // for(u64 i = 0; i < mem_size; i++)
  // {
  //   mem[i] = (i + 1) * 10;
  // }

  // pop_arena(&arena, sizeof(u64));
  // pop_arena(&arena, sizeof(u64) * 2);

  // for(s64 i = 0; i < (s64)(arena->used/sizeof(s64)); i++)
  // {
  //   printf("%lld\n", mem[i]);
  // }

  // destroy_arena(arena);

  Entity entities[] =
  {
    {10, "player"},
    {20, "enemy"},
    {30, "floor"},
  };
  u64 entities_size = ARRAY_SIZE(entities);

  Arena *arena = arena_create_entity(entities);

  for(u32 i = 0; i < users_size; i++)
  { 
    arena_push_entity(arena, entities[i]);
  }

  print_entities(arena);

  dse_pop_from_index(arena, sizeof(Entity));
  arena_push_entity(arena, entities[0]);

  puts("Freelist:");
  print_entities(arena);

  arena_pop_user(arena, 1);

  puts("Removed:");
  // print_entities(arena);
  // arena_user_iterate(arena, print_user);
  arena_user_iterate(arena, print_pointer_entity);

  return 0;
}