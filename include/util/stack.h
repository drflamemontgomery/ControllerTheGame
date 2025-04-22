/*
    Heap Stack Structure
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

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "heap/allocator.h"
#include "util/slice.h"

#ifndef DEFAULT_STACK_SIZE
#define DEFAULT_STACK_SIZE 8
#endif

#define Stack(T)                                                               \
  struct {                                                                     \
    Allocator *allocator;                                                      \
    size_t len;                                                                \
    Slice(T) data;                                                             \
  }

#define Stack_create(T, ALLOCATOR)                                             \
  {                                                                            \
      .allocator = ALLOCATOR,                                                  \
      .len = 0,                                                                \
      .data = allocSlice(T, ALLOCATOR, DEFAULT_STACK_SIZE),                    \
  }

#define Stack_push(SELF, ELEM)                                                 \
  {                                                                            \
    if ((SELF).len == (SELF).data.len) {                                       \
      (SELF).data.ptr = (typeof((SELF).data.ptr))remapBlock(                   \
          (SELF).allocator, (SELF).data.len * sizeof(*(SELF).data.ptr),        \
          (char *)(SELF).data.ptr, sizeof(*(SELF).data.ptr),                   \
          (SELF).data.len * 1.5f);                                             \
      (SELF).data.len = (SELF).data.len * 1.5f;                                \
    }                                                                          \
    (SELF).data.ptr[(SELF).len] = ELEM;                                        \
    (SELF).len += 1;                                                           \
  }

#define Stack_pop(SELF)                                                        \
  ({                                                                           \
    debugAssert((SELF).len > 0, #SELF ".len == 0");                            \
    (SELF).data.ptr[--(SELF).len];                                             \
  })

#define Stack_peek(SELF)                                                       \
  ((SELF).len == 0 ? NULL : &(SELF).data.ptr[(SELF).len - 1])

#define Stack_destroy(SELF)                                                    \
  {                                                                            \
    freeSlice((SELF).allocator, (SELF).data);                                  \
  }

#endif // STACK_H
