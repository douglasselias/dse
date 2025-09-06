#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#define null NULL

typedef  int64_t s64;
typedef  uint8_t  u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef double   f64;

#define Struct(name)        \
  typedef struct name name; \
  struct name               \

#define Enum(name)        \
  typedef enum name name; \
  enum name               \

#endif // BASE_TYPES_H