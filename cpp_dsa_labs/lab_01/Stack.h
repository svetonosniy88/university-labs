#pragma once

#include <cstddef>

using StackInfo = char;

class StackL
{
private:
    struct NODE
    {
        StackInfo info;
        NODE* next;
        NODE(StackInfo value, NODE* ptr) : info(value), next(ptr) {}
    };

    NODE* head;
    std::size_t count;
    void show() const;

public:
    StackL();
    ~StackL();
    StackL(const StackL&) = delete;
    StackL& operator=(const StackL&) = delete;

    bool empty() const;
    StackInfo top() const;
    void push(StackInfo elem);
    void pop();
    std::size_t size() const;
};

class StackA
{
private:
    StackInfo* data;
    const std::size_t MAX_SIZE;
    std::ptrdiff_t head;
    void show() const;

public:
    explicit StackA(std::size_t capacity = 100);
    ~StackA();
    StackA(const StackA&) = delete;
    StackA& operator=(const StackA&) = delete;

    bool empty() const;
    bool full() const;
    StackInfo top() const;
    void push(StackInfo elem);
    void pop();
    std::size_t size() const;
};
