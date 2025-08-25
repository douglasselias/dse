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

DSE_DECLARE_ARRAY_FUNCTIONS(, user, User)

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
  memcpy(u, &user, sizeof(User));
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

int main()
{
  Arena *arena = create_arena(sizeof(User) * 3);

  for(u32 i = 0; i < 3; i++)
  { 
    // User *u = push_arena(&arena, sizeof(User));
    // memcpy(u, &users[i], sizeof(User));
    arena_push_user(arena, users[i]);
  }

  User *users_array = (User*)arena->data;
  u64 size = arena->used / sizeof(User);
  for(u64 i = 0; i < size; i++)
  {
    print_user(users_array[i]);
  }

  UserList ul = create_user_list(arena);
  for(u64 i = 0; i < ul.size; i++)
  {
    print_user(users_array[i]);
  }
  

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