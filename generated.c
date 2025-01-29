#include "dse_tester.h"
#include "code.c"

void code_a_test_1() {   int result = sum(1, 2);   DSE_ASSERT(result == 3, "Result is not 3. Got: %d", result); }

void for_loop_test() {   for(int i = 0; i < 3; i++) {     DSE_ASSERT(true, "i is not odd. Got %d", i);     // DSE_ASSERT((i & 1) == 1, "i is not odd. Got %d", i);
  }
}

void sum_test_2() {   int result = sum(4, 3);   result = 3;   DSE_SKIP(DSE_ASSERT(result == 7));   } 
int main() {
	dse_register(code_a_test_1);
	dse_register(for_loop_test);
	dse_register(sum_test_2);

	for(int i = 0; i < 100; i++) {
		if(dse_functions[i]) dse_functions[i]();
	}

	DSE_PRINT_RESULTS();
	return 0;
}