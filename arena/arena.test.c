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

  dse_destroy_arena(arena);
}

void test_pop_arena() {
  dse_u64 arena_size = 1;
  Arena* arena = dse_create_arena(arena_size);
  dse_u8 target_a = 10;
  dse_u8 target_b = 20;

  {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = target_a;
  }
  {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = target_b;
  }

  {  
    dse_pop_arena(&arena, 1);
    
    dse_u8 value = *(arena->data);
    assertion(value == target_b, "Value %d", value);
  }

  {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = target_a;
    
    dse_u8 value = *(arena->data);
    assertion(value == target_a, "Value %d", value);
  }

  {
    assertion(arena->capacity == 2, "Arena size %lld", arena->capacity);

    dse_pop_arena(&arena, 1);
    dse_pop_arena(&arena, 1);
    
    assertion(arena->capacity == 1, "Arena size %lld", arena->capacity);
  }

  {
    dse_pop_arena(&arena, 1);
    assertion(arena->used == 0, "Arena size %lld", arena->used);
  }
}

void test_freelist() {
  dse_u64 arena_size = 10;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = i * 10;
  }

  dse_u8 free_index = 4;
  dse_pop_from_index(arena, free_index);

  dse_u8 target = 77;
  {
    dse_u8* block = dse_push_arena(&arena, 1);
    *block = target;
  }

  for(dse_u8 i = 0; i < arena_size; i++) {
    if(i == free_index) {
      assertion(arena->data[i] == target, "Arena data: %d:%d", i, arena->data[i]);
    } else {
      assertion(arena->data[i] == i * 10, "Arena data: %d:%d", i, arena->data[i]);
    }
  }
}