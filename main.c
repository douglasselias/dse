#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>
#include<string.h>

/// @todo: Remove strsafe header
#include<strsafe.h>
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
        list_of_filenames[insert_index] = calloc(sizeof(char), (string_length(path) + string_length(ffd.cFileName) + 2)); // one slash between '..' and the file, and the null terminator.

        StringCchCat(list_of_filenames[insert_index], MAX_PATH, path);
        StringCchCat(list_of_filenames[insert_index], MAX_PATH, "/");
        StringCchCat(list_of_filenames[insert_index], MAX_PATH, ffd.cFileName);

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
        insert_index++;
      }
    }
  } while(FindNextFile(hFind, &ffd) != 0);
}

typedef struct {
  char* name;
} TestInfo;

TestInfo test_infos[1000] = {0};
int test_info_index = 0;

void extract_name_of_test(char* text) {
  // int i = 0;

  // while(i < text_length && text[i] != 'D' && text[i+1] != 'S' && text[i+2] != 'E') {
  //   i++;
  // }
  // if(text[i+4] == 'S')

  // size_t text_length = string_length(text);

  // int position = strstr(text, "DSE_SUITE(") - text;

  char* test_name = calloc(sizeof(char), 100);
  char* line = strtok(text, "\n");

  while(line != NULL) {
    // printf(" %s\n", line);
    if(has_substring(line, "DSE_SUITE(")
    || has_substring(line, "DSE_TEST(")) {
      int parenthesis_index = char_index(line, '(');
      int coma_index = char_index(line, ',');

      test_infos[test_info_index].name = calloc(sizeof(char), 100);
      memcpy(test_infos[test_info_index].name, text + parenthesis_index, coma_index);
      test_info_index++;
    }

    line = strtok(NULL, "\n");
  }


  // if(text[i] == '@') {
  //   if(text[i+1] == 't') {
  //     char* current_offset = text + i + string_length("@test(");
  //     int index = char_index(current_offset, ')');

  //     memcpy(test_name, current_offset, index);

  //     // printf(">> TestName:(%s)\n", test_name);

  //     test_infos[test_info_index].name = calloc(sizeof(char), 100);
  //     test_infos[test_info_index].name = strdup(test_name);
  //     test_info_index++;
  //   } else if(text[i+1] == 's') {
  //     char* current_offset = text + i + string_length("@suite(");
  //     int index = char_index(current_offset, ')');

  //     memcpy(test_name, current_offset, index);
  //     for(int j = 0; j < 100; j++) {
  //       if(test_name[j] == ' ') test_name[j] = '_';
  //     }
  //     // printf(">> SuiteName:(%s)\n", test_name);
  //     /// @todo: Not robust. A suite has other blocks inside.
  //     int index_end = char_index(current_offset, '}');
  //     memcpy(test_body, current_offset + index, index_end);
  //     // test_infos[test_info_index++] = (TestInfo){test_name, test_body};
  //   }
  // }

  // // for(int index = 0; index < 100; index++) {
  //   // if(test_name[index] == ' ') test_name[index] = '_';
  // // }
}

int main(int argc, char* argv[]) {
  // for(int i = 0; i < argc; i++) {
  //   printf("Arg %d -> %s\n", i, argv[i]);
  // }

  printf("This system has %d processors\n", count_threads());

  list_files_from_dir("..");

  /// @todo: The user should be able to specify the path
  FILE* generated_file = fopen("../build/generated.c", "w");

  fprintf(generated_file, "#include \"dse_tester.h\"\n");
  // fprintf(generated_file, "#include \"code.c\"\n");

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
      printf("Inserting Test info: Name: %s\n", test_infos[i].name);
      // fprintf(generated_file, "\nvoid %s()", test_info.name);
    }
  }

  fprintf(generated_file, "int main() {");

  for(int i = 0; i < 100; i++) {
    TestInfo test_info = test_infos[i];
    if(test_info.name != NULL) {
      fprintf(generated_file, "\n\tdse_functions[dse_functions_index++] = %s;", test_info.name);
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
  // SetCurrentDirectory("..");
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