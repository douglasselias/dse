#include<stdio.h>
#include<stdbool.h>
#include<time.h>

#define DSE_TESTER_IMPLEMENTATION
#include "dse_tester.h"

#include "code.c"

#include<strsafe.h>

// DSE_START_SUITE(sum_suite);
// puts("Setup();");

// {
//   int result = sum(1, 2);
//   DSE_ASSERT(result == 1, "Result is not 1. Got: %d", result);
// }

// {
//   int result = sum(2, 0);
//   DSE_ASSERT(result == 2, "Result is not 2. Got: %d", result);
// }

// puts("Teardown();");
// DSE_END_SUITE();

// DSE_START_SUITE(for_suite);
// puts("Setup For();");

// DSE_START_TEST_SUITE(sum_test_suite);
//   int result = sum(1, 2);
//   DSE_ASSERT(result == 1, "Result is not 1. Got: %d", result);
// DSE_END_TEST_SUITE();

// DSE_START_TEST_SUITE(multiplication_suite);
//   int result = sum(2, 0);
//   DSE_ASSERT(result == 2, "Result is not 2. Got: %d", result);
// DSE_END_TEST_SUITE();

// puts("Teardown For();");
// DSE_END_SUITE();

DSE_START_TEST(sum_test_outside);
  int result = sum(1, 3);
  DSE_ASSERT(result == 4, "Result is not four. Got: %d", result);
DSE_END_TEST();

DSE_START_TEST(sum_test_outside_2);
  int result = sum(1, 3);
  DSE_ASSERT(result == 4, "Result is not four. Got: %d", result);
DSE_END_TEST();

// void sum_test() {
//   int result = sum(1, 3);
//   DSE_ASSERT(result == 4, "Result is not four. Got: %d", result);
// }

// void sum_test_1() {
//   int result = sum(2, 3);
//   result = 3;
//   DSE_ASSERT(result == 5, "Result is not five. Got: %d", result);
// }

// void sum_test_2() {
//   int result = sum(4, 3);
//   result = 3;
//   DSE_SKIP(DSE_ASSERT(result == 7));
// }

// void for_loop_test() {
//   for(int i = 0; i < 3; i++) {
//     DSE_ASSERT((i & 1) == 1, "i is not odd. Got %d", i);
//   }
// }

char* list_of_filenames[100] = {0};
int list_of_filesizes[100] = {0};
int insert_index = 0;

void list_files_from_dir();

void list_files_from_dir(const char* path) {
  WIN32_FIND_DATA ffd;
  TCHAR szDir[MAX_PATH];

  StringCchCopy(szDir, MAX_PATH, path);
  StringCchCat(szDir, MAX_PATH, "\\*");

  HANDLE hFind = FindFirstFile(szDir, &ffd);

  if(INVALID_HANDLE_VALUE == hFind) {
    puts("<> invalid file...");
    return;
  }

  do {
    if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      if(ffd.cFileName[0] != '.') {
        // printf("<DIR>\t%s\n", ffd.cFileName);
        TCHAR dir_buffer[MAX_PATH];
        StringCchCopy(dir_buffer, MAX_PATH, path);
        StringCchCat(dir_buffer, MAX_PATH, "\\");
        StringCchCat(dir_buffer, MAX_PATH, ffd.cFileName);
        // printf("dir_buffer: %s\n", dir_buffer);
        list_files_from_dir(dir_buffer);
      }
    } else {
      LARGE_INTEGER filesize;
      filesize.LowPart = ffd.nFileSizeLow;
      filesize.HighPart = ffd.nFileSizeHigh;
      if(has_substring(ffd.cFileName, ".test.")) 
      {
        printf(">>>>> path: %s\n", path);
        printf("<FILE>\t%s\t%lld bytes\n", ffd.cFileName, filesize.QuadPart);
        list_of_filenames[insert_index] = calloc(sizeof(char), (strlen(path) + strlen(ffd.cFileName)));

        if(strchr(path, '\\') != NULL) {
          memcpy(list_of_filenames[insert_index], path+3, strlen(path)-strlen("..\\"));
          char* c;
          while((c = strchr(list_of_filenames[insert_index], '\\')) != NULL) {
            puts("one backslash");
            *c = '/';
          }
          StringCchCat(list_of_filenames[insert_index], MAX_PATH, "/");
        }

        // strcpy(list_of_filenames[insert_index], ffd.cFileName);
        StringCchCat(list_of_filenames[insert_index], MAX_PATH, ffd.cFileName);
        // StringCchCat(dir_buffer, MAX_PATH, "\\");

        list_of_filesizes[insert_index] = filesize.QuadPart;
        insert_index++;
      }
    }
  } while(FindNextFile(hFind, &ffd) != 0);
}

int main(int argc, char* argv[]) {
  // for(int i = 0; i < argc; i++) {
  //   printf("Arg %d -> %s\n", i, argv[i]);
  // }

  // dse_suite_query = argv[2];
  // dse_test_query = argv[4];

  // printf("Suite Query %s\n", dse_suite_query);
  // printf("Test Query %s\n", dse_test_query);

  // puts("Running tests...\n");
  // sum_suite();
  // for_suite();
  // sum_test_outside();
  // sum_test_1();
  // sum_test_2();
  // for_loop_test();
  // DSE_HIGHRES_TIMER(for_loop_test);
  // DSE_TIMER(sum_suite);

  // DSE_PRINT_RESULTS();

  list_files_from_dir("..");

  FILE* dse_generated_file = fopen("../dse_generated.c", "w");
  // if(dse_generated_file == NULL) puts("no generated file");

  fprintf(dse_generated_file, "#include \"dse_tester.h\"\n");

  for(int i = 0; i < 100; i++) {
    if(list_of_filenames[i]) {
      fprintf(dse_generated_file, "#include \"%s\"\n", list_of_filenames[i]);
      printf("FILE: %s\n", list_of_filenames[i]);
    }
  }
  fclose(dse_generated_file);

  char* file_text = read_entire_file();
  const char* s = "\n";
  char *token = strtok(file_text, s);
   
  while(token != NULL) {
    const char* test_token = "DSE_START_TEST(";
    if(token[0] == 'D' && has_substring(token, test_token)) {
      printf("%s\n", token);
      char buffer[100];
      int index = strchr(token,'(') - token + 1;
      memcpy(buffer, token+index, strlen(token) - strlen(test_token) - strlen(");\n"));
      printf("%s\n", buffer);
    }
    token = strtok(NULL, s);
  }

  // for(int i = 0; i < 1000; i++) {
  //   (*dse_functions[i])();
  // }
}