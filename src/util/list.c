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
#include <stdio.h>

List List_default() { return (List){.head = NULL, .tail = NULL}; }
void List_push(List *list, ListNode *node) {
  if (list == NULL)
    return;
  if (node == NULL)
    return;

  if (list->head == NULL) {
    list->tail = list->head = node;
    node->prev = NULL;
    return;
  }

  list->tail->next = node;
  node->prev = list->tail;
  list->tail = node;
}

ListNode *List_pop(List *list) {
  if (list == NULL)
    return NULL;
  if (list->tail == NULL)
    return NULL;

  ListNode *ret = list->tail;
  if (list->tail == list->head) {
    list->tail = list->head = NULL;
    return ret;
  }

  list->tail = list->tail->prev;
  list->tail->next = NULL;
  ret->prev = NULL;

  return ret;
}

void List_remove(List *list, ListNode *node) {
  if (list == NULL)
    return;
  if (node == NULL)
    return;

  if (list->head == node) {
    if (list->head == list->tail) {
      list->head = list->tail = NULL;
      return;
    }

    list->head = list->head->next;
    list->head->prev = NULL;
    node->next = NULL;
    return;
  }

  if (list->tail == node) {
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    node->prev = NULL;

    return;
  }

  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->next = NULL;
  node->prev = NULL;
}
