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

#include "heap/arena_allocator.h"
#include "debug/debug.h"
#include "heap/allocator.h"

static void ArenaAllocator_free(ArenaAllocator *self, void *ptr) {
  debugAssert(self != NULL, "self == NULL");
  debugAssert(ptr != NULL, "ptr == NULL");

  size_t i = 0;
  forArray(self->m_allocations, allocation) {
    if (!allocation->allocated)
      goto ArenaAllocator_free_forArray_continue;
    if (allocation->ptr != ptr)
      goto ArenaAllocator_free_forArray_continue;
    freePtr(self->m_allocator, ptr);
    allocation->ptr = NULL;
    allocation->allocated = false;
    if (i < self->m_first_free_allocation)
      self->m_first_free_allocation = i;

    return;

  ArenaAllocator_free_forArray_continue:
    i++;
  }
  errtrace("ptr 0x%lX is not managed by this instance of ArenaAllocator",
           (size_t)ptr);
  abort();
}

static void *ArenaAllocator_alloc(ArenaAllocator *self, size_t elem_size,
                                  size_t num_of_elems) {
  size_t i;
  for (i = self->m_first_free_allocation; i < self->m_allocations.len; i++) {
    if (!self->m_allocations.ptr[i].allocated)
      break;
  }
  if (i >= self->m_allocations.len) {
    size_t new_size = self->m_allocations.len * 1.5;
    void *new_ptr = remapBlock(self->m_allocator, self->m_allocations.len,
                               (char *)self->m_allocations.ptr,
                               sizeof(*self->m_allocations.ptr), new_size);
    debugAssert(new_ptr != NULL, "newPtr == NULL. Allocator Ran Out of Memory");
    self->m_allocations.len = new_size;
    self->m_allocations.ptr = new_ptr;
  }

  self->m_first_free_allocation = i + 1;
  self->m_allocations.ptr[i].ptr =
      allocBlock(self->m_allocator, elem_size, num_of_elems);
  self->m_allocations.ptr[i].allocated = true;
  return self->m_allocations.ptr[i].ptr;
}

static void *ArenaAllocator_remap(ArenaAllocator *self, size_t ptr_size,
                                  char ptr[ptr_size], size_t elem_size,
                                  size_t num_of_elems) {
  debugAssert(self != NULL, "self == NULL");
  debugAssert(ptr != NULL, "ptr == NULL");
  forArray(self->m_allocations, allocation) {
    if (!allocation->allocated)
      continue;
    if (allocation->ptr != ptr)
      continue;
    void *new_ptr =
        remapBlock(self->m_allocator, ptr_size, ptr, elem_size, num_of_elems);
    debugAssert(new_ptr != NULL,
                "new_ptr == NULL; Allocator Ran Out Of Memory");

    allocation->ptr = new_ptr;
    return new_ptr;
  };
  errtrace("ptr is not managed by this instance of ArenaAllocator");
  abort();
}

ArenaAllocator ArenaAllocator_create(Allocator *allocator) {
  return (ArenaAllocator){
      .m_first_free_allocation = 0,
      .m_allocator = allocator,
      .m_allocations = allocSlice(ArenaAllocatorPtrData, allocator, 8),
      .alloc = ArenaAllocator_alloc,
      .free = ArenaAllocator_free,
      .remap = ArenaAllocator_remap,
  };
}

__attribute__((const)) Allocator *
ArenaAllocator_getAllocator(ArenaAllocator *self) {
  debugAssert(self != NULL, "self == NULL");
  return &self->super;
}

void ArenaAllocator_destroy(ArenaAllocator *self) {
  debugAssert(self != NULL, "self == NULL");
  forArray(self->m_allocations, allocation) {
    if (!allocation->allocated)
      continue;
    freePtr(self->m_allocator, allocation->ptr);
    allocation->allocated = false;
    allocation->ptr = NULL;
  }
  freeSlice(self->m_allocator, self->m_allocations);

  self->m_allocations.len = 0;
  self->m_allocations.ptr = NULL;

  self->m_first_free_allocation = 0;

  self->alloc = NULL;
  self->free = NULL;
  self->remap = NULL;
}
