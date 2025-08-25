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

int main()
{
  Array_user users = create_array_user((u32)-1);

  array_append_user(&users, (User){"one",   "one@email.com"});
  array_append_user(&users, (User){"two",   "two@email.com"});
  array_append_user(&users, (User){"three", "three@email.com"});
  
  array_remove_by_index_user(&users, 1);
  
  print_array_user(users);

  users.size = 0;
  printf("Size after reset: %d\n", users.size);
  print_array_user(users);

  destroy_array_user(&users);
  printf("Destroyed? %s\n", users.data == null ? "Yes" : "No");

  return 0;
}