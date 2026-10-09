#include "FLIST.h"
#include <iostream>
#include <fstream>
#include <functional>


using TInfo = int;

struct NODE
{
	TInfo info;
	NODE* next;
	NODE() {}
	NODE(TInfo info, NODE* ptr = nullptr) : info(info), next(ptr)
	{
		//this->info = info;
		//next = ptr;
	}
	~NODE()
	{
		next = nullptr;
	}
};

using ptrNODE = NODE*;

void init(ptrNODE &head)
{
	head = nullptr;
}

bool empty(ptrNODE head)
{
	return head == nullptr;
}

void push_front(ptrNODE& head, TInfo elem)
{
	head = new NODE(elem, head);
}

void insert_after(ptrNODE& ptr, TInfo elem)
{
	if (ptr)
		ptr->next = new NODE(elem, ptr->next);
	else
		std::cout << "Error";
}



void erase_after(ptrNODE& ptr)
{
	if (ptr && ptr->next)
	{
		ptrNODE p{ ptr->next };
		ptr->next = ptr->next->next; // p->next;
		delete p;
	}
}

void create_by_stack(ptrNODE& head, std::ifstream& file)
{
	init(head);
	TInfo elem{};
	while (file >> elem)
	{
		push_front(head, elem);
	}
}

void create_by_queue(ptrNODE& head, std::ifstream& file)
{
	init(head);
	TInfo elem{};
	file >> elem;
	push_front(head, elem);
	ptrNODE tail{ head };
	while (file >> elem)
	{
		insert_after(tail, elem);
		tail = tail->next;
	}
}

void create_by_order(ptrNODE& head, std::ifstream& file)
{
	TInfo elem{};
	init(head);
	auto find_place = [&head](TInfo elem)
		{
			ptrNODE ptr{ head };
			while (ptr->next && ptr->next->info < elem)
				ptr = ptr->next;
			return ptr;
		};
	ptrNODE ptr{};
	while (file >> elem)
	{
		if (empty(head) || head->info >= elem)
			push_front(head, elem);
		else
		{
			ptr = find_place(elem);
			insert_after(ptr, elem);
		}
	}
}

void print(ptrNODE head, std::ostream& stream = std::cout)
{
	if (empty(head))
		std::cout << "Empty";
	else
	{
		ptrNODE ptr{ head };
		while (ptr)
		{
			stream << ptr->info << ' ';
			ptr = ptr->next;
		}
	}
	stream << '\n';
}

/*void clear(ptrNODE& head)
{
	while (head)
		pop_front(head);
}*/

////---------------------------------------------
//void task1(ptrNODE head)
//{
//	ptrNODE ptr{ head };
//	while (ptr)
//	{
//		if (ptr->info % 2 != 0)
//		{
//			insert_after(ptr, ptr->info);
//			ptr = ptr->next;
//		}
//		ptr = ptr->next;
//	}
//}
//
//bool task2(ptrNODE& head)
//{
//	bool res{};
//	if (head->info % 2 != 0)
//	{
//		pop_front(head);
//		res = true;
//	}
//	else
//	{
//		auto find = [head, &res]()
//			{
//				ptrNODE ptr{ head };
//				while (ptr->next && !res)
//				{
//					if (ptr->next->info % 2 != 0)
//					{
//						res = true;
//					}
//					else
//					{
//						ptr = ptr->next;
//					}
//				}
//				return ptr;
//			};
//		ptrNODE ptr{ find() };
//		if (res)
//		{
//			erase_after(ptr);
//		}
//	}
//	return res;
//}
//
//bool task3(ptrNODE& head)
//{
//	bool flag{ false };
//
//	while (head && head->info % 2 != 0)
//	{
//		pop_front(head);
//		flag = true;
//	}
//
//	if (head)
//	{
//		ptrNODE ptr{ head };
//		while (ptr->next)
//		{
//			if (ptr->next->info % 2 != 0)
//			{
//				erase_after(ptr);
//				flag = true;
//			}
//			else
//			{
//				ptr = ptr->next;
//			}
//		}
//	}
//
//	return flag;
//}
//
//int main()
//{
//	std::ifstream file("data.txt");
//	if (file)
//	{
//		ptrNODE head{};
//		//create_by_stack(head, file);
//		create_by_queue(head, file);
//		//create_by_order(head, file);
//		print(head);
//		//task1(head);
//		if (task3(head))
//		{
//			print(head);
//		}
//		else
//		{
//			std::cout << "No\n";
//		}
//
//		
//		clear(head);
//	}
//	else
//		std::cout << "File error!\n";
//
//	return 0;
//}
//

// функция которая находит максимальный элемент

auto max = [](TInfo x, TInfo y) {
	return x > y ? x : y;
	};

TInfo find_max(flist::ptrNODE head) {
	TInfo result{};
	if (head->next) {
		result = max(head->info, find_max(head->next));
	}
	else {
		result = head->info;
	}
	return result;
}

// печатает список в обратном порядке

void print_reverse(ptrNODE head) {
	if (head) {
		print_reverse(head->next);
		std::cout << head->info << ' ';
	}
}

//заменяет все x1 на x2

void replace_all(ptrNODE head, TInfo x1, TInfo x2) {
	if (head) {
		if (head->info == x1) {
			head->info = x2;
		}
		replace_all(head->next, x1, x2);
	}
}

// удваивает первое вхождение



//удваивает все вхождения



//удваивает x



//проверка списка на упорядоченность по возрастанию (строгое)




//Среднее арифметическое чисел оканчивающихся на заданную цифру
bool task(flist::FLIST& list, int digit, double& aver)
{
	flist::ptrNODE ptr{ list.get_head()->next };
	flist::TInfo sum{};
	int cnt{};
	while (ptr)
	{
		if (abs(ptr->info) % 10 == digit)
		{
			sum += ptr->info;
			++cnt;
		}
		ptr = ptr->next;
	}
	bool result{};
	if (cnt)
	{
		aver = static_cast<double>(sum) / cnt;
		result = true;
	}
	return result;
}

bool task2(flist::FLIST& list, flist::TInfo& sum)
{
	flist::ptrNODE head{ list.get_head() };
	bool result{};
	if (head->next)
		if (head->next->next)
		{
			result = true;
			while (head->next->next)
			{
				head = head->next;
			}
			sum = head->info + head->next->info;
		}
	return result;
}

bool task3(flist::FLIST& list)
{
	bool ans{ true };
	flist::ptrNODE ptr{ list.get_head()->next };
	while (ans && ptr->next)
	{
		if (ptr->info > ptr->next->info)
		{
			ans = false;
		}
		ptr = ptr->next;
	}
	return ans;
}

void task4(flist::FLIST& list1, flist::FLIST& list2)
{
	flist::ptrNODE ptr{ list1.get_head()->next }, tail{ list2.get_head() };
	flist::TInfo max{ ptr->info };
	int num{ 1 };
	while (ptr) {
		if (ptr->info == max)
		{
			list2.insert_after(tail, num);
			tail = tail->next;
		}
		else if (ptr->info > max)
		{
			max = ptr->info;
			list2.clear(list2.get_head());
			tail = list2.get_head();
			list2.insert_after(tail, num);
			tail = tail->next;
		}
		ptr = ptr->next;
		++num;

	}
}
bool find_last(flist::FLIST& list, flist::ptrNODE& beg, flist::ptrNODE& end, 
	std::function<bool(flist::TInfo, flist::TInfo)> predicat)  {
	beg = end = nullptr;
	flist::ptrNODE head{ list.get_head()};
	flist::ptrNODE ptr{ head };
	while (ptr->next) { // первый фрагмент && !end;
		if ((ptr == head || !predicat(ptr->info, ptr->next->info)) && ptr->next->next &&
			predicat(ptr->next->info, ptr->next->next->info)) {
			beg = ptr;
		}
		else
			if ((!ptr->next->next || predicat(ptr->info, ptr->next->info)) && ptr->next->next &&
				!predicat(ptr->next->info, ptr->next->next->info))
				end = ptr;
		ptr = ptr->next;
	
	}	
	if (beg && end) {
		std::cout << beg->info << ' ' << end->info << '\n';
	}
	return end != nullptr;
}

bool find_last_ver2(flist::FLIST& list, flist::ptrNODE& beg, flist::ptrNODE& end,
	std::function<bool(flist::TInfo, flist::TInfo)> predicat) {
	beg = end = nullptr;
	flist::ptrNODE head{ list.get_head() };
	flist::ptrNODE ptr{ head };
	bool is_beg{};
	while (ptr->next) {
		if (ptr->next->next && predicat(ptr->next->info, ptr->next->next->info)) {
			if (!is_beg) {
				beg = ptr;
				is_beg = true;
			}
			else {
				if (predicat(ptr->info, ptr->next->info)) {
					end = ptr->next;
					is_beg = false;
				}
			}
		}
	}
	if (beg && end) {
		std::cout << beg->info << ' ' << end->info << '\n';
	}
	return end != nullptr;

}



int main()
{
	std::ifstream file("data.txt");
	if (file)
	{
		flist::FLIST list;
		list.create_by_queue(file);
		list.print();
		/*auto a = [](flist::TInfo x) { return x % 5 == 0; };
		flist::ptrNODE temp{ list.find_if(list.get_head(), nullptr, a) };
		if (temp)
			std::cout << temp->next->info << '\n';
		else
			std::cout << "NO\n";*/
			//list.remove_if([](flist::TInfo x) {return x % 2 != 0; });
			//list.removee(1);
			//list.sorting();
			//list.print();
			//flist::TInfo aver{};
		flist::FLIST list2;
		task4(list, list2);
		list2.print();
	}

	return 0;
}

