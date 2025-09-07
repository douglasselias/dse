#define DSE_ARENA_IMPLEMENTATION
#define DSE_ARENA_STRIP_PREFIX
#include "../arena/arena.h"

#include "array.h"

Struct(User)
{
  char *name;
  char *email;
};

void print_user(User user)
{
  printf("  %s, %s\n", user.name, user.email);
}

DSE_DECLARE_ARRAY_FUNCTIONS(user, User)

void print_array_user(Array_user array)
{
  printf("\n[\n");

  for(u32 i = 0; i < array.size; i++)
  {
    print_user(array.data[i]);
  }

  printf("]\n");
}

User users[] =
{
  {"one",   "one@email.com"},
  {"two",   "two@email.com"},
  {"three", "three@email.com"},
};

void arena_push_user(Arena *arena, User user)
{
  User *u = push_arena(&arena, sizeof(User));
  dse_mem_copy(u, &user, sizeof(User));
}

Struct(UserList)
{
  u64  size;
  User *users;
};

UserList create_user_list(Arena *arena)
{
  UserList user_list = {0};
  user_list.size  = arena->used / sizeof(User);
  user_list.users = (User*)arena->data;
  return user_list;
}

void pop_user(Arena *arena, u64 index)
{
  // TODO: Index can be out of bounds.
  u64 size = sizeof(User);
  u64 offset = size * index;

  dse_mem_copy(arena->data + offset, arena->data + offset + size, size);
  arena->used -= size;
}

int main()
{
  Arena *arena = create_arena(sizeof(User) * 3);

  for(u32 i = 0; i < 3; i++)
  { 
    arena_push_user(arena, users[i]);
  }

  for(u64 i = 0; i < 3; i++)
  {
    User *u = (User*)arena->data;
    print_user(u[i]);
  }

  dse_pop_from_index(arena, sizeof(User));
  arena_push_user(arena, users[0]);

  puts("Removed:");
  for(u64 i = 0; i < 3; i++)
  {
    User *u = (User*)arena->data;
    print_user(u[i]);
  }

  pop_user(arena, 1);

  puts("Removed (again):");
  u64 s = arena->used / sizeof(User);
  for(u64 i = 0; i < s; i++)
  {
    User *u = (User*)arena->data;
    print_user(u[i]);
  }

  // Array_user au = {};
  // au.size = 3;
  // au.data = arena->data;

  // array_remove_by_index_user(&au, 1);
  // puts("Removed:");

  // for(u64 i = 0; i < au.size; i++)
  // {
  //   print_user(au.data[i]);
  // }



  // User *users_array = (User*)arena->data;
  // u64 size = arena->used / sizeof(User);
  // for(u64 i = 0; i < size; i++)
  // {
  //   print_user(users_array[i]);
  // }

  // UserList ul = create_user_list(arena);
  // for(u64 i = 0; i < ul.size; i++)
  // {
  //   print_user(users_array[i]);
  // }
  

  // Array_user users = create_array_user((u32)-1);

  // array_append_user(&users, (User){"one",   "one@email.com"});
  // array_append_user(&users, (User){"two",   "two@email.com"});
  // array_append_user(&users, (User){"three", "three@email.com"});
  
  // array_remove_by_index_user(&users, 1);
  
  // print_array_user(users);

  // users.size = 0;
  // printf("Size after reset: %d\n", users.size);
  // print_array_user(users);

  // destroy_array_user(&users);
  // printf("Destroyed? %s\n", users.data == null ? "Yes" : "No");

  return 0;
}