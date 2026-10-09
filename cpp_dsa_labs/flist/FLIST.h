#pragma once

#include <iostream>
#include <functional>
#include <fstream>

namespace flist
{
	using TInfo = int;

	struct NODE
	{
		TInfo info;
		NODE* next;
		NODE() : info{}, next{} {}
		NODE(TInfo info, NODE* ptr = nullptr) : info(info), next(ptr) {}
		~NODE()
		{
			next = nullptr;
		}
	};

	using ptrNODE = NODE*;

	struct FLIST
	{
	private:
		ptrNODE head;
		void add_by_pointer(ptrNODE& ptr, TInfo elem)
		{
			ptr = new NODE(elem, ptr);
		}
		void del_by_pointer(ptrNODE& ptr)
		{
			if (ptr)
			{
				ptrNODE tmp{ ptr };
				ptr = ptr->next;
				delete tmp;
			}
			else
				std::cout << "Error";
		}
		void init()
		{
			head = new NODE(0);
		}
	public:
		FLIST()
		{
			init();
		}
		FLIST(const FLIST&) = delete;
		FLIST& operator=(const FLIST&) = delete;
		~FLIST();
		ptrNODE get_head()
		{
			return head;
		}
		TInfo get_elem(ptrNODE ptr)
		{
			return ptr->info;
		}
		bool empty();
		void push_front(TInfo elem);
		void pop_front();
		void insert_after(ptrNODE ptr, TInfo elem);
		void erase_after(ptrNODE ptr);
		void clear(ptrNODE begin, ptrNODE end = nullptr);
		void create_by_queue(std::ifstream& file);
		void print(std::ostream& stream = std::cout);
		ptrNODE find_if(ptrNODE begin, ptrNODE end, std::function<bool(TInfo)> predicat);
		void remove_if(std::function<bool(TInfo)> predicat);
		//void remove(TInfo value);
		void removee(TInfo value);
		void sorting();
		void switch_fragment(ptrNODE where, ptrNODE begin, ptrNODE end);
	};

}
