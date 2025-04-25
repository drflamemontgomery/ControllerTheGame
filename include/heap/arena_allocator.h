/*
    Allocation Type That Tracks Allocations
    Copyright (C) 2025  Ashton Warner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
