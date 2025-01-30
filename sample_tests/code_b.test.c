#include "code_b.c"

DSE_SUITE(code_b_suite,
  DSE_SUITE_TEST(subtract_is_three,
    int result = subtract(1, 1);
    DSE_ASSERT(result == 3, "Result is not 3. Got: %d", result);
  );

  DSE_SUITE_TEST(subtract_is_five,
    int result = subtract(2, 1);
    DSE_ASSERT(result == 5, "Result is not 5. Got: %d", result);
  );
);
