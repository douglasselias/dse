#ifndef OS_H
#define OS_H

/// @todo: Must return a list of paths
/// Maybe I can use the variable already defined
// char* list_of_filenames[max_filenames] = {0};
void list_files_from_dir(const char* path);

int count_threads();
void create_thread();
void wait_all_threads();
void execute_shell_cmd(char* cmd);

#endif // OS_H