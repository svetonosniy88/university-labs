#include "element_array.h"

// Линейно проходит по массиву и сравнивает
// пользовательские значения элементов с искомым.
int find_element(const ElementArray& elements, int value)
{
    for (int index{}; index < elements.count; ++index)
    {
        if (elements.data[index].value == value)
        {
            return index;
        }
    }

    return -1;
}

void ensure_capacity(ElementArray& elements)
{
    // Пока есть свободная ячейка, перевыделять память не нужно.
    if (elements.count < elements.capacity)
    {
        return;
    }

    // При первом добавлении выделяются четыре ячейки.
    // При последующих расширениях вместимость удваивается:
    // 4 -> 8 -> 16 -> 32 и так далее.
    elements.capacity = elements.capacity == 0
        ? 4
        : elements.capacity * 2;

    // Создаётся новый массив увеличенного размера.
    Element* new_elements = new Element[elements.capacity];

    // Все занятые элементы переносятся в новую область памяти.
    for (int index{}; index < elements.count; ++index)
    {
        new_elements[index] = elements.data[index];
    }

    // Старый массив больше не нужен: его память освобождается,
    // после чего data начинает указывать на новый массив.
    delete[] elements.data;
    elements.data = new_elements;
}

int add_element(ElementArray& elements, int value)
{
    // Перед записью обязательно проверяется наличие свободного места.
    ensure_capacity(elements);

    const int new_index{ elements.count };

    // Новый элемент пока ни с кем не объединён,
    // поэтому сам является корнем своей компоненты.
    elements.data[new_index].value = value;
    elements.data[new_index].parent = new_index;
    ++elements.count;

    return new_index;
}

void clear_data(ElementArray& elements)
{
    // delete[] безопасен и для nullptr.
    delete[] elements.data;

    // После очистки структура возвращается
    // в то же состояние, в котором была создана.
    elements.data = nullptr;
    elements.count = 0;
    elements.capacity = 0;
}
