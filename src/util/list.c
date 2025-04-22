/*
    Linked List Implementation
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

#include "util/list.h"
#include "debug/debug.h"
#include <stdio.h>

List List_create(Allocator *allocator, size_t elem_size) {
  return (List){
      .head = NULL,
      .tail = NULL,
      .allocator = allocator,
      .elem_size = elem_size,
  };
}
ListNode *List_push(List *self) {
  debugAssert(self != NULL, "self == NULL");

  ListNode *node = self->elem_size > 0
                       ? allocPtr(self->allocator, self->elem_size, 1)
                       : allocPtr(self->allocator, sizeof(ListNode), 1);

  if (self->head == NULL) {
    self->tail = self->head = node;
    node->prev = NULL;
    return node;
  }

  self->tail->next = node;
  node->prev = self->tail;
  self->tail = node;
  return node;
}

ListNode *List_pop(List *self) {
  if (self == NULL)
    return NULL;
  if (self->tail == NULL)
    return NULL;

  ListNode *ret = self->tail;
  if (self->tail == self->head) {
    self->tail = self->head = NULL;
    return ret;
  }

  self->tail = self->tail->prev;
  self->tail->next = NULL;
  ret->prev = NULL;

  return ret;
}

void List_remove(List *self, ListNode *node) {
  if (self == NULL)
    return;
  if (node == NULL)
    return;

  if (self->head == node) {
    if (self->head == self->tail) {
      self->head = self->tail = NULL;
      return;
    }

    self->head = self->head->next;
    self->head->prev = NULL;
    node->next = NULL;
    return;
  }

  if (self->tail == node) {
    self->tail = self->tail->prev;
    self->tail->next = NULL;
    node->prev = NULL;

    return;
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->next = NULL;
  node->prev = NULL;
}

void List_destroy(List *self) {
  debugAssert(self != NULL, "self == NULL");
  for (ListNode *node = self->head; node != NULL;) {
    ListNode *next = node->next;
    freePtr(self->allocator, node);

    node = next;
  }
}
