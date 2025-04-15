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

typedef struct Allocator {
  /** \brief Allocate n amount of objects of size N
   *
   */
  void *(*alloc)(struct Allocator *, size_t, size_t);
  void (*free)(struct Allocator *, void *);
  void *(*remap)(struct Allocator *, void *, size_t, size_t);
} Allocator;

/** \brief Standard C library allocator
 *
 * \warn Not recommended to use by itself
 */
extern Allocator std_allocator;

void *allocMem(Allocator *allocator, size_t elem_size, size_t num_of_elems);
void freeMem(Allocator *allocator, void *ptr);
void *remapMem(Allocator *allocator, void *ptr, size_t elem_size,
               size_t num_of_elems);

#endif // ALLOCATOR_H
