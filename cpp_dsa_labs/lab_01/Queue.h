#pragma once

#include <cstddef>
#include <string>

using QueueInfo = std::string;

class QueueL
{
private:
    struct NODE
    {
        QueueInfo info;
        NODE* next;
        explicit NODE(const QueueInfo& value) : info(value), next(nullptr) {}
    };

    NODE* head;
    NODE* tail;
    std::size_t count;

public:
    QueueL();
    ~QueueL();
    QueueL(const QueueL&) = delete;
    QueueL& operator=(const QueueL&) = delete;

    bool empty() const;
    QueueInfo front() const;
    QueueInfo back() const;
    void push(const QueueInfo& elem);
    void pop();
    std::size_t size() const;
    void view() const;
};
