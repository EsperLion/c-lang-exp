#include "../include/list.h"
#include <stdlib.h>
#include <string.h>

typedef struct ListNode {
  ListNode *next;
  void *data;
  ListNode *prev;
} ListNode;

typedef struct List {
  ListNode *head;
  ListNode *tail;
  size_t size;
  size_t elem_size;
} List;

List *list_create(size_t elem_size) {
  List *list = malloc(sizeof(List));
  list->size = 0;
  list->elem_size = elem_size;
  list->head = NULL;
  list->tail = NULL;

  return list;
}

void list_destroy(List **pList) {
  if (pList == NULL || *pList == NULL)
    return;

  List *list = *pList;
  ListNode *current = list->head;
  while (current != NULL) {
    ListNode *next = current->next;
    free(current->data);
    free(current);
    current = next;
  }

  free(list);
  *pList = NULL;
  return;
}

size_t list_size(const List *list) {
  if (list == NULL)
    return 0;

  return list->size;
}

int list_is_empty(const List *list) {
  if (list == NULL)
    return 0;

  return list->size == 0;
}

int list_push_front(List *list, const void *value) {
  if (list == NULL || value == NULL)
    return 0;

  // create new node
  ListNode *node = malloc(sizeof(ListNode));
  if (node == NULL)
    return 0;

  node->data = malloc(list->elem_size);
  if (node->data == NULL) {
    free(node);
    return 0;
  }

  memcpy(node->data, value, list->elem_size);
  node->next = list->head;
  node->prev = NULL;

  if (list->head != NULL) {
    list->head->prev = node;
  } else {
    // runs once when list is empty
    list->tail = node;
  }

  list->head = node;
  list->size++;

  return 1;
}

int list_push_back(List *list, const void *value) {
  if (list == NULL || value == NULL)
    return 0;

  // create new node
  ListNode *node = malloc(sizeof(ListNode));
  if (node == NULL)
    return 0;

  node->data = malloc(list->elem_size);
  if (node->data == NULL) {
    free(node);
    return 0;
  }
  memcpy(node->data, value, list->elem_size);
  node->next = NULL;
  node->prev = list->tail;

  if (list->tail != NULL) {
    list->tail->next = node;
  } else {
    list->head = node;
  }

  list->tail = node;
  list->size++;

  return 1;
}

int list_pop_front(List *list, void *out) {
  if (list == NULL || list->size == 0)
    return 0;

  ListNode *node = list->head;
  if (out != NULL) {
    memcpy(out, node->data, list->elem_size);
  }

  list->head = node->next;
  if (list->head != NULL) {
    list->head->prev = NULL;
  } else {
    list->tail = NULL;
  }
  free(node->data);
  free(node);
  list->size--;

  return 1;
}

int list_pop_back(List *list, void *out) {
  if (list == NULL || list->size == 0)
    return 0;

  ListNode *node = list->tail;
  if (out != NULL) {
    memcpy(out, node->data, list->elem_size);
  }

  list->tail = node->prev;
  if (list->tail != NULL) {
    list->tail->next = NULL;
  } else {
    list->head = NULL;
  }

  free(node->data);
  free(node);
  list->size--;
  return 1;
}

int list_get(const List *list, size_t index, void *out) {
  if (list == NULL || list->size == 0 || out == NULL)
    return 0;

  if (index >= list->size)
    return 0;

  if (index == 0) {
    memcpy(out, list->head->data, list->elem_size);
    return 1;
  }
  if (index == list->size - 1) {
    memcpy(out, list->tail->data, list->elem_size);
    return 1;
  }

  ListNode *curr = list->head;
  size_t currIndex = 0;
  while (currIndex < index) {
    curr = curr->next;
    currIndex++;
  }
  memcpy(out, curr->data, list->elem_size);

  return 1;
}

int list_set(List *list, size_t index, const void *value) {
    if (list == NULL || list->size == 0 || value == NULL)
      return 0;

    if (index >= list->size)
      return 0;

    if (index == 0) {
      memcpy(list->head->data, value, list->elem_size);
      return 1;
    }
    if (index == list->size - 1) {
      memcpy(list->tail->data, value, list->elem_size);
      return 1;
    }

    ListNode *curr = list->head;
    size_t currIndex = 0;
    while (currIndex < index) {
      curr = curr->next;
      currIndex++;
    }
    memcpy(curr->data, value, list->elem_size);

    return 1;
}

int list_remove_at(List *list, size_t index) {
    if (list == NULL || list->size == 0 || index >= list-> size)
      return 0;

    if (index == 0) {
        ListNode *node = list->head;
        list->head = node->next;

        if (list->head)
            list->head->prev = NULL;
        else
            list->tail = NULL;

        free(node->data);
        free(node);
        list->size--;
        return 1;
    }

    if (index == list->size - 1) {
        ListNode *node = list->tail;
        list->tail = node->prev;
        if (list->tail)
            list->tail->next = NULL;
        else
         list->head = NULL;

        free(node->data);
        free(node);
        list->size--;
        return 1;
    }

    ListNode *curr = list->head;
    size_t currIndex = 0;
    while (currIndex < index) {
        curr = curr->next;
        currIndex++;
    }
    ListNode *prev = curr->prev;
    ListNode *next = curr->next;
    free(curr->data);
    free(curr);
    prev->next = next;
    next->prev = prev;
    list->size--;
    return  1;
}

void list_clear(List *list) {
    if (list == NULL)
      return;

    ListNode *current = list->head;
    while (current != NULL) {
      ListNode *next = current->next;
      free(current->data);
      free(current);
      current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return;
}

void list_foreach(List *list, void (*callback)(void *elem)) {
    if (list == NULL || callback == NULL)
      return;

    ListNode *current = list->head;
    while (current != NULL) {
      ListNode *next = current->next;
      callback(current->data);
      current = next;
    }

    return;
}
