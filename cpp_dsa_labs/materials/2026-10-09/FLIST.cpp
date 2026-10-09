#include "FLIST.h"

flist::FLIST::~FLIST()
{
	clear(head);
	delete(head);
}

bool flist::FLIST::empty()
{
	return head->next == nullptr;
}

void flist::FLIST::push_front(TInfo elem)
{
	add_by_pointer(head->next, elem);
}

void flist::FLIST::pop_front()
{
	del_by_pointer(head->next);
}

void flist::FLIST::insert_after(ptrNODE ptr, TInfo elem)
{
	add_by_pointer(ptr->next, elem);
}

void flist::FLIST::erase_after(ptrNODE ptr)
{
	del_by_pointer(ptr->next);
}

void flist::FLIST::clear(ptrNODE begin, ptrNODE end)
{
	while (begin->next != end)
	{
		//del_by_pointer(begin->next);
		erase_after(begin);
	}
}

void flist::FLIST::create_by_queue(std::ifstream& file)
{
	ptrNODE tail{ head };
	TInfo elem{};
	while (file >> elem)
	{
		insert_after(tail, elem);
		tail = tail->next;
	}
}

void flist::FLIST::print(std::ostream& stream)
{
	if (empty())
		stream << "Empty list";
	else
	{
		ptrNODE ptr{ head->next };
		while (ptr)
		{
			stream << ptr->info << ' ';
			ptr = ptr->next;
		}
	}
	stream << '\n';
}

flist::ptrNODE flist::FLIST::find_if(ptrNODE begin, ptrNODE end, std::function<bool(TInfo)> predicat)
{
	ptrNODE result{}, ptr{ begin };

	while (ptr->next != end && !result)
	{
		if (predicat(ptr->next->info))
			result = ptr;
		else
			ptr = ptr->next;
	}

	return result;
}

void flist::FLIST::remove_if(std::function<bool(TInfo)> predicat)
{
	ptrNODE ptr{ head };

	while (ptr->next) {
		if (predicat(ptr->next->info)) {
			erase_after(ptr);
		}
		else ptr = ptr->next;
	}
}

void flist::FLIST::removee(TInfo value)
{
	remove_if([value](TInfo x) {return x == value; });
}

void flist::FLIST::sorting()
{
	auto switch_pointers = [](ptrNODE q, ptrNODE p)
		{
			ptrNODE tmp{ p->next };
			p->next = tmp->next;
			tmp->next = q->next;
			q->next = tmp;
		};
	auto find_place = [this](TInfo elem) //->ptrNODE
		{
			ptrNODE result{ head };
			while (result->next && result->next->info < elem)
				result = result->next;
			return result;
		};
	ptrNODE h{ head->next->next }, h_prev{ head->next };
	while (h)
	{
		if (h_prev->info > h->info)
		{
			switch_pointers(find_place(h->info), h_prev);
			h = h_prev->next;
		}
		else
		{
			h_prev = h;
			h = h->next;
		}
	}
}

void flist::FLIST::switch_fragment(ptrNODE where, ptrNODE begin, ptrNODE end) {
	flist::ptrNODE tmp { begin->next };
	begin->next = end->next;
	end->next = where->next;
	where->next = tmp->next;
}





