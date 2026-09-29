#pragma once

#include <fstream>
#include <iostream>

using TInfo = int;

struct NODE
{
    TInfo info;
    NODE* next;

    NODE(TInfo info, NODE* ptr) : info(info), next(ptr) {}

    ~NODE()
    {
        next = nullptr;
    }
};

using ptrNODE = NODE*;

void init(ptrNODE& head);
bool empty(ptrNODE head);
void add_by_pointer(ptrNODE& ptr, TInfo elem);
void add_to_head(ptrNODE& head, TInfo elem);
void add_after(ptrNODE& ptr, TInfo elem);
void del_by_pointer(ptrNODE& ptr);
void del_from_head(ptrNODE& head);
void del_after(ptrNODE& ptr);
void print(ptrNODE head, std::ostream& stream = std::cout);
void clear(ptrNODE& head);
void create_by_stack(ptrNODE& head, std::ifstream& file);
void create_by_queue(ptrNODE& head, std::ifstream& file);
ptrNODE create_by_order(std::ifstream& file);
