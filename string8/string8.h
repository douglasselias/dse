#ifndef DSE_STRING8_H
#define DSE_STRING8_H

#include<stdbool.h>
#include<stdint.h>

/// @todo: check all for null terminator bugs

typedef int64_t dse_s64;
typedef uint8_t dse_u8;
typedef uint64_t dse_u64;

typedef struct {
  char* text;
  dse_u64 size;
} String8;

dse_u64 __dse_size(char* s);

#define STR8(string) { string, __dse_size(string) }

bool dse_strings_are_equal(String8 a, String8 b);

char dse_char_to_uppercase(char c);
char dse_char_to_lowercase(char c);

void dse_to_uppercase(String8 s);
void dse_to_lowercase(String8 s);
void dse_to_pascal_case(String8 s);
void dse_to_camel_case(String8 s);
void dse_to_snake_case(String8 s);
void dse_to_kebab_case(String8 s);

void dse_string_copy(char* source, String8* destination);

char dse_ascii_code_to_char(dse_u8 number);
dse_u8 dse_ascii_code_at(String8 string, dse_u8 index);

String8* dse_concat_strings(String8 a, String8 b);
void dse_append_char(String8* s, char c);

String8* dse_string_join(String8** array_of_strings, dse_u64 count, char delim);
String8* dse_string_join_string(String8** array_of_strings, dse_u64 count, String8 delim);

String8** dse_string_split(String8 s, char delim);
String8** dse_string_split_string(String8 s, String8 delim);

bool dse_string_includes(String8 haystack, String8 needle);

dse_s64 dse_substring_start_index(String8 haystack, String8 needle);
dse_s64 dse_substring_end_index(String8 haystack, String8 needle);

String8* dse_slice_string(String8 string, dse_u64 start, dse_u64 end);

String8* dse_string_format(String8 format, ...);

void dse_string_printf(String8 format, ...);

String8* dse_int_to_string(dse_s64 number);
dse_s64 dse_string_to_int(String8 string);

// String8 dse_string_replace(String8 string, char delim); // Should I return or modify in place?
// String8 dse_string_replace(String8 string, String8 delim);

String8* dse_remove_chars(String8 string, char delim);
String8* dse_remove_strings(String8 string, String8 delim);

String8* dse_slugify(String8 string);

String8* dse_trim(String8 string);

dse_s64 dse_index_of(String8 string, char search, dse_u64 from_index);
dse_s64 dse_last_index_of(String8 string, char search, dse_u64 from_index);

bool simple_fuzzy_match(String8 string, String8 pattern);

#ifdef DSE_STRING8_IMPLEMENTATION

dse_u64 __dse_size(char* s) {
  dse_u64 size = 0;
  while(*s++ != '\0') size++;
  return size;
}

bool dse_strings_are_equal(String8 a, String8 b) {
  if(a.size != b.size) return false;
  for(dse_u64 i = 0; i < a.size; i++) {
    if(a.text[i] != b.text[i]) return false;
  }
  return true;
}

char dse_char_to_uppercase(char c) {
  return c - 32;
}

char dse_char_to_lowercase(char c) {
  return c + 32;
}

void dse_to_uppercase(String8 s) {
  for(dse_u64 i = 0; i < s.size; i++) {
    if('a' <= s.text[i] && s.text[i] <= 'z') {
      s.text[i] -= 32;
    }
  }
}

void dse_to_lowercase(String8 s) {
  for(dse_u64 i = 0; i < s.size; i++) {
    if('A' <= s.text[i] && s.text[i] <= 'Z') {
      s.text[i] += 32;
    }
  }
}

void dse_to_pascal_case(String8 s) {
  bool is_first_letter = true;
  for(dse_u64 i = 0; i < s.size; i++) {
    char c = s.text[i];
    if(c == ' ') {
      is_first_letter = true;
      continue;
    }
    if('a' <= c && c <= 'z' && is_first_letter) {
      s.text[i] -= 32;
      is_first_letter = false;
    }
  }
}

void dse_to_camel_case(String8 s) {}

void dse_to_snake_case(String8 s) {
  for(dse_u64 i = 0; i < s.size; i++) {
    if('A' <= s.text[i] && s.text[i] <= 'Z') {
      s.text[i] += 32;
    } else if(s.text[i] == ' ') {
      s.text[i] = '_';
    }
  }
}

void dse_to_kebab_case(String8 s) {
  for(dse_u64 i = 0; i < s.size; i++) {
    if('A' <= s.text[i] && s.text[i] <= 'Z') {
      s.text[i] += 32;
    } else if(s.text[i] == ' ') {
      s.text[i] = '-';
    }
  }
}

void dse_string_copy(char* source, String8* destination) {
  dse_u64 source_size = __dse_size(source);
  destination->size = source_size;
  destination->text = calloc(sizeof(char), source_size);

  for(dse_u64 i = 0; i < source_size; i++) {
    destination->text[i] = source[i];
  }
}

char dse_ascii_code_to_char(dse_u8 number) {
  return (char)number;
}

dse_u8 dse_ascii_code_at(String8 string, dse_u8 index) {
  return (dse_u8)string.text[index];
}

String8* dse_concat_strings(String8 a, String8 b) {
  dse_u64 total_size = a.size + b.size;
  String8* result = calloc(sizeof(String8), 1);
  result->text = calloc(sizeof(char), total_size + 1); // +1 for null terminator
  result->size = total_size;
  for(dse_u64 i = 0; i < total_size; i++) {
    if(i < a.size)
      result->text[i] = a.text[i];
    else if(a.size <= i && (i - a.size) < b.size) {
      result->text[i] = b.text[i - a.size];
    }
  }
  result->text[total_size] = '\0';
  return result;
}

void dse_append_char(String8* s, char c) {
  s->size++;
  char* buffer = calloc(sizeof(char), s->size);
  for(dse_u64 i = 0; i < s->size; i++) {
    buffer[i] = s->text[i];
  }
  buffer[s->size-1] = c;
  s->text = buffer;
}

String8* dse_string_join(String8** array_of_strings, dse_u64 count, char delim) {
  dse_u64 total_size = 0;
  for(dse_u64 i = 0; i < count; i++) {
    String8* s = array_of_strings[i];
    total_size += s->size + 1;
  }
  total_size--;
  String8* result = calloc(sizeof(String8), 1);
  result->text = calloc(sizeof(char), total_size);
  result->size = total_size;

  dse_u64 global_index = 0;
  dse_u64 delim_count = count - 1;
  for(dse_u64 i = 0; i < count; i++) {
    String8* s = array_of_strings[i];
    for(dse_u64 j = 0; j < s->size; j++) {
      result->text[global_index++] = s->text[j];
    }

    if(delim_count > 0) {
      result->text[global_index++] = delim;
      delim_count--;
    }
  }

  return result;
}


String8* dse_string_join_string(String8** array_of_strings, dse_u64 count, String8 delim) {
  dse_u64 total_size = 0;
  for(dse_u64 i = 0; i < count; i++) {
    String8* s = array_of_strings[i];
    total_size += s->size;
  }
  total_size += delim.size * (count - 1);
  String8* result = calloc(sizeof(String8), 1);
  result->text = calloc(sizeof(char), total_size);
  result->size = total_size;

  dse_u64 global_index = 0;
  dse_u64 delim_count = count - 1;
  for(dse_u64 i = 0; i < count; i++) {
    String8* s = array_of_strings[i];
    for(dse_u64 j = 0; j < s->size; j++) {
      result->text[global_index++] = s->text[j];
    }

    if(delim_count > 0) {
      for(dse_u64 j = 0; j < delim.size; j++) {
        result->text[global_index++] = delim.text[j];
      }
      delim_count--;
    }
  }

  return result;
}

String8** dse_string_split(String8 string, char delim) {
  dse_u64 result_size = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    if(string.text[i] == delim) result_size++;
  }
  String8** result = calloc(sizeof(String8*), result_size + 1);
  dse_u64 result_index = 0;
  dse_u64 start_index = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    if(string.text[i] == delim) {
      result[result_index++] = dse_slice_string(string, start_index, i);
      start_index = i + 1;
    }
  }

  result[result_index] = dse_slice_string(string, start_index, string.size);

  return result;
}

String8** dse_string_split_string(String8 string, String8 delim) {
  dse_u64 result_size = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    String8* sliced = dse_slice_string(string, i, string.size);
    dse_s64 start_index = dse_substring_start_index(*sliced, delim);
    if(start_index < 0) break;
    else {
      i += start_index + delim.size - 1;
      result_size++;
    }
  }

  dse_u64 final_result_size = result_size + 1;
  String8** result = calloc(sizeof(String8*), final_result_size);

  dse_u64 result_index = 0;
  dse_s64 start_index  = 0;

  for(dse_u64 i = 0; i < string.size; i++) {
    String8* sliced = dse_slice_string(string, i, string.size);
    start_index = dse_substring_start_index(*sliced, delim);

    if(start_index > 0) {
      result[result_index++] = dse_slice_string(string, i, i+start_index);
      i += start_index + delim.size - 1;
    } else {
      start_index = i;
      break;
    }
  }

  result[result_index] = dse_slice_string(string, start_index, string.size);

  return result;
}

bool dse_string_includes(String8 haystack, String8 needle) {
  if(needle.size == 0) return true;
  if(haystack.size < needle.size) return false;

  dse_u64 haystack_index = 0;
  dse_u64 needle_index   = 0;

  while(haystack_index < haystack.size) {
    char a = dse_char_to_lowercase(haystack.text[haystack_index]);
    char b = dse_char_to_lowercase(needle.text[needle_index]);
    if(a == b) {
      needle_index++;

      if(needle_index < needle.size) haystack_index++;
      else return true;
    } else {
      needle_index = 0;
      haystack_index++;
    }
  }

  return false;
}

dse_s64 dse_substring_start_index(String8 haystack, String8 needle) {
  if(needle.size == 0) return true;
  if(haystack.size < needle.size) return false;

  dse_u64 haystack_index = 0;
  dse_u64 needle_index   = 0;

  while(haystack_index < haystack.size) {
    char a = dse_char_to_lowercase(haystack.text[haystack_index]);
    char b = dse_char_to_lowercase(needle.text[needle_index]);
    if(a == b) {
      needle_index++;

      if(needle_index < needle.size) haystack_index++;
      else {
        return haystack_index - needle.size + 1;
      }
    } else {
      needle_index = 0;
      haystack_index++;
    }
  }

  return -1;
}

dse_s64 dse_substring_end_index(String8 haystack, String8 needle) {
  if(needle.size == 0) return true;
  if(haystack.size < needle.size) return false;

  dse_u64 haystack_index = 0;
  dse_u64 needle_index   = 0;

  while(haystack_index < haystack.size) {
    /// @todo: Should this be case insensitive???
    char a = dse_char_to_lowercase(haystack.text[haystack_index]);
    char b = dse_char_to_lowercase(needle.text[needle_index]);
    if(a == b) {
      needle_index++;

      if(needle_index < needle.size) haystack_index++;
      else {
        return haystack_index + 1;
      }
    } else {
      needle_index = 0;
      haystack_index++;
    }
  }

  return -1;
}

String8* dse_slice_string(String8 string, dse_u64 start, dse_u64 end) {
  String8* slice = calloc(sizeof(String8), 1);
  slice->size = end - start;
  slice->text = calloc(sizeof(char), slice->size + 1);

  dse_u64 slice_index = 0;
  for(dse_u64 i = start; i < end; i++) {
    slice->text[slice_index++] = string.text[i];
  }
  slice->text[slice->size] = '\0';

  return slice;
}

String8* dse_string_format(String8 format, ...) {
  va_list vargs;
  va_start(vargs, format);

  String8** strings = dse_string_split_string(format, (String8)STR8("%s"));
  dse_u64 strings_total = 0;

  while(*format.text) {
    if(*format.text == '%' && *(format.text+1) == 's') {
      strings_total++;
      format.text++;
    }
    format.text++;
  }

  String8* result = calloc(sizeof(String8), 1);

  for(dse_u64 i = 0; i < strings_total; i++) {
    String8 s = *(strings[i]);
    String8 arg_string = va_arg(vargs, String8);

    result = dse_concat_strings(*result, s);
    result = dse_concat_strings(*result, arg_string);
  }

  result = dse_concat_strings(*result, *(strings[strings_total]));

  va_end(vargs);
  return result;
}

void dse_string_printf(String8 format, ...) {
  va_list vargs;
  va_start(vargs, format);

  while(*format.text) {
    if(*format.text == '%' && *(format.text+1) == 's') {
      String8 string = va_arg(vargs, String8);
      printf("%s", string.text);
      format.text++;
    } else putchar(*format.text);
    format.text++;
  }

  va_end(vargs);
}

String8* dse_int_to_string(dse_s64 number) {
  String8* result = calloc(sizeof(String8), 1);
  result->text = calloc(sizeof(char), 21);
  sprintf(result->text, "%lld", number);
  result->size = __dse_size(result->text);
  return result;
}

dse_s64 dse_string_to_int(String8 string) {
  dse_s64 result = 0;
  dse_u64 decimal_place = 1;

  for(dse_s64 i = string.size - 1; i >= 0; i--) {
    dse_u8 n = string.text[i] - 48;

    if(i != (dse_s64)string.size - 1) {
      decimal_place *= 10;
    }

    result += n * decimal_place;
  }

  return result;
}

String8* dse_slugify(String8 string) {
  String8* trimmed_string = dse_trim(string);
  dse_to_lowercase(*trimmed_string);

  for(dse_u64 i = 0; i < trimmed_string->size; i++) {
    char c = trimmed_string->text[i];
    bool is_invalid_letter = !('a' <= c && c <= 'z');
    bool is_invalid_number = !('0' <= c && c <= '9');
    bool is_invalid_char = is_invalid_letter && is_invalid_number;
    if(is_invalid_char) trimmed_string->text[i] = '-';
  }

  return trimmed_string;
}

String8* dse_trim(String8 string) {
  dse_u64 start = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    if(string.text[i] == ' ') start++;
    else break;
  }

  dse_u64 end = string.size;
  for(dse_s64 i = (dse_s64)string.size - 1; i >= 0; i--) {
    if(string.text[i] == ' ') end--;
    else break;
  }
  
 return dse_slice_string(string, start, end);
}

String8* dse_remove_chars(String8 string, char delim) {
  String8* result = calloc(sizeof(String8), 1);

  dse_u64 delim_size = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    if(string.text[i] == delim) delim_size++;
  }

  dse_u64 result_size = string.size - delim_size;
  result->text = calloc(sizeof(char), result_size);
  result->size = result_size;

  dse_u64 result_index = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    if(string.text[i] != delim) result->text[result_index++] = string.text[i];
  }

  return result;
}

String8* dse_remove_strings(String8 string, String8 delim) {
  String8** strings = dse_string_split_string(string, delim);
  dse_u64 strings_total = 0;

  dse_u64 start_index = 0;
  for(dse_u64 i = 0; i < string.size; i++) {
    String8* sliced = dse_slice_string(string, i, string.size);
    start_index = dse_substring_start_index(*sliced, delim);
    if(start_index > 0) {
      i += start_index + delim.size;
      strings_total++;
    } else {
      /// @todo: test with uneven input
      strings_total++;
      break;
    }
  }

  String8* result = calloc(sizeof(String8), 1);

  for(dse_u64 i = 0; i < strings_total; i++) {
    String8 s = *(strings[i]);
    result = dse_concat_strings(*result, s);
  }

  return result;
}

dse_s64 dse_index_of(String8 string, char search, dse_u64 from_index) {
  for(dse_u64 i = from_index; i < string.size; i++) {
    if(string.text[i] == search) return i;
  }

  return -1;
}

dse_s64 dse_last_index_of(String8 string, char search, dse_u64 from_index) {
  for(dse_s64 i = string.size - 1 - from_index; i >= 0; i--) {
    if(string.text[i] == search) return i;
  }

  return -1;
}

bool simple_fuzzy_match(String8 string, String8 pattern) {
  while(*string.text != '\0' && *pattern.text != '\0')  {
    if(*string.text + 32 == *pattern.text + 32)
      pattern.text++;
    string.text++;
  }

  return *pattern.text == '\0' ? true : false;
}

#endif // DSE_STRING8_IMPLEMENTATION
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