#define DSE_STRING8_IMPLEMENTATION
#include "string8.h"

int main()
{
  DSE_String8 s = DSE_STR8("Hello World");
  printf("S: %s\n", s.text);

  DSE_String8 s0 = dse_c_string_to_string8("HWLLE");
  printf(s0.text);
  puts("");

  {
    {
      DSE_String8 string = DSE_STR8("carpet");
      DSE_String8 target = DSE_STR8("car");
      printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
    }
     
    {
      DSE_String8 string = DSE_STR8("carpet");
      DSE_String8 target = DSE_STR8("");
      printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("");
      DSE_String8 target = DSE_STR8("");
      printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("a");
      DSE_String8 target = DSE_STR8("abc");
      printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("hexdecimal");
      DSE_String8 target = DSE_STR8("mal");
      printf("Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("m");
      DSE_String8 target = DSE_STR8("mal");
      printf("Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("m");
      DSE_String8 target = DSE_STR8("");
      printf("Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("");
      DSE_String8 target = DSE_STR8("");
      printf("Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    // pf(DSE_STR8("Result: %b\n"), dse_starts_with(string, target));
    // u64 a_big_number = 18446744073709551615;
    u64 a_big_number = 1234;
    pf(DSE_STR8("%d\n"), a_big_number);

    f64 a_big_float = 1234.5678;
    pf(DSE_STR8("%f\n"), a_big_float);


    pf(DSE_STR8("%s\n"), DSE_STR8("My Awesome String8!\n"));
  }

  return 0;
}