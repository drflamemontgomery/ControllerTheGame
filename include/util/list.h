/*
    Linked List Structure
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

#ifndef LIST_H
#define LIST_H

#include <stdio.h>

#include "heap/allocator.h"

typedef struct ListNode {
  struct ListNode *prev;
  struct ListNode *next;
  void *value;
} ListNode;

/** \brief Linked List Structure
 *
 * \param elem_size   when elem_size == 0. Allocation of elements is handled by
 *                    the list holder
 */
typedef struct List {
  ListNode *head;
  ListNode *tail;

  size_t elem_size;
  Allocator *allocator;
} List;

List List_create(Allocator *allocator, size_t elem_size);
ListNode *List_push(List *self);
ListNode *List_pop(List *self);
void List_remove(List *self, ListNode *node);
void List_destroy(List *self);

#endif // LIST_H
