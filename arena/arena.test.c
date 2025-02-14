#define DSE_ARENA_IMPLEMENTATION
#include "arena.h"

void test_arena_push() {
  dse_u64 arena_size = 10;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(arena, 1);
    *block = 10 * (i+1);

    assertion(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d", i, arena->data[i]);
  }

  assertion(arena->used == (dse_s64)arena_size);
}

/// @todo: push overflow
// void test_arena_push() {
//   dse_u64 arena_size = 10;
//   Arena* arena = dse_create_arena(arena_size);

//   for(dse_u8 i = 0; i < arena_size; i++) {
//     dse_u8* block = dse_push_arena(arena, 1);
//     *block = 10 * (i+1);

//     assertion(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d", i, arena->data[i]);
//   }

//   assertion(arena->used == (dse_s64)arena_size);
// }


void test_set_position_back() {
  dse_u64 arena_size = 10;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(arena, 1);
    *block = 10 * (i+1);
  }

  dse_u64 half_arena_size = arena_size / 2;
  dse_set_position_back(arena, half_arena_size);
  assertion(arena->used == (dse_s64)half_arena_size);

  for(dse_u8 i = 0; i < half_arena_size; i++) {
    dse_u8* block = dse_push_arena(arena, 1);
    *block = 20 * (i+1);
  }

  for(dse_u8 i = 0; i < arena_size; i++) {
    if(i < half_arena_size) {
      assertion(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d", i, arena->data[i]);
    } else {
      assertion(arena->data[i] == (20 * (i+1)), "Arena data: %d:%d: Expected %d", i, arena->data[i], (20 * (i+1)));
    }
  }
}

void test_arena_push_chaining() {
  dse_u64 arena_size = 8;
  Arena* arena = dse_create_arena(arena_size);
  // printf("Sizeof u8 %zd", sizeof(dse_u8));

  for(dse_u8 i = 0; i < arena_size; i++) {
    dse_u8* block = dse_push_arena(arena, 1);
    *block = 10 * (i+1);
  }
}


// void test_arena_push_overflow_chaining() { /// @todo: not working
//   Arena* arena = dse_create_arena(1);
//   void* block = dse_push_arena(arena, 2);
//   assertion(block != NULL);
//   assertion(arena->previous != NULL);
//   assertion(arena->next == NULL);
// }

// void test_arena_push_underflow_chaining() {
//   Arena* arena = dse_create_arena(1);
//   Arena* first_arena_address = arena;
//   void* block = dse_push_arena(arena, 3);
//   dse_pop_arena(arena, 2);
//   Arena* second_arena_address = arena;
//   assertion(first_arena_address == second_arena_address);
// }

// void test_arena_pop() { /// Old implementation of pop
//   Arena* arena = dse_create_arena(2);
//   void* block1 = dse_push_arena(arena, 1);
//   assertion(arena->used == 1, "Arena used %lld", arena->used);
//   void* block2 = dse_push_arena(arena, 1);
//   assertion(arena->used == 2, "Arena used %lld", arena->used);
//   dse_pop_arena(arena, 1);
//   assertion(arena->used == 1, "Arena used %lld", arena->used);
// }

// void test_position_back() {
//   Arena* arena = dse_create_arena(2);
//   void* block1 = dse_push_arena(arena, 1);
//   *(dse_u8*)block1 = 10;
//   void* block2 = dse_push_arena(arena, 1);

//   dse_u8* current_data = arena->data + ((arena->used-1) * sizeof(dse_u8));
//   assertion(*current_data == 0, "Arena data is not zero, is %d", *current_data);
//   dse_set_position_back(arena, 0);
//   assertion(*(arena->data) == 10, "Arena data is not ten");
//   *(arena->data) = 20;
//   assertion(*(arena->data) == 20, "Arena data is not twenty");
// }

// void test_arena_total_bytes() {
//   Arena* arena = dse_create_arena(8);
//   void* block1 = dse_push_arena(arena, 1);
//   void* block2 = dse_push_arena(arena, 1);
//   void* block3 = dse_push_arena(arena, 1);
//   assertion(dse_total_bytes_allocated(arena) == (3 * sizeof(dse_u8)));
// }