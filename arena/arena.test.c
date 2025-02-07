#define DSE_ARENA_IMPLEMENTATION
#include "arena.h"

// void test_arena_creation() {
//   Arena* arena = dse_create_arena(8);
//   DSE_ASSERT(arena->capacity == 8, "Capacity is wrong size, got %lld", arena->capacity);
//   for(int i = 0; i < 8; i++) {
//     DSE_ASSERT(arena->data[i] == 0);
//   }
// }

void test_arena_push() {
  dse_u64 arena_size = 8;
  Arena* arena = dse_create_arena(arena_size);

  for(dse_u8 i = 0; i < arena_size; i++) {
    void* block = dse_push_arena(arena, 1);
    *(dse_u8*)block = 10 * (i+1);
  }

  for(dse_u8 i = 0; i < arena_size; i++) {
    DSE_ASSERT(arena->data[i] == (10 * (i+1)), "Arena data: %d:%d", i, arena->data[i]);
  }
}

// void test_arena_push_overflow() { // Old version of arena
//   Arena* arena = dse_create_arena(1);
//   void* block = dse_push_arena(arena, 2);
//   DSE_ASSERT(block == NULL);
// }

// void test_arena_push_overflow_chaining() { /// @todo: not working
//   Arena* arena = dse_create_arena(1);
//   void* block = dse_push_arena(arena, 2);
//   DSE_ASSERT(block != NULL);
//   DSE_ASSERT(arena->previous != NULL);
//   DSE_ASSERT(arena->next == NULL);
// }

// void test_arena_push_underflow_chaining() {
//   Arena* arena = dse_create_arena(1);
//   Arena* first_arena_address = arena;
//   void* block = dse_push_arena(arena, 3);
//   dse_pop_arena(arena, 2);
//   Arena* second_arena_address = arena;
//   DSE_ASSERT(first_arena_address == second_arena_address);
// }

// void test_arena_pop() { /// Old implementation of pop
//   Arena* arena = dse_create_arena(2);
//   void* block1 = dse_push_arena(arena, 1);
//   DSE_ASSERT(arena->used == 1, "Arena used %lld", arena->used);
//   void* block2 = dse_push_arena(arena, 1);
//   DSE_ASSERT(arena->used == 2, "Arena used %lld", arena->used);
//   dse_pop_arena(arena, 1);
//   DSE_ASSERT(arena->used == 1, "Arena used %lld", arena->used);
// }

// void test_position_back() {
//   Arena* arena = dse_create_arena(2);
//   void* block1 = dse_push_arena(arena, 1);
//   *(dse_u8*)block1 = 10;
//   void* block2 = dse_push_arena(arena, 1);

//   dse_u8* current_data = arena->data + ((arena->used-1) * sizeof(dse_u8));
//   DSE_ASSERT(*current_data == 0, "Arena data is not zero, is %d", *current_data);
//   dse_set_position_back(arena, 0);
//   DSE_ASSERT(*(arena->data) == 10, "Arena data is not ten");
//   *(arena->data) = 20;
//   DSE_ASSERT(*(arena->data) == 20, "Arena data is not twenty");
// }

// void test_arena_total_bytes() {
//   Arena* arena = dse_create_arena(8);
//   void* block1 = dse_push_arena(arena, 1);
//   void* block2 = dse_push_arena(arena, 1);
//   void* block3 = dse_push_arena(arena, 1);
//   DSE_ASSERT(dse_total_bytes_allocated(arena) == (3 * sizeof(dse_u8)));
// }