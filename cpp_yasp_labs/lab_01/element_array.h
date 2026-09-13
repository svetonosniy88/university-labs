#pragma once

// Один элемент системы непересекающихся множеств.
// value хранит пользовательское значение,
// parent — индекс непосредственного родителя элемента.
struct Element
{
    int value;
    int parent;
};

// Динамический массив всех элементов.
// count показывает число занятых ячеек,
// capacity — число ячеек, под которые выделена память.
struct ElementArray
{
    Element* data;
    int count;
    int capacity;
};

// Ищет элемент по пользовательскому значению.
// Возвращает индекс элемента или -1, если его нет.
int find_element(const ElementArray& elements, int value);

// Увеличивает вместимость динамического массива,
// если свободного места больше нет.
void ensure_capacity(ElementArray& elements);

// Добавляет новый элемент.
// Новый элемент первоначально сам является корнем своей компоненты.
// Возвращает индекс созданного элемента.
int add_element(ElementArray& elements, int value);

// Освобождает память и возвращает массив
// в исходное пустое состояние.
void clear_data(ElementArray& elements);
