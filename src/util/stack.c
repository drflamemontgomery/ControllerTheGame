/*
    Heap Stack Implementation
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

#include "util/stack.h"
#include <memory.h>

Stack Stack_create(size_t element_size) {
  return (Stack){
      .len = 0,
      .max_len = DEFAULT_STACK_SIZE,
      .stride = element_size,
      .stack = malloc(DEFAULT_STACK_SIZE * element_size),
  };
}

void Stack_push(Stack *self, void *elem) {
  if (self->len == self->max_len) {
    self->max_len *= 1.5f;
    self->stack = realloc(self->stack, self->max_len * self->stride);
  }

  memcpy(self->stack + self->len * self->stride, elem, self->stride);
  self->len++;
}

void *Stack_pop(Stack *self) {
  if (self->len == 0)
    return NULL;
  self->len--;
  return self->stack + self->len * self->stride;
}
