#pragma once

#include "element_array.h"

// Находит индекс корня компоненты элемента.
int find_root(const ElementArray& elements, int index);

// Проверяет, принадлежат ли два элемента одной компоненте.
bool is_connected(
    const ElementArray& elements,
    int first_index,
    int second_index
);

// Объединяет две компоненты,
// присоединяя корень первой к корню второй.
void unite(
    ElementArray& elements,
    int first_index,
    int second_index
);

// Обрабатывает очередную пару.
//
// Если связь уже следует из существующих связей,
// пара отклоняется и функция возвращает false.
//
// Иначе необходимые элементы создаются,
// их компоненты объединяются и функция возвращает true.
bool process_pair(ElementArray& elements, int p, int q);
