#define DSE_STRING8_IMPLEMENTATION
#include "string8.h"

#include <stdlib.h>
#include <string.h>
// #include <stdargs.h>

DSE_String8 create_string(char *s, u64 size)
{
  DSE_String8 result = {0};

  // result.size = size;
  result.text = calloc(1, size);
  memset(result.text, '\0', size);

  // dse_string_copy(s, &result);
  u64 source_size = __dse_string_size(s);
  result.size = source_size;

  for(u64 i = 0; i < source_size; i++)
  {
    result.text[i] = s[i];
  }

  return result;
}

int main()
{

  // {
  //   DSE_String8 s = DSE_STR8("Hello World");
  //   printf("S: %s\n", s.text);
    
  //   DSE_String8 s0 = dse_c_string_to_string8("HWLLE");
  //   printf(s0.text);
  //   puts("");
  // }

  // {
  //   {
  //     DSE_String8 string = DSE_STR8("carpet");
  //     DSE_String8 target = DSE_STR8("car");
  //     printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
  //   }
     
  //   {
  //     DSE_String8 string = DSE_STR8("carpet");
  //     DSE_String8 target = DSE_STR8("");
  //     printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
  //   }

  //   {
  //     DSE_String8 string = DSE_STR8("");
  //     DSE_String8 target = DSE_STR8("");
  //     printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
  //   }

  //   {
  //     DSE_String8 string = DSE_STR8("a");
  //     DSE_String8 target = DSE_STR8("abc");
  //     printf("Result: %s\n", dse_starts_with(string, target) ? "true" : "false");
  //   }

    {
      DSE_String8 string = DSE_STR8("hexdecimal");
      DSE_String8 target = DSE_STR8("mal");
      printf("END: Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("m");
      DSE_String8 target = DSE_STR8("mal");
      printf("END: Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("m");
      DSE_String8 target = DSE_STR8("");
      printf("END: Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 string = DSE_STR8("");
      DSE_String8 target = DSE_STR8("");
      printf("END: Result: %s\n", dse_ends_with(string, target) ? "true" : "false");
    }

    {
      DSE_String8 b = DSE_STR8("SAILOR");
      DSE_String8 a = create_string("HELLO ", 20);

      dse_append_string(&a, b);
      printf(STR8_FMT"\n", STR8_ARG(a));
    }

    {
      DSE_String8 a = create_string("HELLO", 5 + 3);

      dse_append_char(&a, '!');
      dse_append_char(&a, '!');
      dse_append_char(&a, '!');
      printf(STR8_FMT"\n", STR8_ARG(a));
    }

  //   // dse_pf(DSE_STR8("Result: %b\n"), dse_starts_with(string, target));
  //   // u64 a_big_number = 18446744073709551615;
  //   u64 a_big_number = 1234;
  //   dse_pf(DSE_STR8("%d\n"), a_big_number);

  //   f64 a_big_float = 1234.5678;
  //   dse_pf(DSE_STR8("%f\n"), a_big_float);


  //   dse_pf(DSE_STR8("%s\n"), DSE_STR8("My Awesome String8!\n"));
  // }

  // {
  //   DSE_String8 string = DSE_STR8("Hello World");
  //   char *result = dse_to_c_string(string);

  //   printf("String8 to C: %s.\n", result);

  //   DSE_String8 result_string = dse_c_string_to_string8(result);
  //   dse_pf(DSE_STR8("C to String8: %s.\n"), result_string);

  //   printf(STR8_FMT, STR8_ARG(result_string));

  //   dse_cpf("\nCPF FN: %s\n", result_string);
  // }

  // {
  //   char *str = "  1HE?LLO-1239-WORLD9  ";
  //   DSE_String8 s = {0};
  //   s.text = calloc(1, __dse_string_size(str) + 1);

  //   dse_string_copy(str, &s);

  //   dse_slugify(s);
  //   dse_cpf("Slugified: |%s|\n", s);
  // }

  // {
  //   char *str = "     HELLO-WORLD    ";
  //   DSE_String8 s = {0};
  //   s.text = calloc(1, __dse_string_size(str) + 1);

  //   dse_string_copy(str, &s);

  //   DSE_String8 result = dse_trim_both(s);
  //   DSE_String8 l = dse_trim_left2(s);
  //   DSE_String8 r = dse_trim_right2(s);
  //   dse_cpf("Trimmed Both: |%s|\n", result);
  //   dse_cpf("Trimmed Left: |%s|\n", l);
  //   dse_cpf("Trimmed Right: |%s|\n", r);
  //   printf("BOTH:" STR8_FMT  ":\n", STR8_ARG(result));
  //   printf("LEFT:" STR8_FMT  ":\n", STR8_ARG(l));
  //   printf("RIGHT:" STR8_FMT ":\n", STR8_ARG(r));
  // }

  return 0;
}