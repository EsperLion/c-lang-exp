#ifndef LIST_H
#define LIST_H

#include <stddef.h> // size_t

// Узел списка (скрыт от пользователя в .c)
typedef struct ListNode ListNode;

// Двусвязный список
typedef struct List List;

/*
 * Создаёт пустой список для элементов заданного размера.
 *
 * Пример:
 *   List *list = list_create(sizeof(int));
 */
List *list_create(size_t elem_size);

/*
 * Освобождает всю память списка и обнуляет указатель.
 * НЕ освобождает вложенные ресурсы элементов.
 *
 * Пример:
 *   list_destroy(&list);
 */
void list_destroy(List **pList);

/*
 * Возвращает количество элементов в списке.
 *
 * Пример:
 *   size_t n = list_size(list);
 */
size_t list_size(const List *list);

/*
 * Проверяет, пуст ли список.
 *
 * Пример:
 *   if (list_is_empty(list)) { ... }
 */
int list_is_empty(const List *list);

/*
 * Добавляет элемент в начало списка.
 *
 * Пример:
 *   int x = 10;
 *   list_push_front(list, &x);
 */
int list_push_front(List *list, const void *value);

/*
 * Добавляет элемент в конец списка.
 *
 * Пример:
 *   int x = 20;
 *   list_push_back(list, &x);
 */
int list_push_back(List *list, const void *value);

/*
 * Удаляет первый элемент списка и копирует его значение в out.
 *
 * Пример:
 *   int x;
 *   list_pop_front(list, &x);
 */
int list_pop_front(List *list, void *out);

/*
 * Удаляет последний элемент списка и копирует его значение в out.
 *
 * Пример:
 *   int x;
 *   list_pop_back(list, &x);
 */
int list_pop_back(List *list, void *out);

/*
 * Получает элемент по индексу (O(n)).
 *
 * Пример:
 *   int x;
 *   list_get(list, 2, &x);
 */
int list_get(const List *list, size_t index, void *out);

/*
 * Заменяет значение элемента по индексу (O(n)).
 *
 * Пример:
 *   int x = 42;
 *   list_set(list, 1, &x);
 */
int list_set(List *list, size_t index, const void *value);

/*
 * Удаляет элемент по индексу (O(n)).
 *
 * Пример:
 *   list_remove_at(list, 0);
 */
int list_remove_at(List *list, size_t index);

/*
 * Полностью очищает список.
 * После вызова size == 0.
 *
 * Пример:
 *   list_clear(list);
 */
void list_clear(List *list);

/*
 * Итерация по списку.
 * callback вызывается для каждого элемента.
 *
 * Пример:
 *   void print_int(void *elem) {
 *       printf("%d\n", *(int *)elem);
 *   }
 *   list_foreach(list, print_int);
 */
void list_foreach(List *list, void (*callback)(void *elem));

#endif // LIST_H
