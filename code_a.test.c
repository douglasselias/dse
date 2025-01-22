DSE_START_TEST(code_a_test_1);
  int result = sum(1, 2);
  DSE_ASSERT(result == 2, "Result is not 3. Got: %d", result);
DSE_END_TEST();