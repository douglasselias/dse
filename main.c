#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<string.h>

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include "dse_tester.h"

int count_threads() {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
}

void my_thread_fn() {}

void create_thread() {
  // LPSECURITY_ATTRIBUTES default_security_attr = NULL;
  // int default_stack_size = 0;
  // void* thread_args = 0;
  // CreateThread(default_security_attr, default_stack_size, my_thread_fn, thread_args, CREATE_SUSPENDED, &dwThreadIdArray[i]);

  // WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
}

void wait_all_threads() {}

int char_index(const char* haystack, char needle) {
  for(int i = 0; i < string_length(haystack); i++) {
    if(haystack[i] == needle) {
      return i;
    }
  }

  return -1;
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

char* list_of_filenames[100] = {0};
int list_of_filesizes[100] = {0};
int insert_index = 0;

void list_files_from_dir(const char* path);

void list_files_from_dir(const char* path) {
  char szDir[MAX_PATH];
  /// @todo: Remove strcpy, strcat
  strcpy(szDir, path);
  strcat(szDir, "\\*");

  WIN32_FIND_DATA ffd;
  HANDLE hFind = FindFirstFile(szDir, &ffd);

  if(INVALID_HANDLE_VALUE == hFind) {
    puts("Invalid file.");
    return;
  }

  do {
    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if(ffd.cFileName[0] != '.') {
        char dir_buffer[MAX_PATH];
        strcpy(dir_buffer, path);
        strcat(dir_buffer, "\\");
        strcat(dir_buffer, ffd.cFileName);
        // puts(dir_buffer);
        list_files_from_dir(dir_buffer);
      }
    } else {
      LARGE_INTEGER filesize;
      filesize.LowPart = ffd.nFileSizeLow;
      filesize.HighPart = ffd.nFileSizeHigh;
      if(has_substring(ffd.cFileName, ".test.")) {
        // printf("<FILE>\t%s\\%s\t\t%lld bytes\n", path, ffd.cFileName, filesize.QuadPart);
        list_of_filenames[insert_index] = calloc(sizeof(char), (string_length(path) + string_length(ffd.cFileName) + 2)); // one slash between '..' and the file, and the null terminator.

        strcat(list_of_filenames[insert_index], path);
        strcat(list_of_filenames[insert_index], "/");
        strcat(list_of_filenames[insert_index], ffd.cFileName);

        /// @note: Convert backwards slash to forward slash
        int i = 0;
        size_t filename_length = string_length(list_of_filenames[insert_index]);
        while(i < filename_length) {
          if(list_of_filenames[insert_index][i] == '\\') {
            list_of_filenames[insert_index][i] = '/';
          }
          i++;
        }

        list_of_filesizes[insert_index] = filesize.QuadPart;
        puts(list_of_filenames[insert_index]);
        insert_index++;
      }
    }
  } while(FindNextFile(hFind, &ffd) != 0);
}

typedef struct {
  char* name;
} TestInfo;
/// @todo: Hardcoded size.
TestInfo test_infos[1000] = {0};
int test_info_index = 0;

void extract_name_of_test(char* text) {
  char* test_name = calloc(sizeof(char), 100);
  /// @todo: Replace strtok
  char* line = strtok(text, "\n");

  while(line != NULL) {
    if(has_substring(line, "DSE_SUITE(")
    || has_substring(line, "DSE_TEST(")) {
      // printf("Line: %s | %s\n", line, has_substring(line, "DSE_TEST(") ? "true" : "false");
      // printf("Line: %s\n", line);
      // printf("Is SUITE: %s\n", has_substring(line, "DSE_SUITE(") ? "true" : "false");
      // printf("Is TEST: %s\n", has_substring(line, "DSE_TEST(") ? "true" : "false");
      // puts("--------------");
      int parenthesis_index = char_index(line, '(') + 1;
      int coma_index = char_index(line, ',');

      test_infos[test_info_index].name = calloc(sizeof(char), 100);
      memcpy(test_infos[test_info_index].name, line + parenthesis_index, coma_index - parenthesis_index);
      // printf("Test Name: %s\n", test_infos[test_info_index].name);
      test_info_index++;
    }

    line = strtok(NULL, "\n");
  }
}

int main(int argc, char* argv[]) {
  // for(int i = 0; i < argc; i++) {
  //   printf("Arg %d -> %s\n", i, argv[i]);
  // }

  printf("This system has %d processors\n", count_threads());

  puts("Listing files...");
  list_files_from_dir("..");

  puts("Generating file...");
  FILE* generated_file = fopen("../build/generated.c", "w");

  fprintf(generated_file, "#include \"../dse_tester.h\"\n");
  // fprintf(generated_file, "#include \"code.c\"\n");

  /// @todo: Hardcoded number of filenames.
  for(int i = 0; i < 100; i++) {
    if(list_of_filenames[i]) {
      fprintf(generated_file, "#include \"%s\"\n", list_of_filenames[i]);
      char* file_text = read_entire_file(list_of_filenames[i]);
      extract_name_of_test(file_text);
    }
  }

  fprintf(generated_file, "\nint main() {");

  /// @todo: Hardcoded number of function pointers.
  for(int i = 0; i < 1000; i++) {
    TestInfo test_info = test_infos[i];
    if(test_info.name != NULL) {
      fprintf(generated_file, "\n\tdse_functions[dse_functions_index++] = %s;", test_info.name);
    }
  }

  /// @todo: Hardcoded number of function pointers.
  /// @todo: Make it multithreaded.
  fprintf(generated_file, "\n\n\tfor(int dse_i = 0; dse_i < 1000; dse_i++) {");
  fprintf(generated_file, "\n\t\tif(dse_functions[dse_i]) dse_functions[dse_i]();");
  fprintf(generated_file, "\n\t}");

  fprintf(generated_file, "\n\n\tDSE_PRINT_RESULTS();");

  fprintf(generated_file, "\n\treturn 0;\n}");
  fclose(generated_file);

  puts("Executing compiler...");
  system("cl /nologo /diagnostics:caret /Wall /WX /W4 /wd4464 /wd5045 /wd4255 /wd4996 /wd4100 /wd4244 generated.c");
  system("generated.exe");
}