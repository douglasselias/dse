#define DSE_ARENA_IMPLEMENTATION
#include "arena.h"

void test_arena_push() {
  dse_u64 arena_size = 10;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = 10 * (i+1);

    assertion(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d", i, arena->data[i]);
  }

  assertion(arena->used == (dse_s64)arena_size);
}

void test_arena_push_chaining() {
  dse_u64 arena_size = 1;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = 10 * (i+1);
  }

  dse_u8* block = dse_push_arena(&arena, 1);
  *block = 77;
  assertion(arena->capacity == 2, "Cap %lld", arena->capacity);
  assertion(*(arena->data) == 77);
}

void test_set_position_back() {
  dse_u64 arena_size = 10;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = i + 1;
  }

  dse_u64 half_arena_size = arena_size / 2;
  arena->used -= half_arena_size;
  assertion(arena->used == (dse_s64)half_arena_size);

  for(dse_u8 i = 5; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = 10 * (i+1);
  }

  for(dse_u8 i = 0; i < arena_size; i++) {
    if(i < half_arena_size) {
      assertion(arena->data[i] == (i+1), "Arena data: %d:%d", i, arena->data[i]);
    } else {
      assertion(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d: Expected %d", i, arena->data[i], (10 * (i+1)));
    }
  }
}

void test_destroy_arena() {
  dse_u64 arena_size = 1;
  Arena* arena = dse_create_arena(arena_size);

  dse_push_arena(&arena, 1);
  // Chain
  dse_push_arena(&arena, 1);
  dse_push_arena(&arena, 1);
  // Chain
  dse_push_arena(&arena, 1);

  dse_destroy_arena(&arena);
}