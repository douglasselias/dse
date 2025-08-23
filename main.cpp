#include <stdio.h>

#define DSE_STRING8_IMPLEMENTATION
#include "string8/string8.h"

#define DSE_ARENA_IMPLEMENTATION
#include "arena/arena.h"

#define DSE_ARRAY_IMPLEMENTATION
#include "array/array.h"

#define DSE_INIT_TESTS()              \
  dse_u64 dse_failed_tests_count = 0; \

#define ASSERTION(expression, ...)                                   \
  if(!(expression))                                                  \
  {                                                                  \
    printf("\n\033[31mFAILED\033[0m\t%s:%d : ", __FILE__, __LINE__); \
    printf("" __VA_ARGS__);                                          \
    dse_failed_tests_count++;                                        \
  }                                                                  \

#define DSE_PRINT_ALL_TESTS_PASSED()         \
  puts("");                                  \
  if(dse_failed_tests_count == 0)            \
  {                                          \
    puts("\033[36mALL TESTS PASSED\033[0m"); \
  }                                          \


#define MAX_STR_SIZE 20
typedef struct User User;
struct User
{
  char username[MAX_STR_SIZE];
  char email[MAX_STR_SIZE];
};

void print_user(User *u)
{
  User user = *u;
  printf("  Username: %*s  | Email: %*s\n", MAX_STR_SIZE, user.username, MAX_STR_SIZE, user.email);
}

DSE_CREATE_CUSTOM_ARRAY_TYPE_FUNCTIONS(User, user);

void dse_print_array(DSE_Array array)
{
  printf("[");

  for(dse_u64 i = 0; i < array.size; i++)
  {
    char final_char = i < array.size - 1 ? ',' : '\0';
    printf("%d%c", array.data[i], final_char);
  }

  printf("]\n");
}

int main()
{
  DSE_INIT_TESTS();

  { // __dse_string_size function test
    dse_u64 result = __dse_string_size("hello");
    dse_u64 target = 5;
    ASSERTION(result == target, "Expected %lld, but got %lld", target, result);
  }

  { // DSE_STR8 macro test
    String8 result = DSE_STR8("hello");
    String8 target = {"hello", 5};

    ASSERTION(result.size == target.size, "Expected %lld, but got %lld", target.size, result.size);
    ASSERTION(dse_strings_are_equal(result, target), "Expected %s, but got |%s|", target.text, result.text);
  }

  { // dse_strings_are_equal test
    {
      String8 a = DSE_STR8("hello");
      String8 b = DSE_STR8("hello");
      
      bool result = dse_strings_are_equal(a, b);
      
      ASSERTION(result == true, "Strings are not equal A: %s, B: %s", a.text, b.text);
    }
    
    {
      String8 a = DSE_STR8("hello");
      String8 b = DSE_STR8("hello!");
      
      bool result = dse_strings_are_equal(a, b);
      
      ASSERTION(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
    }
    
    {
      String8 a = DSE_STR8("hello");
      String8 b = DSE_STR8("hellw");
      
      bool result = dse_strings_are_equal(a, b);
      
      ASSERTION(result == false, "Strings are equal??? A: %s, B: %s", a.text, b.text);
    }
  }

  {
    String8 input  = DSE_STR8("hello");
    String8 target = DSE_STR8("HELLO");

    String8 result = dse_string_to_uppercase(input);

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  {
    String8 input  = DSE_STR8("///");
    String8 target = DSE_STR8("///");

    String8 result = dse_string_to_uppercase(input);

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  {
    String8 input  = DSE_STR8("HELLO");
    String8 target = DSE_STR8("hello");

    String8 result = dse_string_to_lowercase(input);

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  {
    String8 input  = DSE_STR8("///");
    String8 target = DSE_STR8("///");

    String8 result = dse_string_to_lowercase(input);

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }


  { // Append string test
    String8 a      = DSE_STR8("hello ");
    String8 b      = DSE_STR8("sailor");
    String8 target = DSE_STR8("hello sailor");

    String8 result = dse_append_string(a, b);

    ASSERTION(result.size == target.size, "Got %lld", result.size);
    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  { // Append char test
    String8 input  = DSE_STR8("help");
    String8 target = DSE_STR8("help!");

    String8 result = dse_append_char(input, '!');

    ASSERTION(result.size == target.size, "Size is %lld", result.size);
    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  { // Join strings with char test
    #define SIZE 5
    String8 strings[SIZE] =
    {
      DSE_STR8("Hello"),
      DSE_STR8("sailor!"),
      DSE_STR8("Are"),
      DSE_STR8("you"),
      DSE_STR8("ready?"),
    };

    String8 result = dse_join_strings_with_char(strings, SIZE, ' ');
    String8 target = DSE_STR8("Hello sailor! Are you ready?");

    ASSERTION(result.size == target.size, "Size is %lld", result.size);
    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  { // Join strings with string test
    #define SIZE 5
    String8 strings[SIZE] =
    {
      DSE_STR8("Hello"),
      DSE_STR8("sailor!"),
      DSE_STR8("Are"),
      DSE_STR8("you"),
      DSE_STR8("ready?"),
    };
    String8 delim = DSE_STR8("__");

    String8 result = dse_join_strings_with_string(strings, SIZE, delim);
    String8 target = DSE_STR8("Hello__sailor!__Are__you__ready?");

    ASSERTION(result.size == target.size, "Size is %lld", result.size);
    ASSERTION(dse_strings_are_equal(result, target), "Got %s", result.text);
  }

  { // String split test
    String8 string = DSE_STR8("hello\nworld\nsometext");
    char delim = '\n';
    dse_u64 strings_count = 0;

    String8 targets[3] =
    {
      DSE_STR8("hello"),
      DSE_STR8("world"),
      DSE_STR8("sometext"),
    };

    String8* results = dse_split_string_with_char(string, delim, &strings_count);

    for(dse_u8 i = 0; i < 3; i++)
    {
      ASSERTION(dse_strings_are_equal(results[i], targets[i]));
    }
    ASSERTION(strings_count == 3);
  }

  { // String split string test
    String8 input = DSE_STR8("hello<>world<>sometext");
    String8 delim = DSE_STR8("<>");
    dse_u64 strings_count = 0;

    String8 targets[3] =
    {
      DSE_STR8("hello"),
      DSE_STR8("world"),
      DSE_STR8("sometext"),
    };

    String8* results = dse_split_string_with_string(input, delim, &strings_count);

    for(dse_u8 i = 0; i < 3; i++)
    {
      ASSERTION(dse_strings_are_equal(results[i], targets[i]));
    }
    ASSERTION(strings_count == 3);
  }

  { // String includes test
    String8 a = DSE_STR8("carpentry");
    String8 b = DSE_STR8("pen");

    bool result = dse_string_includes(a, b);

    ASSERTION(result, "String '%s' does not include '%s'", a.text, b.text);
  }

  { // Substring start index test
    {
      String8 a = DSE_STR8("carpentry");
      String8 b = DSE_STR8("pen");

      dse_u64 result = dse_substring_start_index(a, b);

      ASSERTION(a.text[result] == 'p');
      ASSERTION(result == 3, "Incorrect index, got %lld", result);
    }

    {
      String8 a = DSE_STR8("hello sailor");
      String8 b = DSE_STR8("sail");

      dse_u64 result = dse_substring_start_index(a, b);

      ASSERTION(a.text[result] == 's');
      ASSERTION(result == 6, "Incorrect index, got %lld", result);
    }
  }

  { // Substring end index test
    {
      String8 a = DSE_STR8("carpentry");
      String8 b = DSE_STR8("pen");

      dse_u64 result = dse_substring_end_index(a, b);

      ASSERTION(a.text[result] == 't', "Incorrect char, got %c", a.text[result]);
      ASSERTION(result == 6, "Incorrect index, got %lld", result);
    }

    {
      String8 a = DSE_STR8("hello sailor");
      String8 b = DSE_STR8("sail");

      dse_u64 result = dse_substring_end_index(a, b);

      ASSERTION(a.text[result] == 'o', "Incorrect char, got %c", a.text[result]);
      ASSERTION(result == 10, "Incorrect index, got %lld", result);
    }
  }

  { // Slice string test
    String8 input = DSE_STR8("carpentry");
    String8 slice = dse_slice_string(input, 3, 6);
    String8 target = DSE_STR8("pen");

    ASSERTION(dse_strings_are_equal(slice, target), "Strings are not equal, got slice: %s", slice.text);
  }

  { // Int to string test
    dse_u64 number = 123456789;
    String8 result = dse_int_to_string(number);
    String8 target = DSE_STR8("123456789");

    ASSERTION(result.size == target.size, "Got size: %lld, but expected %lld", result.size, target.size);
    ASSERTION(dse_strings_are_equal(result, target), "Got result: %s |", result.text);
  }

  { // String to int test
    {
      String8 n_str = DSE_STR8("1");
      dse_s64 n = dse_string_to_int(n_str);
      ASSERTION(n == 1, "Got %lld", n);
    }

    {
      String8 n_str = DSE_STR8("100");
      dse_s64 n = dse_string_to_int(n_str);
      ASSERTION(n == 100, "Got %lld", n);
    }

    {    
      String8 n_str = DSE_STR8("123456789");
      dse_s64 n = dse_string_to_int(n_str);
      ASSERTION(n == 123456789, "Got %lld", n);
    }

    {
      String8 n_str = DSE_STR8("-123456789");
      dse_s64 n = dse_string_to_int(n_str);
      ASSERTION(n == -123456789, "Got %lld", n);
    }

    {
      String8 n_str = DSE_STR8("-9223372036854775808");
      dse_s64 n = dse_string_to_int(n_str);
      dse_s64 target = -9223372036854775808LL;
      ASSERTION(n == target, "Got %lld", n);
    }

    {
      String8 n_str = DSE_STR8("9223372036854775807");
      dse_s64 n = dse_string_to_int(n_str);
      dse_s64 target = 9223372036854775807LL;
      ASSERTION(n == target, "Got %lld", n);
    }

    /// TODO: Add option for unsined ints
    // {
    //   String8 n_str = DSE_STR8("0xffffffffffffffffui64");
    //   dse_s64 n = dse_string_to_int(n_str);
    //   dse_s64 target = 9223372036854775807LL;
    //   ASSERTION(n == target, "Got %lld", n);
    // }
  }

  { // Slugify test
    String8 input  = DSE_STR8("  ABC_!_@#123 hello  ");
    String8 result = dse_slugify(input);
    String8 target = DSE_STR8("abc-----123-hello");

    ASSERTION(dse_strings_are_equal(result, target), "Wrong result, got |%s|", result.text);
  }

  { // Trim test
    String8 input  = DSE_STR8("  abc  ");

    String8 result_left  = dse_trim_left (input);
    String8 result_right = dse_trim_right(input);
    String8 result_both  = dse_trim      (input);

    String8 target_left  = DSE_STR8("abc  ");
    String8 target_right = DSE_STR8("  abc");
    String8 target_both  = DSE_STR8("abc");

    ASSERTION(dse_strings_are_equal(result_left, target_left), "Got %s", result_left.text);
    ASSERTION(dse_strings_are_equal(result_right, target_right), "Got %s", result_right.text);
    ASSERTION(dse_strings_are_equal(result_both, target_both), "Got %s", result_both.text);
  }

  { // String replace with char test
    String8 input  = DSE_STR8("abc-def-ghi");
    String8 target = DSE_STR8("abc+def+ghi");

    String8 result = dse_string_replace_char(input, '-', '+');

    ASSERTION(dse_strings_are_equal(result, target), "Got %s", result.text);
  }

  { // String replace with string test
    String8 input  = DSE_STR8("abc<>def<>ghi");
    String8 target = DSE_STR8("abc##def##ghi");

    String8 delim_target = DSE_STR8("<>");
    String8 replacement  = DSE_STR8("##");

    String8 result = dse_string_replace_string(input, delim_target, replacement);

    ASSERTION(dse_strings_are_equal(result, target), "Got %s", result.text);
  }

  { // Remove chars test
    String8 input  = DSE_STR8("abc-def-ghi");
    String8 target = DSE_STR8("abcdefghi");

    String8 result = dse_remove_chars(input, '-');

    ASSERTION(dse_strings_are_equal(result, target), "Wrong result, got %s", result.text);
  }

  { // Remove strings test
    String8 input = DSE_STR8("abc<>def<>ghi");
    String8 target = DSE_STR8("abcdefghi");
    String8 delim = DSE_STR8("<>");

    String8 result = dse_remove_strings(input, delim);

    ASSERTION(dse_strings_are_equal(result, target), "(test_remove_strings) Wrong result, got %s", result.text);
  }

  { // Index of char test
    String8 input = DSE_STR8("abc<>def<>ghi");
    char target = '<';

    dse_s64 i = dse_index_of(input, target);

    ASSERTION(i == 3, "Got %lld", i);
    ASSERTION(input.text[i] == target);
  }

  { // Index of char from index test
    String8 input = DSE_STR8("abc<>def<>ghi");
    char target = '<';
    dse_u64 from_index = 6;

    dse_s64 i = dse_index_of_from(input, target, from_index);

    ASSERTION(i == 8, "Got %lld", i);
    ASSERTION(input.text[i] == target);
  }

  { // Last index of char test
    String8 input = DSE_STR8("abc<>def<>ghi");
    char target = '<';
    dse_u64 from_index = 5;

    dse_s64 i = dse_last_index_of(input, target, from_index);

    ASSERTION(i == 3, "Got %lld", i);
    ASSERTION(input.text[i] == target);
  }

  { // Simple fuzzy matching test
    {
      String8 input   = DSE_STR8("carpentry");
      String8 pattern = DSE_STR8("carry");

      bool result = simple_fuzzy_match(input, pattern);

      ASSERTION(result, "Did not match: %d", result);
    }

    {
      String8 input   = DSE_STR8("carpentr");
      String8 pattern = DSE_STR8("carry");

      bool result = simple_fuzzy_match(input, pattern);

      ASSERTION(!result, "Did not match: %d", result);
    }
  }

  { // String view test
    String8 input = DSE_STR8("carpet");
    dse_u64 start_index = 3, end_index = input.size;
    String8 result = dse_string_view(input, start_index, end_index);
    String8 target = DSE_STR8("pet");

    ASSERTION(dse_strings_are_equal(result, target), "Got %s", result.text);
  }

  { // String view size test
    String8 input = DSE_STR8("carpet");
    dse_u64 start_index = 3, size = 3;
    String8 result = dse_string_view_size(input, start_index, size);
    String8 target = DSE_STR8("pet");

    ASSERTION(dse_strings_are_equal(result, target), "Got |%s|", result.text);
  }

  { // Print string using macros test
    String8 input = DSE_STR8("Hello World!");
    printf("\nThe message: " STR8_FMT ", More data: %d\n", STR8_ARG(input), 10);
  }

  /// ARENA: ///

  { // Push arena test
    dse_u64 arena_size = 10;
    Arena* arena = dse_create_arena(arena_size);

    for(dse_u8 i = 0; i < arena_size; i++)
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = 10 * (i+1);

      ASSERTION(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d", i, arena->data[i]);
    }

    ASSERTION(arena->used == (dse_s64)arena_size);
  }

  { // Push arena chaining test
    dse_u64 arena_size = 1;
    Arena* arena = dse_create_arena(arena_size);

    for(dse_u8 i = 0; i < arena_size; i++)
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = 10 * (i+1);
    }

    dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
    *block = 77;
    ASSERTION(arena->capacity == 2, "Cap %lld", arena->capacity);
    ASSERTION(*(arena->data) == 77);
  }

  { // Set position back test
    dse_u64 arena_size = 10;
    Arena* arena = dse_create_arena(arena_size);

    for(dse_u8 i = 0; i < arena_size; i++)
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = i + 1;
    }

    dse_u64 half_arena_size = arena_size / 2;
    arena->used -= half_arena_size;
    ASSERTION(arena->used == (dse_s64)half_arena_size);

    for(dse_u8 i = 5; i < arena_size; i++)
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = 10 * (i+1);
    }

    for(dse_u8 i = 0; i < arena_size; i++)
    {
      if(i < half_arena_size)
      {
        ASSERTION(arena->data[i] == (i+1), "Arena data: %d:%d", i, arena->data[i]);
      }
      else
      {
        ASSERTION(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d: Expected %d", i, arena->data[i], (10 * (i+1)));
      }
    }
  }

  { // Destroy arena test
    dse_u64 arena_size = 1;
    Arena* arena = dse_create_arena(arena_size);

    dse_push_arena(&arena, 1);
    // Chain
    dse_push_arena(&arena, 1);
    dse_push_arena(&arena, 1);
    // Chain
    dse_push_arena(&arena, 1);

    dse_destroy_arena(arena);

    /// TODO: Add assertions?
  }

  { // Pop arena test
    dse_u64 arena_size = 1;
    Arena* arena = dse_create_arena(arena_size);
    dse_u8 target_a = 10;
    dse_u8 target_b = 20;

    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = target_a;
    }
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = target_b;
    }

    {  
      dse_pop_arena(&arena, 1);
      
      dse_u8 value = *(arena->data);
      ASSERTION(value == target_b, "Value %d", value);
    }

    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = target_a;
      
      dse_u8 value = *(arena->data);
      ASSERTION(value == target_a, "Value %d", value);
    }

    {
      ASSERTION(arena->capacity == 2, "Arena size %lld", arena->capacity);

      dse_pop_arena(&arena, 1);
      dse_pop_arena(&arena, 1);
      
      ASSERTION(arena->capacity == 1, "Arena size %lld", arena->capacity);
    }

    {
      dse_pop_arena(&arena, 1);
      ASSERTION(arena->used == 0, "Arena size %lld", arena->used);
    }
  }

  { // Freelist test
    dse_u64 arena_size = 10;
    Arena* arena = dse_create_arena(arena_size);

    for(dse_u8 i = 0; i < arena_size; i++)
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = i * 10;
    }

    dse_u8 free_index = 4;
    dse_pop_from_index(arena, free_index);

    dse_u8 target = 77;
    {
      dse_u8* block = (dse_u8*)dse_push_arena(&arena, 1);
      *block = target;
    }

    for(dse_u8 i = 0; i < arena_size; i++)
    {
      if(i == free_index)
      {
        ASSERTION(arena->data[i] == target, "Arena data: %d:%d", i, arena->data[i]);
      }
      else
      {
        ASSERTION(arena->data[i] == i * 10, "Arena data: %d:%d", i, arena->data[i]);
      }
    }
  }

  { // Destroy array test
    DSE_Array array = dse_create_array(1);
    dse_destroy_array(&array);
    ASSERTION(array.capacity == 0, "Capacity is not zero, is %lld", array.capacity);
  }

  { // Append array test
    DSE_Array array = dse_create_array(1);

    dse_u8 first_target = 10, second_target = 20;
    dse_array_append(&array, first_target);
    dse_array_append(&array, second_target);

    ASSERTION(array.data[0] == first_target,  "data[0] is not %d, but got %d", first_target, array.data[0]);
    ASSERTION(array.data[1] == second_target, "data[1] is not %d, but got %d", second_target, array.data[1]);
    ASSERTION(array.capacity == 2, "Capacity is not %d, but got %lld", 2, array.capacity);
    ASSERTION(array.size == 2, "Size is not %d, but got %lld", 2, array.size);
  }

  { // Remove by index test
    DSE_Array array = dse_create_array(1);

    dse_u8 first_target = 10, second_target = 20, third_target = 30, fourth_target = 40;
    dse_array_append(&array, first_target);
    dse_array_append(&array, second_target);
    dse_array_append(&array, third_target);
    dse_array_append(&array, fourth_target);

    dse_print_array(array);
    dse_array_remove_by_index(&array, 1);
    dse_print_array(array);

    ASSERTION(array.data[0] == first_target,  "data[0] is not %d, but got %d\n", first_target, array.data[0]);
    ASSERTION(array.data[1] == third_target, "data[1] is not %d, but got %d\n", third_target, array.data[1]);
    ASSERTION(array.capacity == 4, "Capacity is not %d, but got %lld\n", 4, array.capacity);
    ASSERTION(array.size == 3, "Size is not %d, but got %lld\n", 3, array.size);
  }

  { // Reset array test
    DSE_Array array = dse_create_array(1);

    dse_u8 first_target = 10, second_target = 20, third_target = 30;
    dse_array_append(&array, first_target);
    dse_array_append(&array, second_target);

    dse_reset_array(&array);
    dse_array_append(&array, third_target);

    printf("Reset array test: ");
    dse_print_array(array);

    ASSERTION(array.data[0] == third_target,  "data[0] is not %d, but got %d", third_target, array.data[0]);
    ASSERTION(array.data[1] == second_target, "data[1] is not %d, but got %d", second_target, array.data[1]);
    ASSERTION(array.capacity == 2, "Capacity is not %d, but got %lld", 2, array.capacity);
    ASSERTION(array.size == 1, "Size is not %d, but got %lld", 1, array.size);
  }

  { // Custom array type
    DSE_Array array = dse_create_array((dse_u64)-1);

    #define USERNAME "doug"
    #define EMAIL    "doug@email.com"

    array_append_user(&array, {USERNAME, EMAIL});
    array_append_user(&array, {"elias", "elias@email.com"});

    User *result = array_get_by_index_user(array, 0);

    ASSERTION(strcmp(USERNAME, result->username) == 0, "Username is not %s, but got %s", USERNAME, result->username);
    ASSERTION(strcmp(EMAIL, result->email) == 0, "Email is not %s, but got %s", EMAIL, result->email);

    User *result1 = array_get_by_index_user(array, 1);

    char username[20] = "elias";
    char email[20] = "elias@email.com";
    ASSERTION(strcmp(username, result1->username) == 0, "Username is not %s, but got %s", username, result1->username);
    ASSERTION(strcmp(email, result1->email) == 0, "Email is not %s, but got %s", email, result1->email);

    #undef USERNAME
    #undef EMAIL

    dse_print_array_user(array);
  }

  DSE_PRINT_ALL_TESTS_PASSED();

  return 0;
}