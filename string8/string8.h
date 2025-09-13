#ifndef DSE_STRING8_H
#define DSE_STRING8_H

#include "../base_types.h"

// #define DSE_OS_IMPLEMENTATION
// #include "../os/os.h"
// #include <windows.h>
// #include <stdarg.h>
#include <stdio.h>

Struct(DSE_String8)
{
  char *text;
  u64  size;
};

#define STR8_FMT "%.*s"
#define STR8_ARG(s) (s32)(s).size, (s).text

u64 __dse_string_size(char *string);

#define DSE_STR8(string) (DSE_String8){ string, __dse_string_size(string) }

bool dse_strings_are_equal(DSE_String8 a, DSE_String8 b);

char dse_char_to_uppercase(char c);
char dse_char_to_lowercase(char c);

void dse_string_to_uppercase(DSE_String8 s);
void dse_string_to_lowercase(DSE_String8 s);

void dse_string_copy(char *source, DSE_String8 *destination);

void dse_append_string(DSE_String8 *a, DSE_String8 b);
void dse_append_char  (DSE_String8 *s, char c);

void dse_join_strings_with_char  (DSE_String8 *result, DSE_String8 *strings, u64 count, char    delim);
void dse_join_strings_with_string(DSE_String8 *result, DSE_String8 *strings, u64 count, DSE_String8 delim);

void dse_split_string_with_char  (DSE_String8 **result, DSE_String8 input, char    delim, u64 *strings_count);
void dse_split_string_with_string(DSE_String8 *result, DSE_String8 input, DSE_String8 delim, u64 *strings_count);

bool dse_string_includes(DSE_String8 haystack, DSE_String8 needle);

s64 dse_substring_start_index(DSE_String8 haystack, DSE_String8 needle);
s64 dse_substring_end_index  (DSE_String8 haystack, DSE_String8 needle);

DSE_String8 dse_slice_string(DSE_String8 string, u64 start, u64 end);

DSE_String8 dse_string_format(DSE_String8 format, ...);

void dse_string_printf(DSE_String8 format, ...);

void dse_int_to_string(DSE_String8 *result, s64 number);
s64 dse_string_to_int(DSE_String8 string);
// TODO: Add string to float and float to string.

DSE_String8 dse_string_replace_char(DSE_String8 input, char target, char replacement);
DSE_String8 dse_string_replace_string(DSE_String8 input, DSE_String8 target, DSE_String8 replacement);

void dse_remove_chars(DSE_String8 *result, DSE_String8 s, char delim);
DSE_String8 dse_remove_strings(DSE_String8 s, DSE_String8 delim);

void dse_slugify(DSE_String8 s);

DSE_String8 dse_trim_left (DSE_String8 s);
DSE_String8 dse_trim_right(DSE_String8 s);
DSE_String8 dse_trim      (DSE_String8 s);

bool dse_starts_with(DSE_String8 string, DSE_String8 target);
bool dse_ends_with  (DSE_String8 string, DSE_String8 target);

s64 dse_index_of(DSE_String8 string, char search);
s64 dse_index_of_from(DSE_String8 string, char search, u64 from_index);
s64 dse_last_index_of(DSE_String8 string, char search, u64 from_index);

bool dse_simple_fuzzy_match(DSE_String8 string, DSE_String8 pattern);

// TODO: Search functions that are able to use string view instead of allocating memory. 
DSE_String8 dse_string_view(DSE_String8 string, u64 start_index, u64 end_index);
DSE_String8 dse_string_view_size(DSE_String8 string, u64 start_index, u64 size);

char* dse_to_c_string(DSE_String8 s);
DSE_String8 dse_c_string_to_string8(char *s);

#ifdef DSE_STRING8_IMPLEMENTATION

u64 __dse_string_size(char *s)
{
  u64 size = 0;
  while(*s++ != '\0') size++;
  return size;
}

bool dse_strings_are_equal(DSE_String8 a, DSE_String8 b)
{
  if(a.size != b.size) return false;

  for(u64 i = 0; i < a.size; i++)
  {
    if(a.text[i] != b.text[i]) return false;
  }

  return true;
}

char dse_char_to_uppercase(char c)
{
  return c - 32;
}

char dse_char_to_lowercase(char c)
{
  return c + 32;
}

void dse_string_to_uppercase(DSE_String8 s)
{
  for(u64 i = 0; i < s.size; i++)
  {
    char c = s.text[i];

    // TODO: Create a function is char? or lower char? or alpha?
    if('a' <= c && c <= 'z')
    {
      s.text[i] = dse_char_to_uppercase(c);
    }
  }
}

void dse_string_to_lowercase(DSE_String8 s)
{
  for(u64 i = 0; i < s.size; i++)
  {
    char c = s.text[i];

    if('A' <= c && c <= 'Z')
    {
      s.text[i] = dse_char_to_lowercase(c);
    }
  }
}

void dse_string_copy(char *source, DSE_String8 *destination)
{
  u64 source_size = __dse_string_size(source);
  destination->size = source_size;

  for(u64 i = 0; i < source_size; i++)
  {
    destination->text[i] = source[i];
  }
}

void dse_append_string(DSE_String8 *a, DSE_String8 b)
{
  for(u64 i = 0; i < b.size; i++)
  {
    a->text[a->size + i] = b.text[i];
  }

  a->size += b.size;
}

void dse_append_char(DSE_String8 *s, char c)
{
  s->size++;
  for(u64 i = 0; i < s->size; i++)
  {
    if(s->text[i] == '\0')
    {
      s->text[i] = c;
      break;
    }
  }
}

void dse_join_strings_with_char(DSE_String8 *result, DSE_String8 *strings, u64 strings_count, char delim)
{
  u64 delim_count = strings_count - 1;
  u64 result_index = 0;

  for(u64 i = 0; i < strings_count; i++)
  {
    DSE_String8 s = strings[i];

    for(u64 j = 0; j < s.size; j++)
    {
      result->text[result_index++] = s.text[j];
    }

    if(delim_count > 0)
    {
      result->text[result_index++] = delim;
      delim_count--;
    }
  }
}

void dse_join_strings_with_string(DSE_String8 *result, DSE_String8 *strings, u64 strings_count, DSE_String8 delim)
{
  u64 delim_count = strings_count - 1;
  u64 result_index = 0;

  for(u64 i = 0; i < strings_count; i++)
  {
    DSE_String8 s = strings[i];

    for(u64 j = 0; j < s.size; j++)
    {
      result->text[result_index++] = s.text[j];
    }

    if(delim_count > 0)
    {
      for(u64 j = 0; j < delim.size; j++)
      {
        result->text[result_index++] = delim.text[j];
      }

      delim_count--;
    }
  }
}

void dse_split_string_with_char(DSE_String8 **result, DSE_String8 string, char delim, u64 *strings_count)
{
  *strings_count = 1;
  for(u64 i = 0; i < string.size; i++)
  {
    if(string.text[i] == delim) (*strings_count)++;
  }

  u64 result_index = 0;
  u64 start_index  = 0;

  for(u64 i = 0; i < string.size; i++)
  {
    if(string.text[i] == delim)
    {
      DSE_String8 s = dse_slice_string(string, start_index, i);
      result[result_index++] = &s;
      start_index = i + 1;
    }
  }

  DSE_String8 s = dse_slice_string(string, start_index, string.size);
  result[result_index] = &s;
}

void dse_split_string_with_string(DSE_String8 *result, DSE_String8 string, DSE_String8 delim, u64 *strings_count)
{
  *strings_count = 1;

  for(u64 i = 0; i < string.size; i++)
  {
    DSE_String8 sliced = dse_slice_string(string, i, string.size);
    s64 start_index = dse_substring_start_index(sliced, delim);

    if(start_index < 0) break;
    else
    {
      i += start_index + delim.size - 1;
      (*strings_count)++;
    }
  }

  u64 result_index = 0;
  s64 start_index  = 0;

  for(u64 i = 0; i < string.size; i++)
  {
    DSE_String8 sliced = dse_slice_string(string, i, string.size);
    start_index = dse_substring_start_index(sliced, delim);

    if(start_index > 0)
    {
      result[result_index++] = dse_slice_string(string, i, i + start_index);
      i += start_index + delim.size - 1;
    }
    else
    {
      start_index = i;
      break;
    }
  }

  result[result_index] = dse_slice_string(string, start_index, string.size);
}

bool dse_string_includes(DSE_String8 haystack, DSE_String8 needle)
{
  if(needle.size == 0)            return true;
  if(haystack.size == 0)          return false;
  if(haystack.size < needle.size) return false;

  u64 haystack_index = 0;
  u64 needle_index   = 0;

  while(haystack_index < haystack.size)
  {
    char a = dse_char_to_lowercase(haystack.text[haystack_index]);
    char b = dse_char_to_lowercase(needle.text[needle_index]);

    if(a == b)
    {
      needle_index++;

      if(needle_index < needle.size) haystack_index++;
      else return true;
    }
    else
    {
      needle_index = 0;
      haystack_index++;
    }
  }

  return false;
}

s64 dse_substring_start_index(DSE_String8 haystack, DSE_String8 needle)
{
  if(needle.size == 0)            return true;
  if(haystack.size == 0)          return false;
  if(haystack.size < needle.size) return false;

  u64 haystack_index = 0;
  u64 needle_index   = 0;

  while(haystack_index < haystack.size)
  {
    char a = haystack.text[haystack_index];
    char b = needle.text[needle_index];

    if(a == b)
    {
      needle_index++;

      if(needle_index < needle.size) haystack_index++;
      else return haystack_index - needle.size + 1;
    }
    else
    {
      needle_index = 0;
      haystack_index++;
    }
  }

  return -1;
}

s64 dse_substring_end_index(DSE_String8 haystack, DSE_String8 needle) {
  if(needle.size == 0)            return true;
  if(haystack.size == 0)          return false;
  if(haystack.size < needle.size) return false;

  u64 haystack_index = 0;
  u64 needle_index   = 0;

  while(haystack_index < haystack.size)
  {
    char a = haystack.text[haystack_index];
    char b = needle.text[needle_index];

    if(a == b)
    {
      needle_index++;

      if(needle_index < needle.size) haystack_index++;
      else return haystack_index + 1;
    }
    else
    {
      needle_index = 0;
      haystack_index++;
    }
  }

  return -1;
}

#include <assert.h>
// TODO: Duplicated code?
DSE_String8 dse_slice_string(DSE_String8 string, u64 start, u64 end)
{
  assert(!(start > end));
  DSE_String8 slice = {0};
  slice.size = end - start;
  slice.text = string.text + start;
  return slice;
}

#define DSE_MAX_CHARS 20
char* __dse_parse_number(char *buffer, u64 number)
{
  u8 buffer_index = DSE_MAX_CHARS;

  while(number != 0)
  {
    char rem = number % 10;
    number -= rem;
    number /= 10;

    buffer[buffer_index--] = rem + '0';
  }

  char *start_buffer = buffer + buffer_index + 1; // +1 to undo the last --
  return start_buffer;
}

void dse_int_to_string(DSE_String8 *result, s64 number)
{
  __dse_parse_number(result->text, number);
  result->size = __dse_string_size(result->text);
}

s64 dse_string_to_int(DSE_String8 string)
{
  s64 result = 0;

  for(u64 i = 0; i < string.size; i++)
  {
    char c = string.text[i];

    if('0' <= c && c <= '9')
    {
      result *= 10;
      result += c - '0';
    }
  }

  return string.text[0] == '-' ? -result : result;
}

void dse_slugify(DSE_String8 string)
{
  DSE_String8 s = dse_trim(string);
  dse_string_to_lowercase(s);

  for(u64 i = 0; i < s.size; i++)
  {
    char c = s.text[i];

    bool is_invalid_letter = !('a' <= c && c <= 'z');
    bool is_invalid_number = !('0' <= c && c <= '9');
    bool is_invalid_char   = is_invalid_letter && is_invalid_number;

    if(is_invalid_char) s.text[i] = '-';
    else s.text[i] = c;
  }
}

Enum(DSE_TRIM)
{
  DSE_TRIM_BOTH  = 1 << 0,
  DSE_TRIM_LEFT  = 1 << 1,
  DSE_TRIM_RIGHT = 1 << 2,
};

DSE_String8 dse_trim2(DSE_String8 string, DSE_TRIM trim_side)
{
  u64 start = 0;

  if(trim_side & (DSE_TRIM_BOTH | DSE_TRIM_LEFT))
  {
    for(; start < string.size; start++)
    {
      if(string.text[start] != ' ') break;
    }
  }

  u64 end = string.size;

  if(trim_side & (DSE_TRIM_BOTH | DSE_TRIM_RIGHT))
  {
    for(; end >= 0; end--)
    {
      if(string.text[end - 1] != ' ') break;
    }
  }

  return dse_slice_string(string, start, end);
}

DSE_String8 dse_trim_both(DSE_String8 string)
{
  return dse_trim2(string, DSE_TRIM_BOTH);
}

DSE_String8 dse_trim_left2(DSE_String8 string)
{
  return dse_trim2(string, DSE_TRIM_LEFT);
}

DSE_String8 dse_trim_right2(DSE_String8 string)
{
  return dse_trim2(string, DSE_TRIM_RIGHT);
}

DSE_String8 dse_trim_left(DSE_String8 string)
{
  u64 start = 0;
  u64 end = string.size;

  for(u64 i = 0; i < string.size; i++)
  {
    if(string.text[i] == ' ') start++;
    else break;
  }

  return dse_slice_string(string, start, end);
}

DSE_String8 dse_trim_right(DSE_String8 string)
{
  u64 start = 0;
  u64 end = string.size;

  for(u64 i = string.size - 1; i >= 0; i--)
  {
    if(string.text[i] == ' ') end--;
    else break;
  }

  return dse_slice_string(string, start, end);
}

DSE_String8 dse_trim(DSE_String8 string)
{
  DSE_String8 result = string;
  result = dse_trim_left(result);
  result = dse_trim_right(result);
  return result;
}

bool dse_starts_with(DSE_String8 string, DSE_String8 target)
{
  for(u64 i = 0; i < target.size; i++)
  {
    if(target.text[i] != string.text[i])
    {
      return false;
    }
  }

  return true;
}

bool dse_ends_with(DSE_String8 string, DSE_String8 target) 
{
  if(target.size > string.size) return false;

  u64 start = string.size - target.size;
  DSE_String8 s = dse_slice_string(string, start, start + target.size);
  bool result = dse_strings_are_equal(target, s);
  return result;
}

#ifdef USE_CRT
// TODO: Needs to handle both String8 and C strings.
void dse_cpf(char *format_string, ...)
{
  va_list args;

  va_start(args, format_string);

  for(u64 i = 0; format_string[i] != '\0'; i++)
  {
    char c = format_string[i];
    if(c == '%')
    {
      i++;
      c = format_string[i];
      char buffer[DSE_MAX_CHARS + 1] = {0};

      switch(c)
      {
        case 'b':
        {
          u8 boolean = va_arg(args, u8);
          printf(boolean ? "true" : "false");
        }
        break;
        case 'd':
        {
          u64 number = va_arg(args, u64);
          printf(__dse_parse_number(buffer, number));
        }
        break;
        case 'f':
        {
          f64 number = va_arg(args, f64);
          u64 integer_part = number;
          char* num = __dse_parse_number(buffer, integer_part);

          printf("%s.", num);

          f64 float_part = number - integer_part;

          // TODO: Maybe let the user define how many digits to display
          for(u8 k = 0; k < 4; k++)
          {
            float_part *= 10;
            u8 truncated = float_part;
            putchar(truncated + '0');
            float_part -= truncated;
          }
        }
        break;
        case 's':
        {
          DSE_String8 string = va_arg(args, DSE_String8);
          printf("%s", string.text);
        }
        break;
      }
    }
    else
    {
      putchar(c);
    }
  }

  va_end(args);
}

void dse_pf(DSE_String8 format_string, ...)
{
  va_list args;

  va_start(args, format_string);

  for(u64 i = 0; i < format_string.size; i++)
  {
    char c = format_string.text[i];
    if(c == '%')
    {
      i++;
      c = format_string.text[i];
      char buffer[DSE_MAX_CHARS + 1] = {0};

      switch(c)
      {
        case 'b':
        {
          u8 boolean = va_arg(args, u8);
          printf(boolean ? "true" : "false");
        }
        break;
        case 'd':
        {
          u64 number = va_arg(args, u64);
          printf(__dse_parse_number(buffer, number));
        }
        break;
        case 'f':
        {
          f64 number = va_arg(args, f64);
          u64 integer_part = number;
          char* num = __dse_parse_number(buffer, integer_part);

          printf("%s.", num);

          f64 float_part = number - integer_part;

          // TODO: Maybe let the user define how many digits to display
          for(u8 k = 0; k < 4; k++)
          {
            float_part *= 10;
            u8 truncated = float_part;
            putchar(truncated + '0');
            float_part -= truncated;
          }
        }
        break;
        case 's':
        {
          DSE_String8 string = va_arg(args, DSE_String8);
          printf("%s", string.text);
        }
        break;
      }
    }
    else
    {
      putchar(c);
    }
  }

  va_end(args);
}
#endif

DSE_String8 dse_string_replace_char(DSE_String8 input, char target, char replacement)
{
  for(u64 i = 0; i < input.size; i++)
  {
    if(input.text[i] == target)
    {
      input.text[i] = replacement;
    }
  }

  return input;
}

DSE_String8 dse_string_replace_string(DSE_String8 input, DSE_String8 target, DSE_String8 replacement)
{
  u64 strings_count = 0;
  DSE_String8 *parts = null;
  dse_split_string_with_string(parts, input, target, &strings_count);
  DSE_String8 result = {};
  dse_join_strings_with_string(&result, parts, strings_count, replacement);
  return result;
}

void dse_remove_chars(DSE_String8 *result, DSE_String8 string, char delim)
{
  u64 delim_size = 0;
  for(u64 i = 0; i < string.size; i++)
  {
    if(string.text[i] == delim) delim_size++;
  }

  u64 result_size = string.size - delim_size;
  result->size = result_size;

  u64 result_index = 0;
  for(u64 i = 0; i < string.size; i++)
  {
    if(string.text[i] != delim) result->text[result_index++] = string.text[i];
  }
}

DSE_String8 dse_remove_strings(DSE_String8 string, DSE_String8 delim)
{
  u64 strings_count = 0;
  DSE_String8 *strings = null;
  dse_split_string_with_string(strings, string, delim, &strings_count);

  DSE_String8 result = {0};

  // TODO:
  // for(u64 i = 0; i < strings_count; i++)
  // {
  //   dse_append_string(&result, strings[i]);
  // }

  return result;
}

s64 dse_index_of(DSE_String8 string, char search)
{
  for(u64 i = 0; i < string.size; i++)
  {
    if(string.text[i] == search) return i;
  }

  return -1;
}

s64 dse_index_of_from(DSE_String8 string, char search, u64 from_index)
{
  for(u64 i = from_index; i < string.size; i++)
  {
    if(string.text[i] == search) return i;
  }

  return -1;
}

s64 dse_last_index_of(DSE_String8 string, char search, u64 from_index)
{
  for(s64 i = string.size - 1 - from_index; i >= 0; i--)
  {
    if(string.text[i] == search) return i;
  }

  return -1;
}

bool dse_simple_fuzzy_match(DSE_String8 input, DSE_String8 pattern)
{
  u64 pattern_index = 0;
  for(u64 i = 0; i < input.size; i++)
  {
    char s = dse_char_to_lowercase(input.text[i]);
    char p = dse_char_to_lowercase(pattern.text[pattern_index]);

    if(s == p)
    {
      pattern_index++;
    }
  }

  return pattern_index == pattern.size;
}

DSE_String8 dse_string_view(DSE_String8 string, u64 start_index, u64 end_index)
{
  DSE_String8 result = {0};
  result.size = end_index   - start_index;
  result.text = string.text + start_index;
  return result;
}

DSE_String8 dse_string_view_size(DSE_String8 string, u64 start_index, u64 size)
{
  DSE_String8 result = {0};
  result.size = size;
  result.text = string.text + start_index;
  return result;
}

char* dse_to_c_string(DSE_String8 s)
{
  // TODO: How to deal with null terminator? It seems to work fine.
  return s.text;
}

// TODO: This is the string copy.
// DSE_String8 dse_c_string_to_string8(char *s, String)
// {
//   DSE_String8 result = {0};
//   result.size = __dse_string_size(s);
//   strcpy(result.text, s);
//   return result;
// }

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