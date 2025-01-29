@test(for loop test) {
  for(int i = 0; i < 3; i++) {
    DSE_ASSERT(true, "i is not odd. Got %d", i);
    // DSE_ASSERT((i & 1) == 1, "i is not odd. Got %d", i);
  }
}