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

#ifndef DEFAULT_STACK_SIZE
#define DEFAULT_STACK_SIZE 8
#endif

typedef struct stack {
  size_t len;
  size_t max_len;
  size_t stride;
  void *stack;
} Stack;

Stack Stack_create(size_t element_size);
void Stack_push(Stack *self, void *elem);
void *Stack_pop(Stack *self);

#endif // STACK_H
