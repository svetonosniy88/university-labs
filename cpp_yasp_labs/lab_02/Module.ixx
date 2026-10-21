module;

#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstddef>
#include <cctype>
#include <charconv>
#include <cstring>
#include <limits>
#include <stdexcept>

export module lab_module;

/*
    Лабораторная работа № 2, вариант 4.
    Класс Fraction: числитель со знаком, беззнаковый знаменатель;
    сложение, вычитание, умножение, деление, натуральная степень, сравнения.
    Кузнецов Дмитрий Олегович, группа 91.
*/

export
{
    class Fraction;

    Fraction operator+(const Fraction& a, const Fraction& b);
    Fraction operator-(const Fraction& a, const Fraction& b);
    Fraction operator*(const Fraction& a, const Fraction& b);
    Fraction operator/(const Fraction& a, const Fraction& b);

    bool operator==(const Fraction& a, const Fraction& b);
    bool operator!=(const Fraction& a, const Fraction& b);
    bool operator<(const Fraction& a, const Fraction& b);
    bool operator>(const Fraction& a, const Fraction& b);
    bool operator<=(const Fraction& a, const Fraction& b);
    bool operator>=(const Fraction& a, const Fraction& b);

    class Fraction
    {
        std::int32_t numerator;
        std::uint32_t denominator;

        static Fraction from_parts(
            std::uint32_t n,
            std::uint32_t d,
            bool negative
        );

        static Fraction add(
            const Fraction& a,
            const Fraction& b,
            bool subtract
        );

        friend Fraction operator+(const Fraction& a, const Fraction& b);
        friend Fraction operator-(const Fraction& a, const Fraction& b);
        friend Fraction operator*(const Fraction& a, const Fraction& b);
        friend Fraction operator/(const Fraction& a, const Fraction& b);
        friend bool operator==(const Fraction& a, const Fraction& b);
        friend bool operator<(const Fraction& a, const Fraction& b);

    public:
        Fraction(std::int32_t n = 0, std::uint32_t d = 1);

        Fraction power(std::uint32_t exponent) const;

        Fraction& operator++();
        Fraction operator++(int);

        void to_string(char* text, std::size_t capacity) const;
        void write(std::ostream& output) const;
    };
}

// Модуль INT32_MIN тоже помещается в uint32_t. Сам INT32_MIN не отрицаем.
std::uint32_t magnitude(std::int32_t value)
{
    if (value < 0)
        return static_cast<std::uint32_t>(-(value + 1)) + 1;

    return static_cast<std::uint32_t>(value);
}

std::uint32_t gcd(std::uint32_t a, std::uint32_t b)
{
    while (b != 0)
    {
        std::uint32_t remainder{ a % b };

        a = b;
        b = remainder;
    }

    return a;
}

std::uint32_t multiply(std::uint32_t a, std::uint32_t b)
{
    if (b != 0 && a > std::numeric_limits<std::uint32_t>::max() / b)
        throw std::overflow_error("Intermediate product exceeds 32 bits.");

    return a * b;
}

Fraction Fraction::from_parts(
    std::uint32_t n,
    std::uint32_t d,
    bool negative
)
{
    if (d == 0)
        throw std::invalid_argument("Denominator must be positive.");

    std::uint32_t divisor{ gcd(n, d) };

    n /= divisor;
    d /= divisor;

    const std::uint32_t positive_max{ static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) };

    if (n > positive_max + (negative ? 1U : 0U))
        throw std::overflow_error("Result numerator exceeds int32_t.");

    Fraction result;
    result.denominator = d;

    if (negative && n == positive_max + 1U)
        result.numerator = std::numeric_limits<std::int32_t>::min();
    else
        result.numerator = negative ? -static_cast<std::int32_t>(n) : static_cast<std::int32_t>(n);

    return result;
}

Fraction::Fraction(std::int32_t n, std::uint32_t d)
    : numerator{ 0 }, denominator{ 1 }
{
    if (d == 0)
        throw std::invalid_argument("Denominator must be positive.");

    // Делим модули, чтобы не приводить большой беззнаковый НОД к int32_t.
    if (n != 0)
    {
        std::uint32_t divisor{ gcd(magnitude(n), d) };
        std::uint32_t reduced{ magnitude(n) / divisor };

        denominator = d / divisor;

        if (n == std::numeric_limits<std::int32_t>::min() && divisor == 1)
            numerator = n;
        else
            numerator = n < 0 ? -static_cast<std::int32_t>(reduced) : static_cast<std::int32_t>(reduced);
    }
}

Fraction Fraction::add(
    const Fraction& a,
    const Fraction& b,
    bool subtract
)
{
    std::uint32_t common{ gcd(a.denominator, b.denominator) };
    std::uint32_t left{ multiply(magnitude(a.numerator), b.denominator / common) };
    std::uint32_t right{ multiply(magnitude(b.numerator), a.denominator / common) };

    bool left_negative{ a.numerator < 0 };
    bool right_negative{ (b.numerator < 0) != subtract };

    std::uint32_t n{};
    bool negative{ left_negative };

    if (left_negative == right_negative)
    {
        if (left > std::numeric_limits<std::uint32_t>::max() - right)
            throw std::overflow_error("Intermediate sum exceeds 32 bits.");

        n = left + right;
    }
    else if (left >= right)
        n = left - right;
    else
    {
        n = right - left;
        negative = right_negative;
    }

    if (n == 0)
        return Fraction{};

    // Сначала сокращаем общий множитель, затем считаем знаменатель.
    std::uint32_t divisor{ gcd(n, common) };

    return from_parts(
        n / divisor,
        multiply(a.denominator / common, b.denominator / divisor),
        negative
    );
}

Fraction operator+(const Fraction& a, const Fraction& b)
{
    return Fraction::add(a, b, false);
}

Fraction operator-(const Fraction& a, const Fraction& b)
{
    return Fraction::add(a, b, true);
}

Fraction operator*(const Fraction& a, const Fraction& b)
{
    if (a.numerator == 0 || b.numerator == 0)
        return Fraction{};

    std::uint32_t first{ gcd(magnitude(a.numerator), b.denominator) };
    std::uint32_t second{ gcd(magnitude(b.numerator), a.denominator) };

    return Fraction::from_parts(
        multiply(magnitude(a.numerator) / first, magnitude(b.numerator) / second),
        multiply(a.denominator / second, b.denominator / first),
        (a.numerator < 0) != (b.numerator < 0)
    );
}

Fraction operator/(const Fraction& a, const Fraction& b)
{
    if (b.numerator == 0)
        throw std::domain_error("Cannot divide by zero.");

    if (a.numerator == 0)
        return Fraction{};

    std::uint32_t first{ gcd(magnitude(a.numerator), magnitude(b.numerator)) };
    std::uint32_t second{ gcd(a.denominator, b.denominator) };

    return Fraction::from_parts(
        multiply(magnitude(a.numerator) / first, b.denominator / second),
        multiply(a.denominator / second, magnitude(b.numerator) / first),
        (a.numerator < 0) != (b.numerator < 0)
    );
}

bool operator==(const Fraction& a, const Fraction& b)
{
    return a.numerator == b.numerator && a.denominator == b.denominator;
}

bool operator!=(const Fraction& a, const Fraction& b)
{
    return !(a == b);
}

// Сравниваем целые части; при их равенстве переходим к обратным остаткам.
// Это точное сравнение без опасного перемножения крест-накрест.
bool operator<(const Fraction& a, const Fraction& b)
{
    if ((a.numerator < 0) != (b.numerator < 0))
        return a.numerator < 0;

    std::uint32_t n1{ magnitude(a.numerator) }, d1{ a.denominator };
    std::uint32_t n2{ magnitude(b.numerator) }, d2{ b.denominator };
    bool reverse{ a.numerator < 0 };

    while (true)
    {
        if (n1 / d1 != n2 / d2)
            return reverse ? n1 / d1 > n2 / d2 : n1 / d1 < n2 / d2;

        std::uint32_t r1{ n1 % d1 }, r2{ n2 % d2 };

        if (r1 == 0 || r2 == 0)
            return reverse ? r1 > r2 : r1 < r2;

        n1 = d1;
        d1 = r1;

        n2 = d2;
        d2 = r2;

        reverse = !reverse;
    }
}

bool operator>(const Fraction& a, const Fraction& b)
{
    return b < a;
}

bool operator<=(const Fraction& a, const Fraction& b)
{
    return !(b < a);
}

bool operator>=(const Fraction& a, const Fraction& b)
{
    return !(a < b);
}

Fraction Fraction::power(std::uint32_t exponent) const
{
    if (exponent == 0)
        throw std::invalid_argument("Natural exponent must be at least 1.");

    Fraction result{ 1 }, factor{ *this };

    while (exponent != 0)
    {
        if (exponent % 2 != 0)
            result = result * factor;

        exponent /= 2;

        if (exponent != 0)
            factor = factor * factor;
    }

    return result;
}

Fraction& Fraction::operator++()
{
    *this = *this + Fraction{ 1 };

    return *this;
}

Fraction Fraction::operator++(int)
{
    Fraction previous{ *this };

    ++(*this);

    return previous;
}

void Fraction::to_string(char* text, std::size_t capacity) const
{
    if (capacity == 0)
        throw std::length_error("Text buffer is too small.");

    char* end{ text + capacity - 1 }; // Место для завершающего нуля.
    std::to_chars_result first{ std::to_chars(text, end, numerator) };

    if (first.ec != std::errc{} || first.ptr == end)
        throw std::length_error("Text buffer is too small.");

    *first.ptr = '/';

    std::to_chars_result second{ std::to_chars(first.ptr + 1, end, denominator) };

    if (second.ec != std::errc{})
        throw std::length_error("Text buffer is too small.");

    *second.ptr = '\0';
}

void Fraction::write(std::ostream& output) const
{
    output << numerator << ' ' << denominator << '\n';
}

// Общий построчный ввод для консоли и файла. Длинную строку удаляем целиком.
bool read_line(
    std::istream& input,
    char* text,
    std::streamsize capacity
)
{
    if (input.getline(text, capacity))
    {
        std::streamsize length{ input.gcount() };

        if (!input.eof())
            --length; // getline учитывает извлечённый перевод строки.

        if (static_cast<std::streamsize>(std::strlen(text)) != length)
            throw std::invalid_argument("Input line contains a zero byte.");

        return true;
    }

    if (input.bad())
        throw std::runtime_error("Input read error.");

    if (input.eof() && input.gcount() == 0)
        return false;

    input.clear();
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    throw std::invalid_argument("Input line is too long.");
}

const char* skip_spaces(const char* text)
{
    while (std::isspace(static_cast<unsigned char>(*text)))
        ++text;

    return text;
}

bool read_fraction(std::istream& input, Fraction& value)
{
    char text[128]{};

    if (!read_line(input, text, sizeof(text)))
        return false;

    const char* start{ skip_spaces(text) };
    const char* end{ text + std::strlen(text) };

    if (*start == '+' && start[1] >= '0' && start[1] <= '9')
        ++start;

    std::int32_t n{};
    std::from_chars_result first{ std::from_chars(start, end, n) };

    if (first.ec != std::errc{} || !std::isspace(static_cast<unsigned char>(*first.ptr)))
        throw std::invalid_argument("Enter numerator and positive denominator, separated by a space.");

    start = skip_spaces(first.ptr);

    if (*start == '+' && start[1] >= '0' && start[1] <= '9')
        ++start;

    std::uint32_t d{};
    std::from_chars_result second{ std::from_chars(start, end, d) };

    if (second.ec != std::errc{} || *skip_spaces(second.ptr) != '\0')
        throw std::invalid_argument("Invalid denominator or extra characters.");

    value = Fraction{ n, d };
    return true;
}

export bool read_number(const char* prompt, std::uint32_t& value)
{
    while (true)
    {
        std::cout << prompt;
        char text[128]{};

        try
        {
            if (!read_line(std::cin, text, sizeof(text)))
                return false;

            const char* start{ skip_spaces(text) };

            if (*start == '+' && start[1] >= '0' && start[1] <= '9')
                ++start;

            std::from_chars_result parsed{ std::from_chars(start, text + std::strlen(text), value) };

            if (parsed.ec == std::errc{} && *skip_spaces(parsed.ptr) == '\0')
                return true;

            std::cout << "Enter a whole number from 0 to 4294967295.\n";
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << error.what() << '\n';
        }
    }
}

export bool read_from_console(Fraction& a, Fraction& b)
{
    Fraction values[2];

    for (std::uint32_t i{}; i < 2; ++i)
    {
        while (true)
        {
            std::cout << (i == 0 ? "A" : "B") << " (numerator denominator): ";

            try
            {
                if (!read_fraction(std::cin, values[i]))
                    return false;

                break;
            }
            catch (const std::invalid_argument& error)
            {
                std::cout << error.what() << '\n';
            }
        }
    }
    a = values[0];
    b = values[1];

    return true;
}

export bool read_filename(char* name, std::streamsize capacity)
{
    while (true)
    {
        std::cout << "File name (or full path): ";

        try
        {
            if (!read_line(std::cin, name, capacity))
                return false;

            std::size_t length{ std::strlen(name) };

            if (length >= 2 && name[0] == '"' && name[length - 1] == '"')
            {
                std::memmove(name, name + 1, length - 2);
                name[length - 2] = '\0';
            }

            if (*skip_spaces(name) != '\0')
                return true;

            std::cout << "File name cannot be empty.\n";
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << error.what() << '\n';
        }
    }
}

export void load_from_file(
    Fraction& a,
    Fraction& b,
    const char* name
)
{
    std::ifstream file{ name };

    if (!file.is_open())
        throw std::runtime_error("Cannot open input file.");

    Fraction first, second;

    if (!read_fraction(file, first) || !read_fraction(file, second))
        throw std::invalid_argument("File must contain two fractions on separate lines.");

    file >> std::ws;

    if (file.bad())
        throw std::runtime_error("Input file read error.");

    if (file.peek() != std::char_traits<char>::eof())
        throw std::invalid_argument("Extra data after the two fractions.");

    a = first;
    b = second;
}

export void save_to_file(
    const Fraction& a,
    const Fraction& b,
    const char* name,
    bool result_only = false
)
{
    std::ofstream file{ name, std::ios::trunc };

    if (!file.is_open())
        throw std::runtime_error("Cannot open output file.");

    a.write(file);

    if (!result_only)
        b.write(file);

    file.close();

    if (!file)
        throw std::runtime_error("Output file write error.");
}

export void print_fraction(const char* label, const Fraction& value)
{
    char text[32]{};

    value.to_string(text, sizeof(text));

    std::cout << label << text << '\n';
}
