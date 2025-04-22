/*
    Memory Allocation Handler
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

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>

#include "util/attr.h"
#include "util/slice.h"

typedef struct Allocator {
  /** \brief Allocate n amount of objects of size N
   *
   */
  void *(*alloc)(struct Allocator *, size_t, size_t);
  void (*free)(struct Allocator *, void *);
  void *(*remap)(struct Allocator *, size_t, char[], size_t, size_t);
} Allocator;

/** \brief Standard C library allocator
 *
 * \warn Not recommended to use by itself
 */
extern Allocator std_allocator;

void freePtr(Allocator *allocator, void *ptr);

void *allocPtr(Allocator *allocator, size_t elem_size, size_t num_of_elems);
void *remapBlock(Allocator *allocator, size_t ptr_size, char ptr[ptr_size],
                 size_t elem_size, size_t num_of_elems);

#define allocSlice(T, allocator, num_of_elems)                                 \
  Slice_create(allocPtr(allocator, sizeof(T), num_of_elems), num_of_elems)

#define freeSlice(allocator, slice) freePtr(allocator, (slice).ptr)
#define remapSlice(allocator, slice, num_of_elems)                  \
  Slice_create(typeof(*(slice).ptr),                                           \
               remapBlock(allocator, (slice).len * sizeof(*(slice).ptr),       \
                          (char *)(slice).ptr, elem_size, num_of_elems),       \
               num_of_elems)
#define remapPtr(allocator, ptr, elem_size, num_of_elems)                      \
  remapBlock(allocator, 0, ptr, elem_size, num_of_elems)

#endif // ALLOCATOR_H
