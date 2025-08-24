#define DSE_ARRAY_IMPLEMENTATION
#define DSE_ARRAY_STRIP_PREFIX
#include "array.h"

int main()
{
  Array array = create_array(3);

  array_append(&array, 10);

  // print_array(array);
  printf("Array[0]: %d\n", array.data[0]);

  destroy_array(&array);

  return 0;
}