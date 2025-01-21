// dse_tester.h - v0.0.0 - dual license: public domain/MIT - https://github.com/douglasselias/dse

// Add a how to use

#ifndef DSE_TESTER_H
#define DSE_TESTER_H

#ifdef DSE_TESTER_IMPLEMENTATION

int dse_total_tests = 0;
int dse_total_tests_skipped = 0;
int dse_total_tests_failed = 0;
int dse_total_tests_passed = 0;

#define DSE_ASSERT(expression, ...) \
  dse_total_tests++; \
  if(!(expression)) { \
    dse_total_tests_failed++; \
    printf("\033[31mFAILED\033[0m\t"); \
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    printf("" __VA_ARGS__); \
    puts(""); \
  } else { \
    printf("\033[32mPASSED\033[0m\t"); \
    printf("Line: %s:%d  ", __FILE__, __LINE__); \
    puts(""); \
  } \

#define DSE_SKIP(expression) \
  if(false) { expression } \
  dse_total_tests_skipped++; \
  dse_total_tests++; \
  printf("\033[93mSKIPPED\033[0m\tLine: %s:%d\n", __FILE__, __LINE__); \

#define DSE_PRINT_RESULTS() \
  printf("Total tests: %d\n", dse_total_tests); \
  printf("Total skipped: %d\n", dse_total_tests_skipped); \
  printf("Total failed: %d\n", dse_total_tests_failed); \
  dse_total_tests_passed = dse_total_tests - dse_total_tests_skipped - dse_total_tests_failed; \
  printf("Total passed: %d\n", dse_total_tests_passed); \

#define DSE_TIMER(function) \
  clock_t start = clock(); \
  function(); \
  clock_t end = clock(); \
  printf("%s was executed in %2.3f seconds\n", #function, (double)(end - start) / CLOCKS_PER_SEC); \

#define DSE_START_SUITE(name) \
  void name() { \
    printf("Suite: %s\n", #name); \

#define DSE_END_SUITE() }

#endif // DSE_TESTER_IMPLEMENTATION
#endif // DSE_TESTER_H

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - Public Domain (www.unlicense.org)

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - MIT License

Copyright (c) 2025 Douglas S. Elias

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/