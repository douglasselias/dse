#include<stdio.h>
#include<stdbool.h>
#include<time.h>

#include "dse_tester.h"

#include<strsafe.h>

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

int char_index(char needle, const char* haystack) {
  for(int i = 0; i < strlen(haystack); i++) {
    if(haystack[i] == needle) {
      return i;
    }
  }

  return -1;
}

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
        TCHAR dir_buffer[MAX_PATH];
        StringCchCopy(dir_buffer, MAX_PATH, path);
        StringCchCat(dir_buffer, MAX_PATH, "\\");
        StringCchCat(dir_buffer, MAX_PATH, ffd.cFileName);
        list_files_from_dir(dir_buffer);
      }
    } else {
      LARGE_INTEGER filesize;
      filesize.LowPart = ffd.nFileSizeLow;
      filesize.HighPart = ffd.nFileSizeHigh;
      if(has_substring(ffd.cFileName, ".test.")) {
        // printf("<FILE>\t%s\\%s\t\t%lld bytes\n", path, ffd.cFileName, filesize.QuadPart);
        list_of_filenames[insert_index] = calloc(sizeof(char), (strlen(path) + strlen(ffd.cFileName) + 2)); // one slash between '..' and the file, and the null terminator.

        StringCchCat(list_of_filenames[insert_index], MAX_PATH, path);
        StringCchCat(list_of_filenames[insert_index], MAX_PATH, "/");
        StringCchCat(list_of_filenames[insert_index], MAX_PATH, ffd.cFileName);

        int i = 0;
        size_t filename_length = strlen(list_of_filenames[insert_index]);
        while(i < filename_length) {
          if(list_of_filenames[insert_index][i] == '\\') {
            list_of_filenames[insert_index][i] = '/';
          }
          i++;
        }

        list_of_filesizes[insert_index] = filesize.QuadPart;
        insert_index++;
      }
    }
  } while(FindNextFile(hFind, &ffd) != 0);
}

typedef struct {
  char* name;
  char* body;
} TestInfo;

TestInfo test_infos[100] = {0};
int test_info_index = 0;

void extract_name_of_test(char* text) {
  int i = 0;
  size_t text_length = strlen(text);
  while(i < text_length && text[i] != '@') {
    i++;
  }

  char* test_name = calloc(sizeof(char), 100);
  char* test_body = calloc(sizeof(char), 1000);

  if(text[i] == '@') {
    if(text[i+1] == 't') {
      char* current_offset = text + i + strlen("@test(");
      int index = char_index(')', current_offset);

      memcpy(test_name, current_offset, index);
      for(int j = 0; j < 100; j++) {
        if(test_name[j] == ' ') test_name[j] = '_';
      }
      // printf(">> TestName:(%s)\n", test_name);

      /// @todo: Not robust. A test might have other blocks inside.
      int index_end = char_index('}', current_offset);
      memcpy(test_body, current_offset + 1 + index, index_end);
      /// @note: Hack. For some reason when inserting on the file, the body has a extra newline on each line.
      for(int j = 0; j < 100; j++) {
        if(test_body[j] == '\n') {
          test_body[j] = ' ';
          // j++;
        }
      }

      test_infos[test_info_index].name = calloc(sizeof(char), 100);
      test_infos[test_info_index].body = calloc(sizeof(char), 1000);
      test_infos[test_info_index].name = strdup(test_name);
      test_infos[test_info_index].body = strdup(test_body);
      test_info_index++;
    } else if(text[i+1] == 's') {
      char* current_offset = text + i + strlen("@suite(");
      int index = char_index(')', current_offset);

      memcpy(test_name, current_offset, index);
      for(int j = 0; j < 100; j++) {
        if(test_name[j] == ' ') test_name[j] = '_';
      }
      // printf(">> SuiteName:(%s)\n", test_name);
      /// @todo: Not robust. A suite has other blocks inside.
      int index_end = char_index('}', current_offset);
      memcpy(test_body, current_offset + index, index_end);
      // test_infos[test_info_index++] = (TestInfo){test_name, test_body};
    }
  }

  // for(int index = 0; index < 100; index++) {
    // if(test_name[index] == ' ') test_name[index] = '_';
  // }
}

int main(int argc, char* argv[]) {
  // for(int i = 0; i < argc; i++) {
  //   printf("Arg %d -> %s\n", i, argv[i]);
  // }

  printf("This system has %d processors\n", count_threads());

  list_files_from_dir("..");

  /// @todo: The user should be able to specify the path
  FILE* generated_file = fopen("../generated.c", "w");

  fprintf(generated_file, "#include \"dse_tester.h\"\n");
  fprintf(generated_file, "#include \"code.c\"\n");

  for(int i = 0; i < 100; i++) {
    if(list_of_filenames[i]) {
      // fprintf(generated_file, "#include \"%s\"\n", list_of_filenames[i]);

      // printf("Extracting FILE contents: %s\n", list_of_filenames[i]);
      char* file_text = read_entire_file(list_of_filenames[i]);
      extract_name_of_test(file_text);
    }
  }

  for(int i = 0; i < 100; i++) {
    TestInfo test_info = test_infos[i];
    if(test_info.name != NULL) {
      printf("Inserting Test info: Name: %s\nBody: %s\n", test_infos[i].name, test_infos[i].body);
      fprintf(generated_file, "\nvoid %s()", test_info.name);
      fprintf(generated_file, "%s\n", test_info.body);
    }
  }

  fprintf(generated_file, "int main() {");

  // fprintf(generated_file, "\n\ttypedef void(*dse_test_function)();");
  // fprintf(generated_file, "\n\tdse_test_function dse_functions[100] = {0};");
  // fprintf(generated_file, "\n\tint dse_function_index = 0;");

  for(int i = 0; i < 100; i++) {
    TestInfo test_info = test_infos[i];
    if(test_info.name != NULL) {
      fprintf(generated_file, "\n\tdse_register(%s);", test_info.name);
    }
  }

  /// @todo: Hardcoded number of function pointers.
  /// @todo: Make it multithreaded.
  fprintf(generated_file, "\n\n\tfor(int i = 0; i < 100; i++) {");
  fprintf(generated_file, "\n\t\tif(dse_functions[i]) dse_functions[i]();");
  fprintf(generated_file, "\n\t}");

  fprintf(generated_file, "\n\n\tDSE_PRINT_RESULTS();");

  fprintf(generated_file, "\n\treturn 0;\n}");
  fclose(generated_file);

  /// @todo: User can specify the path of generated.c file.
  // system("cd ..");
  // chdir("..");
  SetCurrentDirectory("..");
  // system("dir");
  system("cl /nologo /diagnostics:caret /Wall /WX /W4 /wd5045 /wd4255 /wd4996 /wd4100 /wd4244 generated.c");
  system("generated.exe");

  // char* file_text = read_entire_file("../main.c");
  // const char* s = "\n";
  // char *token = strtok(file_text, s);
   
  // while(token != NULL) {
  //   const char* test_token = "DSE_START_TEST(";
  //   if(token[0] == 'D' && has_substring(token, test_token)) {
  //     printf("%s\n", token);
  //     char buffer[100];
  //     int index = strchr(token,'(') - token + 1;
  //     memcpy(buffer, token+index, strlen(token) - strlen(test_token) - strlen(");\n"));
  //     printf("%s\n", buffer);
  //   }
  //   token = strtok(NULL, s);
  // }
}