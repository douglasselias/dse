#include "code.c"

@suite(sum suite b) {
  @test(sum suite test 1) {
    int result = sum(1, 2);
    DSE_ASSERT(result == 2, "Result is not 3. Got: %d", result);
  }

  @test(sum suite test 2) {
    int result = sum(1, 2);
    DSE_ASSERT(result == 2, "Result is not 3. Got: %d", result);
  }
}