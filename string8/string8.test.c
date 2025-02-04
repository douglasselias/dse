#define DSE_STRING8_IMPLEMENTATION
#include "string8.h"

void string_size() {
  dse_u64 result = __dse_size("hello");
  DSE_ASSERT(result == 5, "Got %lld", result);
}

void str8_macro() {
  String8 result = STR8("hello");
  DSE_ASSERT(result.size == 5, "Got %lld", result.size);
}

void test_strings_equal() {
  {
    String8 a = STR8("hello");
    String8 b = STR8("hello");
    bool result = dse_strings_are_equal(a, b);
    DSE_ASSERT(result, "Strings are not equal A: %s, B: %s", a.text, b.text);
  }

  {
    String8 a = STR8("hello");
    String8 b = STR8("hello!");
    bool result = dse_strings_are_equal(a, b);
    DSE_ASSERT(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
  }

  {
    String8 a = STR8("hello");
    String8 b = STR8("hellw");
    bool result = dse_strings_are_equal(a, b);
    DSE_ASSERT(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
  }
}

void to_uppercase() {
  String8 result = STR8("hello");
  dse_to_uppercase(result);
  char* target = "HELLO";
  for(dse_u64 i = 0; i < result.size; i++) {
    DSE_ASSERT(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void to_tolowercase() {
  String8 result = STR8("HELLO");
  dse_to_lowercase(result);
  char* target = "hello";
  for(dse_u64 i = 0; i < result.size; i++) {
    DSE_ASSERT(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void __to_pascal_case() {
  String8 result = STR8("hello world");
  dse_to_pascal_case(result);
  char* target = "Hello World";
  for(dse_u64 i = 0; i < result.size; i++) {
    DSE_ASSERT(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void snake_case() {
  String8 result = STR8("hello world");
  dse_to_snake_case(result);
  char* target = "hello_world";
  for(dse_u64 i = 0; i < result.size; i++) {
    DSE_ASSERT(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void kebab_case() {
  String8 result = STR8("hello world");
  dse_to_kebab_case(result);
  char* target = "hello-world";
  for(dse_u64 i = 0; i < result.size; i++) {
    DSE_ASSERT(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void string_copy() {
  String8 destination = {};
  char* source = "hello world";
  dse_string_copy(source, &destination);
  for(dse_u64 i = 0; i < destination.size; i++) {
    DSE_ASSERT(destination.text[i] == source[i], "Got %c", destination.text[i]);
  }
}

void ascii_code_to_char() {
  char c = dse_ascii_code_to_char(100);
  DSE_ASSERT(c == 'd', "Got %c", c);
}

void ascii_code_at() {
  String8 s = STR8("abcd");
  dse_u8 n = dse_ascii_code_at(s, 3);
  DSE_ASSERT(n == 100, "Got %c", n);
}

void __concat_strings() {
  String8 a = STR8("hello ");
  String8 b = STR8("sailor");
  String8* result = dse_concat_strings(a, b);
  char* s = "hello sailor";
  for(dse_u64 i = 0; i < result->size; i++) {
    DSE_ASSERT(result->text[i] == s[i], "Got %c", result->text[i]);
  }
}

void __append_char() {
  String8 result = STR8("hello");
  dse_append_char(&result, '!');
  char* s = "hello!";
  DSE_ASSERT(result.size == __dse_size(s), "Size is %lld", result.size);
  for(dse_u64 i = 0; i < result.size; i++) {
    DSE_ASSERT(result.text[i] == s[i], "Got %c", result.text[i]);
  }
}

void string_joins() {
  String8 a = STR8("hello");
  String8 b = STR8("sailor");
  String8 c = STR8("ready?");
  String8** arr = calloc(sizeof(String8*), 3);
  arr[0] = calloc(sizeof(String8), 1);
  arr[0]->text = a.text;
  arr[0]->size = a.size;
  arr[1] = calloc(sizeof(String8), 1);
  arr[1]->text = b.text;
  arr[1]->size = b.size;
  arr[2] = calloc(sizeof(String8), 1);
  arr[2]->text = c.text;
  arr[2]->size = c.size;
  String8* result = dse_string_join(arr, 3, '!');
  char* s = "hello!sailor!ready?";

  DSE_ASSERT(result->size == (a.size + b.size + c.size + 2), "Size is %lld", result->size);
  for(dse_u64 i = 0; i < result->size; i++) {
    DSE_ASSERT(result->text[i] == s[i], "Got %c", result->text[i]);
  }
}

void string_joins_strings() {
  String8 a = STR8("hello");
  String8 b = STR8("sailor");
  String8 c = STR8("ready?");
  String8 delim = STR8("<>");

  String8** arr = calloc(sizeof(String8*), 3);
  arr[0] = calloc(sizeof(String8), 1);
  arr[0]->text = a.text;
  arr[0]->size = a.size;
  arr[1] = calloc(sizeof(String8), 1);
  arr[1]->text = b.text;
  arr[1]->size = b.size;
  arr[2] = calloc(sizeof(String8), 1);
  arr[2]->text = c.text;
  arr[2]->size = c.size;

  String8* r = dse_string_join_string(arr, 3, delim);
  String8 result = *r;
  String8 target = STR8("hello<>sailor<>ready?");

  DSE_ASSERT(result.size == target.size, "Size is %lld", result.size);
  DSE_ASSERT(dse_strings_are_equal(result, target), "Got %s", result.text);
}

void string_includes() {
  String8 a = STR8("carpentry");
  String8 b = STR8("pen");
  bool result = dse_string_includes(a, b);
  DSE_ASSERT(result, "String '%s' does not include '%s'", a.text, b.text);
}

void sub_start_index() {
  {
    String8 a = STR8("carpentry");
    String8 b = STR8("pen");
    dse_u64 result = dse_substring_start_index(a, b);
    DSE_ASSERT(a.text[result] == 'p');
    DSE_ASSERT(result == 3, "Incorrect index, got %lld", result);
  }

  {
    String8 a = STR8("hello sailor");
    String8 b = STR8("sail");
    dse_u64 result = dse_substring_start_index(a, b);
    DSE_ASSERT(a.text[result] == 's');
    DSE_ASSERT(result == 6, "Incorrect index, got %lld", result);
  }
}

void sub_end_index() {
  {
    String8 a = STR8("carpentry");
    String8 b = STR8("pen");
    dse_u64 result = dse_substring_end_index(a, b);
    DSE_ASSERT(a.text[result] == 't', "Incorrect char, got %c", a.text[result]);
    DSE_ASSERT(result == 6, "Incorrect index, got %lld", result);
  }

  {
    String8 a = STR8("hello sailor");
    String8 b = STR8("sail");
    dse_u64 result = dse_substring_end_index(a, b);
    DSE_ASSERT(a.text[result] == 'o', "Incorrect char, got %c", a.text[result]);
    DSE_ASSERT(result == 10, "Incorrect index, got %lld", result);
  }
}

void slice_string() {
  String8 s = STR8("carpentry");
  String8 slice = *dse_slice_string(s, 3, 6);
  String8 target = STR8("pen");
  DSE_ASSERT(dse_strings_are_equal(slice, target), "Strings are not equal, got slice: %s", slice.text);
}

void string_split() {
  String8 string = STR8("hello\nworld\nsometext");
  char delim = '\n';
  String8** results = dse_string_split(string, delim);
  DSE_ASSERT(dse_strings_are_equal(*(results[0]), (String8)STR8("hello")));
  DSE_ASSERT(dse_strings_are_equal(*(results[1]), (String8)STR8("world")));
  DSE_ASSERT(dse_strings_are_equal(*(results[2]), (String8)STR8("sometext")));
}