#include "heap/allocator.h"
#include "debug/debug.h"
#include <stdbool.h>

void freePtr(Allocator *allocator, void *ptr) {
  debugAssert(allocator != NULL, "allocator == NULL");
  debugAssert(allocator->free != NULL, "allocator->free == NULL");
  debugAssert(ptr != NULL, "ptr == NULL");

  allocator->free(allocator, ptr);
}
void *allocPtr(Allocator *allocator, size_t elem_size, size_t num_of_elems) {
  debugAssert(allocator != NULL, "allocator == NULL");
  debugAssert(allocator->alloc != NULL, "allocator->alloc == NULL");
  debugAssert(elem_size > 0, "elem_size <= 0");
  debugAssert(num_of_elems > 0, "num_of_elems <= 0");

  return allocator->alloc(allocator, elem_size, num_of_elems);
}
void *remapBlock(Allocator *allocator, size_t ptr_size, char ptr[ptr_size],
                 size_t elem_size, size_t num_of_elems) {
  debugAssert(allocator != NULL, "allocator == NULL");
  debugAssert(allocator->alloc != NULL, "allocator->alloc == NULL");
  debugAssert(ptr != NULL, "ptr == NULL");
  debugAssert(elem_size > 0, "elem_size <= 0");
  debugAssert(num_of_elems > 0, "num_of_elems <= 0");

  return allocator->remap(allocator, ptr_size, ptr, elem_size, num_of_elems);
}

static void *std_alloc(Allocator *, size_t elem_size, size_t num_of_elems) {
  void *ptr = calloc(num_of_elems, elem_size);
  return ptr;
}
static void std_free(Allocator *, void *ptr) { free(ptr); }
static void *std_remap(Allocator *, size_t ptr_size, char ptr[ptr_size],
                       size_t elem_size, size_t num_of_elems) {
  void *ret_ptr = realloc(ptr, elem_size * num_of_elems);
  return ret_ptr;
}

Allocator std_allocator = {
    .alloc = std_alloc,
    .free = std_free,
    .remap = std_remap,
};
