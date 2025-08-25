#define DSE_STRING8_IMPLEMENTATION
#include "string8.h"

int main()
{
  String8 s = DSE_STR8("Hello World");
  printf("S: %s\n", s.text);

  String8 s0 = dse_c_string_to_string8("HWLLE");
  printf(s0.text);

  return 0;
}