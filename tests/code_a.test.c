


@test(code a test 1) {
  int result = sum(1, 2);
  DSE_ASSERT(result == 3, "Result is not 3. Got: %d", result);
}