#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#ifndef __cplusplus
  #define bool  _Bool
  #define false 0
  #define true  1
#endif

typedef   signed long long s64;
typedef unsigned char      u8;
typedef unsigned int       u32;
typedef unsigned long long u64;

#ifdef __cplusplus
  #define null 0
#else
  #define null ((void*)0)
#endif

#define Struct(name)        \
  typedef struct name name; \
  struct name               \

#endif // BASE_TYPES_H