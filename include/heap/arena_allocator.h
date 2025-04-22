#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include <stdbool.h>

#include "heap/allocator.h"
#include "util/slice.h"

typedef struct ArenaAllocatorPtrData {
  bool allocated;
  void *ptr;
} ArenaAllocatorPtrData;

typedef struct ArenaAllocator {
  union {
    Allocator super;
    struct {
      void *(*alloc)(struct ArenaAllocator *, size_t, size_t);
      void (*free)(struct ArenaAllocator *, void *);
      void *(*remap)(struct ArenaAllocator *, size_t, char[], size_t, size_t);
    };
  };

  size_t m_first_free_allocation;
  Slice(ArenaAllocatorPtrData) m_allocations;
  Allocator *m_allocator;
} ArenaAllocator;

ArenaAllocator ArenaAllocator_create(Allocator *allocator);
Allocator *ArenaAllocator_getAllocator(ArenaAllocator *self);
void ArenaAllocator_destroy(ArenaAllocator *self);

#endif // ARENA_ALLOCATOR_H
