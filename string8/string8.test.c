#define DSE_STRING8_IMPLEMENTATION
#include "string8.h"

DSE_TEST(sum,
  int result = sum(2, 3);
  DSE_ASSERT(result == 5, "Got %d\n", result);
);