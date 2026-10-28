#include "Queue.h"

#include <iostream>

QueueL::QueueL() : head(nullptr), tail(nullptr), count(0) {}

QueueL::~QueueL()
{
    while (!empty())
        pop();
}

bool QueueL::empty() const
{
    return head == nullptr;
}

QueueInfo QueueL::front() const
{
    QueueInfo elem{};
    if (empty())
        std::cout << "Queue is empty\n";
    else
        elem = head->info;
    return elem;
}

QueueInfo QueueL::back() const
{
    QueueInfo elem{};
    if (empty())
        std::cout << "Queue is empty\n";
    else
        elem = tail->info;
    return elem;
}

void QueueL::push(const QueueInfo& elem)
{
    NODE* tmp{ new NODE(elem) };
    if (empty())
        head = tmp;
    else
        tail->next = tmp;
    tail = tmp;
    ++count;
}

void QueueL::pop()
{
    if (empty())
        std::cout << "Queue is empty\n";
    else
    {
        NODE* tmp{ head };
        head = head->next;
        delete tmp;
        --count;
        if (empty())
            tail = nullptr;
    }
}

std::size_t QueueL::size() const
{
    return count;
}

void QueueL::view() const
{
    NODE* ptr{ head };
    while (ptr)
    {
        std::cout << ptr->info << ' ';
        ptr = ptr->next;
    }
    std::cout << '\n';
}
