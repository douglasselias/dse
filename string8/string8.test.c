#define DSE_STRING8_IMPLEMENTATION
#include "string8.h"

void string_size() {
  dse_u64 result = __dse_size("hello");
  assertion(result == 5, "Got %lld", result);
}

void str8_macro() {
  String8 result = STR8("hello");
  assertion(result.size == 5, "Got %lld", result.size);
}

void test_strings_equal() {
  {
    String8 a = STR8("hello");
    String8 b = STR8("hello");
    bool result = dse_strings_are_equal(a, b);
    assertion(result, "Strings are not equal A: %s, B: %s", a.text, b.text);
  }

  {
    String8 a = STR8("hello");
    String8 b = STR8("hello!");
    bool result = dse_strings_are_equal(a, b);
    assertion(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
  }

  {
    String8 a = STR8("hello");
    String8 b = STR8("hellw");
    bool result = dse_strings_are_equal(a, b);
    assertion(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
  }
}

void to_uppercase() {
  String8 result = STR8("hello");
  dse_to_uppercase(result);
  char* target = "HELLO";
  for(dse_u64 i = 0; i < result.size; i++) {
    assertion(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void to_tolowercase() {
  String8 result = STR8("HELLO");
  dse_to_lowercase(result);
  char* target = "hello";
  for(dse_u64 i = 0; i < result.size; i++) {
    assertion(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void to_pascal_case() {
  String8 result = STR8("hello world");
  dse_to_pascal_case(result);
  char* target = "Hello World";
  for(dse_u64 i = 0; i < result.size; i++) {
    assertion(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void snake_case() {
  String8 result = STR8("hello world");
  dse_to_snake_case(result);
  char* target = "hello_world";
  for(dse_u64 i = 0; i < result.size; i++) {
    assertion(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void kebab_case() {
  String8 result = STR8("hello world");
  dse_to_kebab_case(result);
  char* target = "hello-world";
  for(dse_u64 i = 0; i < result.size; i++) {
    assertion(result.text[i] == target[i], "Got %c", result.text[i]);
  }
}

void string_copy() {
  String8 destination = {};
  char* source = "hello world";
  dse_string_copy(source, &destination);
  for(dse_u64 i = 0; i < destination.size; i++) {
    assertion(destination.text[i] == source[i], "Got %c", destination.text[i]);
  }
}

void ascii_code_to_char() {
  char c = dse_ascii_code_to_char(100);
  assertion(c == 'd', "Got %c", c);
}

void ascii_code_at() {
  String8 s = STR8("abcd");
  dse_u8 n = dse_ascii_code_at(s, 3);
  assertion(n == 100, "Got %c", n);
}

void concat_strings() {
  String8 a = STR8("hello ");
  String8 b = STR8("sailor");
  String8* result = dse_concat_strings(a, b);
  String8 s = STR8("hello sailor");
  assertion(result->size == (a.size + b.size), "(concat strings) Got %lld", result->size);
  assertion(dse_strings_are_equal(*result, s), "(concat strings) Got |%s|", result->text);
}

void append_char() {
  String8* result = calloc(sizeof(String8), 1);
  char* hello = "help";
  dse_string_copy(hello, result);

  dse_append_char(result, '!');
  char* s = "help!";

  assertion(result->size == __dse_size(s), "Size is %lld", result->size);
  for(dse_u64 i = 0; i < result->size; i++) {
    assertion(result->text[i] == s[i], "Got %c", result->text[i]);
  }
}

void string_joins() {
  String8 a = STR8("hello");
  String8 b = STR8("sailor");
  String8 c = STR8("ready?");
  String8** arr = calloc(sizeof(String8*), 3);
  arr[0] = calloc(sizeof(String8), 1);
  arr[0] = &a;
  arr[1] = calloc(sizeof(String8), 1);
  arr[1] = &b;
  arr[2] = calloc(sizeof(String8), 1);
  arr[2] = &c;
  String8* result = dse_string_join(arr, 3, '!');
  char* s = "hello!sailor!ready?";

  assertion(result->size == (a.size + b.size + c.size + 2), "Size is %lld", result->size);
  for(dse_u64 i = 0; i < result->size; i++) {
    assertion(result->text[i] == s[i], "Got %c", result->text[i]);
  }
}

void string_joins_strings() {
  String8 a = STR8("hello");
  String8 b = STR8("sailor");
  String8 c = STR8("ready?");
  String8 delim = STR8("<>");

  String8** arr = calloc(sizeof(String8*), 3);
  arr[0] = calloc(sizeof(String8), 1);
  arr[1] = calloc(sizeof(String8), 1);
  arr[2] = calloc(sizeof(String8), 1);
  arr[0] = &a;
  arr[1] = &b;
  arr[2] = &c;

  String8* r = dse_string_join_string(arr, 3, delim);
  String8 result = *r;
  String8 target = STR8("hello<>sailor<>ready?");

  assertion(result.size == target.size, "Size is %lld", result.size);
  assertion(dse_strings_are_equal(result, target), "Got %s", result.text);
}

void string_includes() {
  String8 a = STR8("carpentry");
  String8 b = STR8("pen");
  bool result = dse_string_includes(a, b);
  assertion(result, "String '%s' does not include '%s'", a.text, b.text);
}

void sub_start_index() {
  {
    String8 a = STR8("carpentry");
    String8 b = STR8("pen");
    dse_u64 result = dse_substring_start_index(a, b);
    assertion(a.text[result] == 'p');
    assertion(result == 3, "Incorrect index, got %lld", result);
  }

  {
    String8 a = STR8("hello sailor");
    String8 b = STR8("sail");
    dse_u64 result = dse_substring_start_index(a, b);
    assertion(a.text[result] == 's');
    assertion(result == 6, "Incorrect index, got %lld", result);
  }
}

void sub_end_index() {
  {
    String8 a = STR8("carpentry");
    String8 b = STR8("pen");
    dse_u64 result = dse_substring_end_index(a, b);
    assertion(a.text[result] == 't', "Incorrect char, got %c", a.text[result]);
    assertion(result == 6, "Incorrect index, got %lld", result);
  }

  {
    String8 a = STR8("hello sailor");
    String8 b = STR8("sail");
    dse_u64 result = dse_substring_end_index(a, b);
    assertion(a.text[result] == 'o', "Incorrect char, got %c", a.text[result]);
    assertion(result == 10, "Incorrect index, got %lld", result);
  }
}

void slice_string() {
  String8 s = STR8("carpentry");
  String8 slice = *dse_slice_string(s, 3, 6);
  String8 target = STR8("pen");
  assertion(dse_strings_are_equal(slice, target), "Strings are not equal, got slice: %s", slice.text);
}

void string_split() {
  String8 string = STR8("hello\nworld\nsometext");
  char delim = '\n';
  String8** results = dse_string_split(string, delim);
  assertion(dse_strings_are_equal(*(results[0]), (String8)STR8("hello")));
  assertion(dse_strings_are_equal(*(results[1]), (String8)STR8("world")));
  assertion(dse_strings_are_equal(*(results[2]), (String8)STR8("sometext")));
}

void test_string_split_string() {
  String8 string = STR8("hello<>world<>sometext");
  String8 delim = STR8("<>");
  String8** results = dse_string_split_string(string, delim);
  assertion(dse_strings_are_equal(*(results[0]), (String8)STR8("hello")));
  assertion(dse_strings_are_equal(*(results[1]), (String8)STR8("world")));
  assertion(dse_strings_are_equal(*(results[2]), (String8)STR8("sometext")));
}

void test_string_format() {
  String8 format = STR8("A %s without %s is just a %s.");
  String8 a = STR8("world");
  String8 b = STR8("L");
  String8 c = STR8("word");
  String8* result = dse_string_format(format, a, b, c);
  String8 target = STR8("A world without L is just a word.");
  bool are_equal = dse_strings_are_equal(*result, target);
  assertion(are_equal, "(String Format) Expected %s, Got %s", target.text, result->text);
}

void test_string_printf() {
  String8 format = STR8("A %s without %s is just a %s.");
  String8 a = STR8("world");
  String8 b = STR8("L");
  String8 c = STR8("word");
  // dse_string_printf(format, a, b, c);
}

void int_to_string() {
  dse_u64 n = 123456789;
  String8* result = dse_int_to_string(n);
  String8 n_str = STR8("123456789");
  assertion(result->size == n_str.size, "Got size: %lld, but expected %lld", result->size, n_str.size);
  assertion(dse_strings_are_equal(*result, n_str), "Got result: %s |", result->text);
}

void string_to_int() {
  {
    String8 n_str = STR8("1");
    dse_s64 n = dse_string_to_int(n_str);
    assertion(n == 1, "Got %lld", n);
  }

  {
    String8 n_str = STR8("100");
    dse_s64 n = dse_string_to_int(n_str);
    assertion(n == 100, "Got %lld", n);
  }

  {    
    String8 n_str = STR8("123456789");
    dse_s64 n = dse_string_to_int(n_str);
    assertion(n == 123456789, "Got %lld", n);
  }
}

void test_slugify() {
  String8 input = STR8("  ABC_!_@#123 hello  ");
  String8 result = *dse_slugify(input);
  String8 target = STR8("abc-----123-hello");
  assertion(dse_strings_are_equal(result, target), "Wrong result, got |%s|", result.text);
}

void string_trim() {
  String8 s = STR8("  abc  ");
  String8 result = *dse_trim(s);
  String8 target = STR8("abc");
  assertion(dse_strings_are_equal(result, target), "Wrong result, got %s", result.text);
}

void test_string_replace() {
  String8 abc = STR8("abc-def-ghi");
  String8* s = calloc(sizeof(String8), 1);
  s->size = abc.size;
  s->text = calloc(sizeof(char), s->size);
  dse_string_copy(abc.text, s);

  dse_string_replace(&s, '-', '+');

  String8 target = STR8("abc+def+ghi");
  assertion(dse_strings_are_equal(*s, target), "Wrong result, got %s", s->text);
}

void test_string_replace_string() {
  String8 abc = STR8("abc<>def<>ghi");
  String8* s = calloc(sizeof(String8), 1);
  s->size = abc.size;
  s->text = calloc(sizeof(char), s->size);
  dse_string_copy(abc.text, s);

  dse_string_replace_string(&s, (String8)STR8("<>"), (String8)STR8("##"));

  String8 target = STR8("abc##def##ghi");
  assertion(dse_strings_are_equal(*s, target), "Wrong result, got %s", s->text);
}

void remove_chars() {
  String8 s = STR8("abc-def-ghi");
  String8 result = *dse_remove_chars(s, '-');
  String8 target = STR8("abcdefghi");
  assertion(dse_strings_are_equal(result, target), "Wrong result, got %s", result.text);
}

void test_remove_strings() {
  String8 input = STR8("abc<>def<>ghi");
  String8 delim = STR8("<>");
  String8 result = *dse_remove_strings(input, delim);
  String8 target = STR8("abcdefghi");
  assertion(dse_strings_are_equal(result, target), "(test_remove_strings) Wrong result, got %s", result.text);
}

void test_index_of() {
  String8 input = STR8("abc<>def<>ghi");
  char target = '<';
  dse_s64 i = dse_index_of(input, target, 6);
  assertion(i == 8, "Got %lld", i);
  assertion(input.text[i] == target);
}

void test_last_index_of() {
  String8 input = STR8("abc<>def<>ghi");
  char target = '<';
  dse_s64 i = dse_last_index_of(input, target, 5);
  assertion(i == 3, "Got %lld", i);
  assertion(input.text[i] == target);
}

void simple_fuzzy() {
  { 
    String8 string = STR8("carpentry");
    String8 pattern = STR8("carry");
    bool result = simple_fuzzy_match(string, pattern);
    assertion(result, "Did not match: %d", result);
  }

  {
    String8 string = STR8("carpentr");
    String8 pattern = STR8("carry");
    bool result = simple_fuzzy_match(string, pattern);
    assertion(!result, "Did not match: %d", result);
  }
}