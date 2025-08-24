#define DSE_ARRAY_IMPLEMENTATION
#define DSE_ARRAY_STRIP_PREFIX
#include "array.h"

Struct(User)
{
  // char name[20];
  // char email[20];
  char *name;
  char *email;
};

void print_user(User user)
{
  printf("  %s, %s\n", user.name, user.email);
}

DSE_CREATE_CUSTOM_ARRAY_TYPE_FUNCTIONS(, user, User)

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
  // Array array = create_array(3);

  // array_append(&array, 10);

  // // print_array(array);
  // printf("Array[0]: %d\n", array.data[0]);

  // destroy_array(&array);

  Array_user users = create_array_user((u32)-1);

  // User user_one   = {"one",   "one@email.com"};
  // User user_two   = {"two",   "two@email.com"};
  // User user_three = {"three", "three@email.com"};

  // array_append_user(&users, user_one);
  // array_append_user(&users, user_two);
  // array_append_user(&users, user_three);

  array_append_user(&users, (User){"one",   "one@email.com"});
  array_append_user(&users, (User){"two",   "two@email.com"});
  array_append_user(&users, (User){"three", "three@email.com"});
  
  array_remove_by_index_user(&users, 1);
  
  print_array_user(users);

  // reset_array_user(&users);
  users.size = 0;
  printf("Size after reset: %d\n", users.size);
  print_array_user(users);

  destroy_array_user(&users);
  printf("Destroyed? %s\n", users.data == null ? "Yes" : "No");
  // printf("Name: %s\n", users.data[0].name);

  return 0;
}