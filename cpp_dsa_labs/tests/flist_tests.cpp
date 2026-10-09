#include "recursive_tasks.h"

#include <algorithm>
#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>

static_assert(!std::is_copy_constructible_v<flist::FLIST>);
static_assert(!std::is_copy_assignable_v<flist::FLIST>);

void require(bool condition, const char* message)
{
    if (!condition)
        throw std::runtime_error(message);
}

void fill(flist::FLIST& list, std::initializer_list<int> values)
{
    list.clear(list.get_head());
    flist::ptrNODE tail{ list.get_head() };
    for (int value : values)
    {
        list.insert_after(tail, value);
        tail = tail->next;
    }
}

std::vector<int> values_of(flist::FLIST& list)
{
    std::vector<int> values;
    std::unordered_set<flist::ptrNODE> seen;
    flist::ptrNODE ptr{ list.get_head()->next };
    while (ptr)
    {
        require(seen.insert(ptr).second, "Cycle in list");
        require(values.size() < 1000, "Unexpected node count");
        values.push_back(ptr->info);
        ptr = ptr->next;
    }
    return values;
}

void expect(flist::FLIST& list, std::initializer_list<int> values)
{
    require(values_of(list) == std::vector<int>(values), "Unexpected list values");
}

void test_container()
{
    flist::NODE node;
    require(node.info == 0 && !node.next, "Default NODE is uninitialized");
    flist::FLIST list;
    flist::ptrNODE sentinel{ list.get_head() };
    require(list.empty() && sentinel, "Missing sentinel");
    list.sorting();
    list.push_front(2);
    list.push_front(1);
    list.insert_after(list.get_head()->next, 3);
    expect(list, { 1, 3, 2 });
    list.erase_after(list.get_head()->next);
    list.pop_front();
    expect(list, { 2 });
    list.clear(list.get_head());
    require(list.empty() && list.get_head() == sentinel, "clear removed sentinel");
    fill(list, { -2, -3, 4, 5 });
    flist::ptrNODE before{ list.find_if(sentinel, nullptr,
        [](int value) { return value % 2 != 0; }) };
    require(before == sentinel->next && before->next->info == -3,
        "find_if must return predecessor");
    require(!list.find_if(sentinel, nullptr, [](int value) { return value == 99; }),
        "find_if unexpectedly found an element");
    list.remove_if([](int value) { return value % 2 != 0; });
    expect(list, { -2, 4 });
    list.removee(-2);
    expect(list, { 4 });
    fill(list, { 1, 2, 3, 4 });
    flist::ptrNODE begin{ sentinel->next };
    flist::ptrNODE end{ begin->next->next->next };
    list.clear(begin, end);
    expect(list, { 1, 4 });
}

void test_sorting()
{
    // All lists of length 0..5 over {-1, 0, 1}, including duplicates.
    for (int length = 0, count = 1; length <= 5; ++length, count *= 3)
    {
        for (int code = 0; code < count; ++code)
        {
            flist::FLIST list;
            flist::ptrNODE sentinel{ list.get_head() }, tail{ sentinel };
            std::vector<int> expected;
            std::unordered_set<flist::ptrNODE> original;
            int digits{ code };
            for (int index = 0; index < length; ++index)
            {
                int value{ digits % 3 - 1 };
                digits /= 3;
                expected.push_back(value);
                list.insert_after(tail, value);
                tail = tail->next;
                original.insert(tail);
            }
            list.sorting();
            std::sort(expected.begin(), expected.end());
            require(values_of(list) == expected, "sorting order failed");
            require(list.get_head() == sentinel, "sorting replaced sentinel");
            for (flist::ptrNODE ptr = sentinel->next; ptr; ptr = ptr->next)
                require(original.erase(ptr) == 1, "sorting replaced a node");
            require(original.empty(), "sorting lost a node");
        }
    }
}

void test_fragments()
{
    flist::FLIST list;
    fill(list, { 1, 2, 3, 4, 5 });
    flist::ptrNODE head{ list.get_head() };
    flist::ptrNODE first{ head->next->next };
    flist::ptrNODE last{ first->next };
    flist::ptrNODE tail{ last->next->next };
    list.switch_fragment(tail, head->next, last);
    expect(list, { 1, 4, 5, 2, 3 });
    require(tail->next == first && first->next == last,
        "Fragment start or identity lost");
    list.switch_fragment(head, tail, last);
    expect(list, { 2, 3, 1, 4, 5 });
    require(head->next == first, "Transfer to front lost first node");
    list.switch_fragment(head, head, last);
    expect(list, { 2, 3, 1, 4, 5 });
    fill(list, { 1, 2, 3 });
    first = head->next;
    list.switch_fragment(first->next->next, head, first);
    expect(list, { 2, 3, 1 });
    require(head == list.get_head(), "Transfer replaced sentinel");
}

void test_recursion()
{
    flist::FLIST list;
    fill(list, { -8, -3, -5 });
    require(find_max(list.get_head()->next) == -3, "Maximum includes sentinel");
    fill(list, { 3, 1, 5, 1, 4, 1 });
    std::ostringstream reverse;
    std::streambuf* previous{ std::cout.rdbuf(reverse.rdbuf()) };
    reverse_print(list.get_head()->next);
    std::cout.rdbuf(previous);
    require(reverse.str() == "1 4 1 5 1 3 ", "Reverse order failed");
    expect(list, { 3, 1, 5, 1, 4, 1 });
    require(replace_all(list.get_head()->next, 1, 2), "Replacement flag missing");
    expect(list, { 3, 2, 5, 2, 4, 2 });
    require(!replace_all(list.get_head()->next, 99, 2), "Unexpected replacement");
    require(replace_all(list.get_head()->next, 2, 2), "Same-value match flag changed");
    require(delete_first(list, list.get_head(), 2), "Deletion failed");
    expect(list, { 3, 5, 2, 4, 2 });
    require(delete_first(list, list.get_head(), 3), "First node deletion failed");
    expect(list, { 5, 2, 4, 2 });
    require(!delete_first(list, list.get_head(), 99), "Unexpected deletion");
    require(duplicate_first(list, list.get_head()->next, 2), "First duplication failed");
    expect(list, { 5, 2, 2, 4, 2 });
    require(!duplicate_first(list, list.get_head()->next, 99), "Unexpected duplication");
    require(duplicate_all(list, list.get_head()->next, 2), "All duplication failed");
    expect(list, { 5, 2, 2, 2, 2, 4, 2, 2 });
    require(!duplicate_all(list, list.get_head()->next, 99), "Unexpected all duplication");
    fill(list, { 2, 2 });
    duplicate_all(list, list.get_head()->next, 2);
    expect(list, { 2, 2, 2, 2 });
    fill(list, { 5 });
    require(find_max(list.get_head()->next) == 5, "Single maximum failed");
    require(is_increasing(list.get_head()->next), "Single ascending failed");
    require(delete_first(list, list.get_head(), 5) && list.empty(), "Last deletion failed");
    fill(list, { -5, -1, 0, 3 });
    require(is_increasing(list.get_head()->next), "Ascending check failed");
    fill(list, { -1, -1 });
    require(!is_increasing(list.get_head()->next), "Equal values treated as strict");
    fill(list, { 3, 2 });
    require(!is_increasing(list.get_head()->next), "Descending check failed");
}

void test_reading()
{
    for (const std::string& input : { std::string(""), std::string("4 -1 4"),
        std::string("1 x"), std::string("1 2x"), std::string("2147483648"),
        std::string("1.5") })
    {
        {
            std::ofstream file("test-input.txt");
            file << input;
        }
        flist::FLIST list;
        std::ifstream file("test-input.txt");
        bool rejected{};
        try
        {
            list.create_by_queue(file);
        }
        catch (const std::runtime_error&)
        {
            rejected = true;
        }
        bool valid{ input.empty() || input == "4 -1 4" };
        require(rejected != valid, "File validation failed");
        if (valid && !input.empty())
        {
            expect(list, { 4, -1, 4 });
            file.clear();
            file.seekg(0);
            list.clear(list.get_head());
            list.create_by_queue(file);
            expect(list, { 4, -1, 4 });
        }
        else if (input.empty())
            require(list.empty(), "Empty file created data nodes");
    }
}

int main()
{
    int result{};
    try
    {
        test_container();
        test_sorting();
        test_fragments();
        test_recursion();
        test_reading();
        std::cout << "All FLIST and homework checks passed (364 sorting cases).\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        result = 1;
    }
    return result;
}
