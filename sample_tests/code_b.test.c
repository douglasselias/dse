#include "code.c"

DSE_SUITE(code_b_suite,
  DSE_SUITE_TEST(sum_is_three,
    int result = sum(1, 1);
    DSE_ASSERT(result == 3, "Result is not 3. Got: %d", result);
  );

  DSE_SUITE_TEST(sum_is_five,
    int result = sum(2, 2);
    DSE_ASSERT(result == 5, "Result is not 5. Got: %d", result);
  );
);
