module;
#include <charconv>
#include <cstdint>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

export module lab_queue;

// ЯСП, лабораторная № 3, вариант 4B. Кузнецов Дмитрий Олегович, группа 91.
// Реализовать шаблон очереди на указателях: добавление, просмотр и извлечение
// головы, длина, поиск от головы, загрузка, печать и сохранение без очистки.

export struct Student
{
private:
    std::uint32_t record_number{};
    std::string name;
    std::string group;

public:
    Student() = default;
    Student(std::uint32_t number, const std::string& full_name, const std::string& group_name)
        : record_number{ number }, name{ full_name }, group{ group_name } {}

    friend bool operator==(const Student&, const Student&) = default;

    friend std::ostream& operator<<(std::ostream& out, const Student& value)
    {
        return out << value.record_number << '|' << value.name << '|' << value.group;
    }
};

// Читает всё поле, включая проверку диапазона целевого целого типа.
template<class Integer>
bool parse_integer(std::string_view text, Integer& value)
{
    std::size_t first{ text.find_first_not_of(" \t") };
    if (first == text.npos)
        return false;
    text = text.substr(first, text.find_last_not_of(" \t") - first + 1);
    if (text.front() == '+')
    {
        text.remove_prefix(1);
        if (text.empty() || text.front() < '0' || text.front() > '9')
            return false;
    }
    const char* end{ text.data() + text.size() };
    std::from_chars_result result{ std::from_chars(text.data(), end, value) };
    return result.ec == std::errc{} && result.ptr == end;
}

export bool parse_record(const std::string& line, std::int32_t& value)
{
    return parse_integer(std::string_view{ line }, value);
}

// Для строк пробелы значимы. Управляющие символы не входят в текст записи.
bool printable_text(const std::string& text)
{
    for (unsigned char ch : text)
        if (ch < 32 || ch == 127)
            return false;
    return true;
}

export bool parse_record(const std::string& line, std::string& value)
{
    if (!printable_text(line))
        return false;
    value = line;
    return true;
}

export bool parse_record(const std::string& line, Student& value)
{
    std::size_t first{ line.find('|') };
    if (first == line.npos || !printable_text(line))
        return false;
    std::size_t second{ line.find('|', first + 1) };
    if (second == line.npos || line.find('|', second + 1) != line.npos)
        return false;

    std::uint32_t number{};
    std::string name{ line.substr(first + 1, second - first - 1) };
    std::string group{ line.substr(second + 1) };
    if (!parse_integer(std::string_view{ line }.substr(0, first), number) || number == 0 ||
        name.find_first_not_of(' ') == name.npos || group.find_first_not_of(' ') == group.npos)
        return false;
    value = Student{ number, name, group };
    return true;
}

export template<class X>
class Queue
{
    struct Node
    {
        X value;
        Node* next;
    };
    Node* head{};
    Node* tail{};
    std::uint32_t count{};

public:
    Queue() = default;
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    ~Queue()
    {
        while (head)
        {
            Node* next{ head->next };
            delete head;
            head = next;
        }
    }

    // Первый узел одновременно становится головой и хвостом.
    void push(const X& value)
    {
        if (count == std::numeric_limits<std::uint32_t>::max())
            throw std::length_error("Queue is full.");
        Node* node{ new Node{ value, nullptr } };
        if (tail)
            tail->next = node;
        else
            head = node;
        tail = node;
        ++count;
    }

    const X& peek() const
    {
        if (!head)
            throw std::out_of_range("Queue is empty.");
        return head->value;
    }

    X pop()
    {
        X value{ peek() };
        Node* previous{ head };
        head = head->next;
        delete previous;
        if (!head)
            tail = nullptr;
        --count;
        return value;
    }

    std::uint32_t size() const { return count; }

    // Позиции начинаются с единицы. Ноль означает отсутствие совпадения.
    std::uint32_t find(const X& value) const
    {
        std::uint32_t position{};
        for (const Node* node{ head }; node; node = node->next)
        {
            ++position;
            if (node->value == value)
                return position;
        }
        return 0;
    }

    void print(std::ostream& out) const
    {
        for (const Node* node{ head }; node; node = node->next)
            out << node->value << '\n';
    }

    // Заменяет очередь после проверки всех записей, сохраняя порядок строк.
    void load(std::istream& in)
    {
        Queue replacement;
        std::string line;
        while (std::getline(in, line))
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            X value{};
            if (!parse_record(line, value))
                throw std::invalid_argument("Invalid record on line " + std::to_string(replacement.size() + 1) + ".");
            replacement.push(value);
        }
        if (in.bad() || !in.eof())
            throw std::runtime_error("Input file read error.");
        std::swap(head, replacement.head);
        std::swap(tail, replacement.tail);
        std::swap(count, replacement.count);
    }
};
