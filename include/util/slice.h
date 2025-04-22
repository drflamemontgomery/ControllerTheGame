/*
    Slice Arrays with runtime length information
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

#ifndef SLICE_H
#define SLICE_H

/** \brief Fat Pointer based on Zig slices
 *
 */
#define Slice(T)                                                               \
  struct {                                                                     \
    size_t len;                                                                \
    T *ptr;                                                                    \
  }

#define SliceElemType(SLICE) (typeof(*((SLICE).ptr)))
#define Slice_create(PTR, LEN) {.ptr = (PTR), .len = (LEN)}
#define Slice_sub(SLICE, START, END)                                           \
  ({                                                                           \
    debugAssert(START < (SLICE).len, "Slice_sub() start >= " #SLICE ".len");   \
    int end = END < 0 ? ((SLICE).len + END) : END;                             \
    debugAssert(END < (SLICE).len, "Slice_sub() end >= " #SLICE ".len");       \
    Slice(typeof(*(SLICE).ptr)) ret =                                          \
        Slice_create(&(SLICE).ptr[START], end - START + 1);                    \
    ret;                                                                       \
  })
#define forArray(SLICE, VAR_NAME)                                              \
  for (typeof((SLICE).ptr) VAR_NAME = ((SLICE).ptr);                           \
       VAR_NAME != ((SLICE).ptr) + ((SLICE).len); VAR_NAME++)

#define forIterArray(SLICE, ITER_NAME)                                         \
  for (size_t ITER_NAME = 0; ITER_NAME < (SLICE).len; ITER_NAME++)

#define Array(T)                                                               \
  struct {                                                                     \
    const size_t len;                                                          \
    T *ptr;                                                                    \
  }

#define Array_create(T, size)                                                  \
  {                                                                            \
      .len = size,                                                             \
      .ptr = __builtin_alloca(size * sizeof(T)),                               \
  }
#endif // SLICE_H
