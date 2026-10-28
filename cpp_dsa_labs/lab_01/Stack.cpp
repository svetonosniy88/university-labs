#include "Stack.h"

#include <iostream>

StackL::StackL() : head(nullptr), count(0) {}

StackL::~StackL()
{
    while (!empty())
        pop();
}

bool StackL::empty() const
{
    return head == nullptr;
}

StackInfo StackL::top() const
{
    StackInfo elem{};
    if (empty())
        std::cout << "Empty\n";
    else
        elem = head->info;
    return elem;
}

void StackL::push(StackInfo elem)
{
    head = new NODE(elem, head);
    ++count;
}

void StackL::pop()
{
    if (empty())
        std::cout << "Empty\n";
    else
    {
        NODE* tmp{ head };
        head = head->next;
        delete tmp;
        --count;
    }
}

std::size_t StackL::size() const
{
    return count;
}

void StackL::show() const
{
    NODE* ptr{ head };
    while (ptr)
    {
        std::cout << ptr->info << ' ';
        ptr = ptr->next;
    }
    std::cout << '\n';
}

StackA::StackA(std::size_t capacity)
    : data(new StackInfo[capacity]{}), MAX_SIZE(capacity), head(-1) {}

StackA::~StackA()
{
    delete[] data;
}

bool StackA::empty() const
{
    return head == -1;
}

bool StackA::full() const
{
    return size() == MAX_SIZE;
}

StackInfo StackA::top() const
{
    StackInfo elem{};
    if (empty())
        std::cout << "Empty\n";
    else
        elem = data[head];
    return elem;
}

void StackA::push(StackInfo elem)
{
    if (full())
        std::cout << "Stack is full\n";
    else
        data[++head] = elem;
}

void StackA::pop()
{
    if (empty())
        std::cout << "Empty\n";
    else
        --head;
}

std::size_t StackA::size() const
{
    return static_cast<std::size_t>(head + 1);
}

void StackA::show() const
{
    for (std::ptrdiff_t i{ head }; i >= 0; --i)
        std::cout << data[i] << ' ';
    std::cout << '\n';
}
