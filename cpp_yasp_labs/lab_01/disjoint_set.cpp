#include "disjoint_set.h"

// Поднимается по цепочке непосредственных родителей,
// пока не встретит элемент, который указывает сам на себя.
int find_root(const ElementArray& elements, int index)
{
    while (elements.data[index].parent != index)
    {
        index = elements.data[index].parent;
    }

    return index;
}

bool is_connected(
    const ElementArray& elements,
    int first_index,
    int second_index
)
{
    // Два элемента связаны тогда и только тогда,
    // когда корни их компонент совпадают.
    return find_root(elements, first_index)
        == find_root(elements, second_index);
}

void unite(
    ElementArray& elements,
    int first_index,
    int second_index
)
{
    // Объединять нужно именно корни, а не исходные элементы:
    // тогда целиком соединяются две компоненты.
    const int first_root{ find_root(elements, first_index) };
    const int second_root{ find_root(elements, second_index) };

    if (first_root != second_root)
    {
        // Корень первой компоненты становится потомком
        // корня второй компоненты.
        elements.data[first_root].parent = second_root;
    }
}

bool process_pair(ElementArray& elements, int p, int q)
{
    // Пользовательские значения сначала переводятся
    // во внутренние индексы массива.
    int first_index{ find_element(elements, p) };
    int second_index{ find_element(elements, q) };

    if (first_index != -1
        && second_index != -1
        && is_connected(elements, first_index, second_index))
    {
        // Оба значения уже существуют и имеют общий корень.
        // Новая пара не добавляет никакой связи.
        return false;
    }

    if (p == q)
    {
        // Новая пара вида p-p должна создать только один элемент,
        // а не две копии одного пользовательского значения.
        add_element(elements, p);
        return true;
    }

    if (first_index == -1)
    {
        // Отсутствующее значение добавляется
        // как отдельная одноэлементная компонента.
        first_index = add_element(elements, p);
    }

    if (second_index == -1)
    {
        second_index = add_element(elements, q);
    }

    // К этому моменту оба индекса существуют
    // и принадлежат разным компонентам.
    unite(elements, first_index, second_index);
    return true;
}
