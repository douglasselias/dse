@test(sum test 2) {
  int result = sum(4, 3);
  result = 3;
  DSE_SKIP(DSE_ASSERT(result == 7));  
}
