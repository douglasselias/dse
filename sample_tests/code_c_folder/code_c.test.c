#include "code_c.c"

DSE_TEST(for_test,
  for(int i = 0; i < 3; i++) {
    DSE_ASSERT((power_of_two(i) & 1) == 1, "i is not odd. Got %d", i);
  }
);
