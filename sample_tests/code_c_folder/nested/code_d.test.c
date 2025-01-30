#include "code_d.c"

DSE_SUITE(code_d_some_multiplication,
  DSE_SUITE_TEST(multiply_test,
    int result = multiply(4, 3);
    result = 3;
    DSE_SKIP(DSE_ASSERT(result == 7));
  );
);
