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

/*
 * only contains data to traverse. To use, place the list node as the first
 * element in your list structure
 *
 * ```c
 * struct example_node {
 *   list_node node;
 *   int data;
 * };
 * ```
 */
typedef struct list_node {
  struct list_node *prev;
  struct list_node *next;
} ListNode;

typedef struct list {
  ListNode *head;
  ListNode *tail;
} List;

List List_default();
void List_push(List *list, ListNode *node);
ListNode *List_pop(List *list);
void List_remove(List *list, ListNode *node);
void List_destroy(List *list);

#endif // LIST_H
