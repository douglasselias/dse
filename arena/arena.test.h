#define DSE_ARENA_IMPLEMENTATION
#define DSE_ARENA_STRIP_PREFIX
#include "arena.h"

#define DSE_OS_IMPLEMENTATION
#include "../os/os.h"

#define DSE_ARENA_ALLOC_COMMIT(size)          dse_alloc(size, COMMIT_MEMORY)
#define DSE_ARENA_ALLOC_RESERVE(size)         dse_alloc(size, RESERVE_MEMORY)
#define DSE_ARENA_FREE_MEMORY(memory)         dse_free_memory(memory)
#define DSE_ARENA_MEMSET(memory, value, size) dse_mem_set(memory, value, size)

#include <stdio.h>

typedef struct Entity Entity;
struct Entity
{
  u64 id;
  char *name;
  u64 x, y;
};

Arena* arena_create_entity(u64 size)
{
  Arena *arena = create_arena(sizeof(Entity) * size);
  return arena;
}

void arena_push_user(Arena *arena, User user)
{
  User *u = push_arena(&arena, sizeof(User));
  dse_mem_copy(u, &user, sizeof(User));
}

void arena_pop_user(Arena *arena, u64 index)
{
  // TODO: Index can be out of bounds.
  u64 size = sizeof(User);
  u64 offset = size * index;

  dse_mem_copy(arena->data + offset, arena->data + offset + size, size);
  arena->used -= size;
}

u64 arena_size_user(Arena *arena)
{
  u64 size = arena->used / sizeof(User);
  return size;
}

User* arena_pointer_user(Arena *arena)
{
  User *user = (User*)arena->data;
  return user;
}

void arena_user_iterate(Arena *arena, void proc(User *user))
{
  u64 size = arena_size_user(arena);
  for(u64 i = 0; i < size; i++)
  {
    User *user = arena_pointer_user(arena);
    proc(user);
  }
}

// User defined
void print_user(User user)
{
  printf("  %s, %s\n", user.name, user.email);
}

void print_user_ptr(User *user)
{
  printf("  %s, %s\n", user->name, user->email);
}

void print_users(Arena *arena)
{
  printf("\n[\n");

  u64 size = arena_size_user(arena);
  for(u64 i = 0; i < size; i++)
  {
    print_user(arena_pointer_user(arena)[i]);
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

   User users[] =
  {
    {"uno ",   "one@email.com"},
    {"dos ",   "two@email.com"},
    {"tres", "three@email.com"},
  };
  u64 users_size = ARRAY_SIZE(users);

  Arena *arena = arena_create_user(users_size);

  for(u32 i = 0; i < users_size; i++)
  { 
    arena_push_user(arena, users[i]);
  }

  print_users(arena);

  dse_pop_from_index(arena, sizeof(User));
  arena_push_user(arena, users[0]);

  puts("Freelist:");
  print_users(arena);

  arena_pop_user(arena, 1);

  puts("Removed:");
  // print_users(arena);
  // arena_user_iterate(arena, print_user);
  arena_user_iterate(arena, print_user_ptr);

  return 0;
}