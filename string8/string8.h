#ifndef DSE_STRING8_H
#define DSE_STRING8_H

typedef struct {} String8;
/// String view struct??? {int start, int end, String8* data};

/// Should I return or modify in place?
String8 dse_to_uppercase(String8 s);
String8 dse_to_lowercase(String8 s);
String8 dse_to_pascal_case(String8 s);
String8 dse_to_snake_case(String8 s);
String8 dse_to_camel_case(String8 s);
String8 dse_to_kebab_case(String8 s);

void dse_string_copy(String8 s, char* ss);

// Better names!
int dse_char_code_at(String8 string, int index);
char dse_code_to_char(int n);

String8 dse_concat_strings(String8 a, String8 b); /// Should it be va_args?
String8 dse_append_char(String8 s, char c);

String8 dse_string_join(String8* array_of_strings, int count, char delim);
// String8 dse_string_join(String8* array_of_strings, int count, String8 delim); // ???

String8* dse_string_split(String8 s, char delim);
// String8* dse_string_split(String8 s, String* delim); // C does not support function overloading

bool dse_string_includes(String8 string, String8 substring); // has_substring

int dse_substring_start_index(String8 string, String8 substring);
int dse_substring_end_index(String8 string, String8 substring);

String8 dse_slice_string(String8 s, int start, int end); /// end should have a default value

String* dse_string_format(String8 format, String8 values); /// Needs to be va_args (similar to sprintf)

String8 dse_number_to_string(int n);

String8 dse_string_replace(String8 string, char delim); // Should I return or modify in place?
// String8 dse_string_replace(String8 string, String8 delim);

String8 dse_remove_chars(String8 string, char delim);
String8 dse_remove_strings(String8 string, String8 delim);

String8 dse_slugify(String8 string);

// #define slugify dse_slugify

String8 dse_trim(String8 string);

int dse_index_of(String8 string, char search); // , int fromIndex); /// char_index
int dse_last_index_of(String8 string, char search); // , int fromIndex);

bool dse_strings_are_equal(String8 a, String8 b);

#ifdef DSE_STRING8_IMPLEMENTATION

int sum(int a, int b) {
  return a + b;
}

#endif // DSE_TESTER_IMPLEMENTATION
#endif // DSE_STRING8_H

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - MIT License
Copyright (c) 2025 Douglas S. Elias
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/