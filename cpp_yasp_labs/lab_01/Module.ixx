module;

#include <iostream>
#include <fstream>
#include <limits>

export module lab_module;


// Один элемент системы непересекающихся множеств.
export struct Element
{
    int value;
    int parent;
};


// Динамический массив всех элементов.
export struct ElementArray
{
    Element* data;
    int count;
    int capacity;
};


// Внутренние функции

int find_element(
    const ElementArray& elements,
    int value
)
{
    for (int index{}; index < elements.count; ++index)
        if (elements.data[index].value == value)
            return index;

    return -1;
}


void ensure_capacity(
    ElementArray& elements
)
{
    if (elements.count < elements.capacity)
        return;

    if (elements.capacity == 0)
        elements.capacity = 4;
    else
        elements.capacity *= 2;

    Element* new_elements = new Element[elements.capacity];

    for (int i{}; i < elements.count; ++i)
        new_elements[i] = elements.data[i];

    delete[] elements.data;
    elements.data = new_elements;
}


int add_element(
    ElementArray& elements,
    int value
)
{
    ensure_capacity(elements);

    elements.data[elements.count].value = value;
    elements.data[elements.count].parent = elements.count;

    int new_index{ elements.count };

    ++elements.count;

    return new_index;
}


int find_root(
    const ElementArray& elements,
    int index
)
{
    while (elements.data[index].parent != index)
        index = elements.data[index].parent;

    return index;
}


bool is_connected(
    const ElementArray& elements,
    int first_index,
    int second_index
)
{
    return find_root(elements, first_index) ==
        find_root(elements, second_index);
}


void unite(
    ElementArray& elements,
    int first_index,
    int second_index
)
{
    int first_root{ find_root(elements, first_index) };
    int second_root{ find_root(elements, second_index) };

    if (first_root == second_root)
        return;

    elements.data[first_root].parent = second_root;
}



// Публичные функции

export int read_int()
{
    int value{};

    while (!(std::cin >> value))
    {
        std::cout << "Invalid input. Enter an integer: ";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }

    return value;
}


export void read_pair(
    int& p,
    int& q
)
{
    while (true)
    {
        int first{};
        int second{};

        if (std::cin >> first >> second)
        {
            p = first;
            q = second;

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );

            return;
        }

        std::cout << "Invalid pair. Enter two integers: ";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );
    }
}


export bool process_pair(
    ElementArray& elements,
    int p,
    int q
)
{
    int first_index{ find_element(elements, p) };
    int second_index{ find_element(elements, q) };

    if (first_index != -1 &&
        second_index != -1 &&
        is_connected(elements, first_index, second_index))
        return false;

    if (p == q)
    {
        add_element(elements, p);
        return true;
    }

    if (first_index == -1)
        first_index = add_element(elements, p);

    if (second_index == -1)
        second_index = add_element(elements, q);

    unite(elements, first_index, second_index);

    return true;
}


export void print_groups(
    const ElementArray& elements,
    std::ostream& output
)
{
    if (elements.count == 0)
    {
        output << "No data.\n";
        return;
    }

    for (int i{}; i < elements.count; ++i)
    {
        if (elements.data[i].parent != i)
            continue;

        output << "{ ";

        for (int j{}; j < elements.count; ++j)
            if (find_root(elements, j) == i)
                output << elements.data[j].value << ' ';

        output << "}\n";
    }
}


export void clear_data(
    ElementArray& elements
)
{
    delete[] elements.data;

    elements.data = nullptr;
    elements.count = 0;
    elements.capacity = 0;
}

export void clear_result_file()
{
    std::ofstream file{
        "result.txt",
        std::ios::trunc
    };
}

export bool load_from_file(
    ElementArray& elements
)
{
    std::ifstream file{ "input.txt" };

    if (!file.is_open())
        return false;

    clear_data(elements);

    int p{};
    int q{};

    while (file >> p)
    {
        if (!(file >> q))
        {
            clear_data(elements);
            return false;
        }

        process_pair(elements, p, q);
    }

    if (!file.eof())
    {
        clear_data(elements);
        return false;
    }

    return true;
}


export bool save_to_file(
    const ElementArray& elements
)
{
    std::ofstream file{ "result.txt" };

    if (!file.is_open())
        return false;

    print_groups(elements, file);

    return true;
}