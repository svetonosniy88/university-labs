#include "Tasks.h"

#include <deque>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <vector>

static_assert(!std::is_copy_constructible_v<StackL>);
static_assert(!std::is_copy_constructible_v<StackA>);
static_assert(!std::is_copy_constructible_v<QueueL>);
static_assert(!std::is_copy_assignable_v<StackL>);
static_assert(!std::is_copy_assignable_v<StackA>);
static_assert(!std::is_copy_assignable_v<QueueL>);

int checks{};

void require(bool condition)
{
    ++checks;
    if (!condition)
        throw std::runtime_error("Check " + std::to_string(checks) + " failed.");
}

template <class Stack>
void test_stack(Stack& stack)
{
    std::vector<char> reference;
    std::mt19937 random(12);
    require(stack.empty() && stack.size() == 0);
    for (int i{}; i < 10000; ++i)
    {
        if (reference.empty() || random() % 2 == 0)
        {
            char value{ static_cast<char>('a' + random() % 26) };
            stack.push(value);
            reference.push_back(value);
        }
        else
        {
            require(stack.top() == reference.back());
            stack.pop();
            reference.pop_back();
        }
        require(stack.size() == reference.size());
        require(stack.empty() == reference.empty());
    }
    while (!reference.empty())
    {
        require(stack.top() == reference.back());
        stack.pop();
        reference.pop_back();
    }
    require(stack.empty() && stack.size() == 0);
    stack.push('x');
    require(stack.top() == 'x' && stack.size() == 1);
}

bool reference_balance(const std::string& text)
{
    std::vector<char> open;
    bool valid{ true };
    for (char c : text)
    {
        if (c == '(' || c == '[' || c == '{')
            open.push_back(c);
        else if (c == ')' || c == ']' || c == '}')
        {
            if (open.empty())
                valid = false;
            else
            {
                char expected{ c == ')' ? '(' : c == ']' ? '[' : '{' };
                valid = valid && open.back() == expected;
                open.pop_back();
            }
        }
    }
    return valid && open.empty();
}

void test_brackets()
{
    std::mt19937 random(15);
    const std::string symbols{ "()[]{}abc \t" };
    for (int i{}; i < 5000; ++i)
    {
        std::string line;
        std::size_t length{ random() % 1000 };
        for (std::size_t j{}; j < length; ++j)
            line += symbols[random() % symbols.size()];
        StackL list;
        StackA array(line.size());
        bool expected{ reference_balance(line) };
        require(balanced(line, list) == expected);
        require(balanced(line, array) == expected);
    }
    for (int depth : { 0, 1, 99, 100, 101, 10000 })
    {
        std::string text(static_cast<std::size_t>(depth), '(');
        text += std::string(static_cast<std::size_t>(depth), ')');
        StackL list;
        StackA array(text.size());
        require(balanced(text, list));
        require(balanced(text, array));
    }
}

void test_queue()
{
    QueueL queue;
    std::deque<std::string> reference;
    std::mt19937 random(27);
    for (int i{}; i < 10000; ++i)
    {
        if (reference.empty() || random() % 2 == 0)
        {
            std::string value{ std::to_string(random()) };
            queue.push(value);
            reference.push_back(value);
        }
        else
        {
            require(queue.front() == reference.front());
            queue.pop();
            reference.pop_front();
        }
        require(queue.empty() == reference.empty());
        require(queue.size() == reference.size());
        if (!queue.empty())
        {
            require(queue.front() == reference.front());
            require(queue.back() == reference.back());
        }
    }
    while (!queue.empty())
        queue.pop();
    for (int i{}; i < 100; ++i)
    {
        queue.push("one");
        require(queue.front() == "one" && queue.back() == "one");
        queue.pop();
        require(queue.empty() && queue.size() == 0);
    }
    queue.push("");
    require(!queue.empty() && queue.front().empty());
}

void test_words(const std::string& text, const std::vector<std::string>& expected,
    std::size_t maximum)
{
    std::istringstream input(text);
    QueueL words;
    words.push("old contents must be cleared");
    require(task12(input, words) == maximum);
    require(words.size() == expected.size());
    for (const std::string& word : expected)
    {
        require(!words.empty());
        require(words.front() == word);
        words.pop();
    }
    require(words.empty());
}

int main()
{
    StackL list;
    StackA array(10000);
    test_stack(list);
    test_stack(array);
    test_queue();
    test_brackets();
    test_words("", {}, 0);
    test_words(" \n \t\n", {}, 0);
    test_words("  aa bbbb\ncccc  a bbbb", { "bbbb", "cccc", "bbbb" }, 4);
    test_words("a bb ccc dddd", { "dddd" }, 4);
    test_words("dddd ccc bb a", { "dddd" }, 4);
    test_words("notebook computer\nnotebook", { "notebook", "computer", "notebook" }, 8);
    test_words("\xEF\xBB\xBF" "aa bbb aa", { "bbb" }, 3);
    test_words("\xEF\xBB\xBF \n", {}, 0);
    test_words("кот cats\nслон dogs", { "cats", "слон", "dogs" }, 4);
    std::string long_word(20000, 'x');
    test_words("short " + long_word + "\n" + long_word, { long_word, long_word }, 20000);

    std::ostringstream messages;
    std::streambuf* previous{ std::cout.rdbuf(messages.rdbuf()) };
    {
        StackL empty_list;
        StackA empty_array(0);
        QueueL empty_queue;
        require(empty_list.top() == '\0');
        empty_list.pop();
        require(empty_array.empty() && empty_array.full());
        empty_array.push('a');
        empty_array.pop();
        require(empty_array.top() == '\0');
        require(empty_queue.front().empty() && empty_queue.back().empty());
        empty_queue.pop();
        StackA one(1);
        one.push('a');
        one.push('b');
        require(one.size() == 1 && one.top() == 'a' && one.full());
        one.pop();
        one.push('c');
        require(one.top() == 'c');
    }
    std::cout.rdbuf(previous);
    require(!messages.str().empty());

    flist::FLIST old_list;
    require(!task15(old_list));
    std::istringstream numbers("8 3 2 7 1 6 5 4 -3 -5 -2");
    std::ofstream temp("old-list.txt");
    temp << numbers.str();
    temp.close();
    std::ifstream old_input("old-list.txt");
    old_list.create_by_queue(old_input);
    old_list.sorting();
    require(task15(old_list));
    std::vector<int> actual;
    for (flist::ptrNODE p{ old_list.get_head()->next }; p; p = p->next)
        actual.push_back(p->info);
    require(actual == std::vector<int>({ -2, 1, 2, 3, 4, 5, 6, 7, 8, -5, -3 }));
    std::cout << "Passed " << checks << " checks.\n";
    return 0;
}
