#include<stdlib.h>

/// @todo: Remove
#include<string.h>

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include "dse_tester.h"

void execute_shell_cmd(char* cmd) {
  system(cmd);
}

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
  for(int i = 0; i < string_length(haystack); i++) {
    if(haystack[i] == needle) return i;
  }
  return -1;
}

void concat_string(char* source, const char* string) {
  size_t source_len = string_length(source);
  size_t string_len = string_length(string);
  memcpy(source + source_len, string, string_len);
}

bool strings_are_equal(char* a, char* b) {
  size_t a_length = string_length(a);
  size_t b_length = string_length(b);
  if(a_length != b_length) return false;
  for(int i = 0; i < a_length; i++) {
    if(a[i] != b[i]) return false;
  }
  return true;
}

/// @todo: WIP
char** split_string(char* string, char delim, int* size) {
  char** strings = calloc(sizeof(char*), 10000);

  char* remaining_string = string;
  size_t remaining_length = string_length(remaining_string);
  while(remaining_length > 0) {
    int index = char_index(remaining_string, delim);
    char* token = calloc(sizeof(char), index + 1);
    memcpy(token, remaining_string, index + 1);
    memcpy(remaining_string, remaining_string + index + 1, remaining_length - index - 1);
    remaining_string[remaining_length - index] = '\0';
    remaining_length = string_length(remaining_string);
    // memset()
  }

  return strings;
}

#define max_filenames 1000
char* list_of_filenames[max_filenames] = {0};
/// @todo: Maybe I can use this in the read_entire_file function. Maybe a better approach is to allocate space once for all files. Using an arena to reset after parsing the file.
// int list_of_filesizes[max_filenames] = {0};
int insert_index = 0;

void list_files_from_dir(const char* path);

void list_files_from_dir(const char* path) {
  char dir_name[MAX_PATH] = {0};
  copy_string(path, dir_name);
  concat_string(dir_name, "\\*");

  WIN32_FIND_DATA ffd;
  HANDLE find_file_handle = FindFirstFile(dir_name, &ffd);

  if(INVALID_HANDLE_VALUE == find_file_handle) {
    puts("Invalid file.");
    return;
  }

  do {
    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if(ffd.cFileName[0] != '.') {
        char dir_buffer[MAX_PATH] = {0};
        copy_string(path, dir_buffer);
        concat_string(dir_buffer, "\\");
        concat_string(dir_buffer, ffd.cFileName);
        list_files_from_dir(dir_buffer);
      }
    } else {
      LARGE_INTEGER filesize;
      filesize.LowPart = ffd.nFileSizeLow;
      filesize.HighPart = ffd.nFileSizeHigh;
      if(has_substring(ffd.cFileName, ".test.")) {
        list_of_filenames[insert_index] = calloc(sizeof(char), (string_length(path) + string_length(ffd.cFileName) + 2)); // one slash between '..' and the file, and the null terminator.

        concat_string(list_of_filenames[insert_index], path);
        concat_string(list_of_filenames[insert_index], "/");
        concat_string(list_of_filenames[insert_index], ffd.cFileName);

        /// @note: Convert backwards slash to forward slash
        int i = 0;
        size_t filename_length = string_length(list_of_filenames[insert_index]);
        while(i < filename_length) {
          if(list_of_filenames[insert_index][i] == '\\') {
            list_of_filenames[insert_index][i] = '/';
          }
          i++;
        }

        // list_of_filesizes[insert_index] = filesize.QuadPart;
        puts(list_of_filenames[insert_index]);
        insert_index++;
      }
    }
  } while(FindNextFile(find_file_handle, &ffd) != 0);
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
    if(has_substring(line, "DSE_SUITE(")
    || has_substring(line, "DSE_TEST(")) {
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

  for(int i = 1; i < argc; i+=2) {
    if(strings_are_equal(argv[i], "-s")) {
      copy_string(argv[i+1], suite_query);
    } else if(strings_are_equal(argv[i], "-t")) {
      copy_string(argv[i+1], test_query);
    }
  }

  printf("This system has %d processors\n", count_threads());

  puts("Searching test files...");
  list_files_from_dir("..");

  puts("Generating file...");
  FILE* generated_file = fopen("../build/generated.c", "w");

  fprintf(generated_file, "#include \"../dse_tester.h\"\n");

  for(int i = 0; i < max_filenames; i++) {
    if(list_of_filenames[i]) {
      fprintf(generated_file, "#include \"%s\"\n", list_of_filenames[i]);
      char* file_text = read_entire_file(list_of_filenames[i]);
      extract_name_of_test(file_text);
      /// @todo: A better approach is to allocate once and reuse.
      free(file_text);
    }
  }

  fprintf(generated_file, "\nint main() {");

  fprintf(generated_file, "\n\tcopy_string(\"%s\", dse_suite_query);", suite_query);
  fprintf(generated_file, "\n\tcopy_string(\"%s\", dse_test_query);\n", test_query);

  for(int i = 0; i < max_tests; i++) {
    if(test_infos[i] != NULL) {
      fprintf(generated_file, "\n\tdse_functions[dse_functions_index++] = %s;", test_infos[i]);
    }
  }

  /// @todo: Maybe use two for loops, one to create the threads and the other to run the threads.

  fprintf(generated_file,
    "\n\n\tdse_u64 tests_per_thread = 1;"
    "\n\tdse_u64 remaining_tests = 0;"
    "\n\tdse_u64 available_threads = count_threads() - 1;"
    "\n\tif(dse_functions_index >= available_threads) {"
    "\n\t\ttests_per_thread = dse_functions_index / available_threads;"
    "\n\t\tremaining_tests = dse_functions_index %% available_threads;"
    "\n\t}"
    "\n"
    "\n\tHANDLE* threads_array = calloc(sizeof(HANDLE), available_threads);"
    "\n\tfor(dse_u64 i = 0; i < available_threads; i++) {"
    "\n\t\tthreads_array[i] = calloc(sizeof(HANDLE), 1);"
    "\n\t\tThreadArgs args = {i * tests_per_thread, (i + 1) * tests_per_thread};"
    "\n\t\tthreads_array[i] = create_thread(my_thread_fn, (void*)&args);"
    "\n\t\tstart_thread(threads_array[i]);"
    "\n\t}"
  );


  fprintf(generated_file, "\n\twait_all_threads(threads_array, available_threads);");

  fprintf(generated_file, "\n\tprintf(\"Test / Thread %%lld, Remaining: %%lld, index: %%lld\\n\", tests_per_thread, remaining_tests, dse_functions_index);");
  // fprintf(generated_file, "\n\n\tfor(dse_u64 dse_i = 0; dse_i < dse_functions_index; dse_i++) {");
  // fprintf(generated_file, "\n\t\tdse_functions[dse_i]();");
  // fprintf(generated_file, "\n\t}");
// 
  fprintf(generated_file, "\n\n\tDSE_PRINT_RESULTS();");

  fprintf(generated_file, "\n\treturn 0;\n}");
  fclose(generated_file);

  puts("Executing compiler...");
  execute_shell_cmd("cl /nologo /diagnostics:caret /Wall /WX /W4 /wd4189 /wd4464 /wd5045 /wd4255 /wd4996 /wd4100 /wd4244 generated.c");
  execute_shell_cmd("generated.exe");
}