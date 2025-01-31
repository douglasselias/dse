#include<stdlib.h>

/// @todo: Remove
#include<string.h>

#include "dse_tester.h"

// #define dse_bool _Bool
// #define dse_true  1
// #define dse_false 0

char* read_entire_file(const char* path) {
  FILE* file = fopen(path, "rb");

  if(file == NULL) {
    printf("File %s not found.\n", path);
    return "";
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  // printf("SIZE: %zd\n", size);
  fseek(file, 0, SEEK_SET);

  char* string = malloc(size + 1);
  fread(string, size, 1, file);
  fclose(file);

  string[size] = '\0';
  return string;
}

int char_index(const char* haystack, char needle) {
  for(int i = 0; i < dse_string_length(haystack); i++) {
    if(haystack[i] == needle) return i;
  }
  return -1;
}

void dse_concat_string(char* source, const char* string) {
  size_t source_len = dse_string_length(source);
  size_t string_len = dse_string_length(string);
  memcpy(source + source_len, string, string_len);
}

dse_bool strings_are_equal(char* a, char* b) {
  size_t a_length = dse_string_length(a);
  size_t b_length = dse_string_length(b);
  if(a_length != b_length) return dse_false;
  for(int i = 0; i < a_length; i++) {
    if(a[i] != b[i]) return dse_false;
  }
  return dse_true;
}

/// @todo: WIP
char** split_string(char* string, char delim, int* size) {
  char** strings = calloc(sizeof(char*), 10000);

  char* remaining_string = string;
  size_t remaining_length = dse_string_length(remaining_string);
  while(remaining_length > 0) {
    int index = char_index(remaining_string, delim);
    char* token = calloc(sizeof(char), index + 1);
    memcpy(token, remaining_string, index + 1);
    memcpy(remaining_string, remaining_string + index + 1, remaining_length - index - 1);
    remaining_string[remaining_length - index] = '\0';
    remaining_length = dse_string_length(remaining_string);
    // memset()
  }

  return strings;
}

#define max_tests 10000
char* test_infos[max_tests] = {0};
int test_info_index = 0;

void extract_name_of_test(char* text) {
  int test_name_length = 100;
  char* test_name = calloc(sizeof(char), test_name_length);
  /// @todo: Replace strtok
  char* line = strtok(text, "\n");

  while(line != NULL) {
    if(dse_has_substring(line, "DSE_SUITE(")
    || dse_has_substring(line, "DSE_TEST(")) {
      int parenthesis_index = char_index(line, '(') + 1;
      int coma_index = char_index(line, ',');

      test_infos[test_info_index] = calloc(sizeof(char), test_name_length);
      memcpy(test_infos[test_info_index], line + parenthesis_index, coma_index - parenthesis_index);
      test_info_index++;
    }

    line = strtok(NULL, "\n");
  }
}

int main(int argc, char* argv[]) {
  char suite_query[50] = {0};
  char test_query[50] = {0};

  for(dse_u64 i = 1; i < argc; i+=2) {
    if(strings_are_equal(argv[i], "-s")) {
      dse_copy_string(argv[i+1], suite_query);
    } else if(strings_are_equal(argv[i], "-t")) {
      dse_copy_string(argv[i+1], test_query);
    }
  }

  printf("This system has %lld processors\n", dse_count_threads());

  puts("Searching test files...");
  dse_list_files_from_dir("..");

  puts("Generating file...");
  FILE* generated_file = fopen("../build/generated.c", "w");

  fprintf(generated_file, "#include \"../dse_tester.h\"\n");

  for(dse_u64 i = 0; i < max_filenames; i++) {
    if(list_of_filenames[i]) {
      fprintf(generated_file, "#include \"%s\"\n", list_of_filenames[i]);
      char* file_text = read_entire_file(list_of_filenames[i]);
      extract_name_of_test(file_text);
      /// @todo: A better approach is to allocate once and reuse.
      free(file_text);
    }
  }

  fprintf(generated_file, "\nint main() {");

  /// @todo: WIP: Add a way to show the line number of the crash.
  // fprintf(generated_file, "\nAddVectoredExceptionHandler(1, VectoredHandler); // Register handler");

  // fprintf(generated_file, "\n\tdse_copy_string(\"%s\", dse_suite_query);", suite_query);
  // fprintf(generated_file, "\n\tdse_copy_string(\"%s\", dse_test_query);\n", test_query);

  for(dse_u64 i = 0; i < max_tests; i++) {
    if(test_infos[i] != NULL) {
      fprintf(generated_file, "\n\tdse_functions[dse_functions_index++] = %s;", test_infos[i]);
    }
  }

  /// @todo: Maybe use two for loops, one to create the threads and the other to run the threads.

  fprintf(generated_file,
    "\n"
    "\n\tdse_u64 available_threads = dse_count_threads() - 1;"
    "\n\tif(dse_functions_index >= available_threads) {"
    "\n\t\ttests_per_thread = dse_functions_index / available_threads;"
    "\n\t\tremaining_tests = dse_functions_index %% available_threads;"
    "\n\t}"
    "\n"
    "\n\tHANDLE* threads_array = calloc(sizeof(HANDLE), available_threads);"
    "\n\tfor(dse_u64 i = 0; i < available_threads; i++) {"
    "\n\t\tThreadArgs* args = calloc(sizeof(ThreadArgs), 1);"
    "\n\t\targs->start_index = i * tests_per_thread; args->end_index = (i + 1) * tests_per_thread;"
    "\n\t\tthreads_array[i] = dse_create_thread(my_thread_fn, (void*)args);"
    "\n\t\tdse_start_thread(threads_array[i]);"
    "\n\t}"

    "\n\tdse_wait_all_threads(threads_array, available_threads);"
  );


  fprintf(generated_file, "\n\tprintf(\"Test / Thread %%lld, Remaining: %%lld, index: %%lld\\n\", tests_per_thread, remaining_tests, dse_functions_index);");
  // fprintf(generated_file, "\n\n\tfor(dse_u64 dse_i = 0; dse_i < dse_functions_index; dse_i++) {");
  // fprintf(generated_file, "\n\t\tdse_functions[dse_i]();");
  // fprintf(generated_file, "\n\t}");
// 
  fprintf(generated_file, "\n\n\tDSE_PRINT_RESULTS();");

  fprintf(generated_file, "\n\treturn 0;\n}");
  fclose(generated_file);

  puts("Executing compiler...");
  dse_execute_shell_cmd("cl /nologo /diagnostics:caret /Wall /WX /W4 /wd4189 /wd4464 /wd5045 /wd4255 /wd4996 /wd4100 /wd4244 generated.c");
  dse_execute_shell_cmd("generated.exe");
}