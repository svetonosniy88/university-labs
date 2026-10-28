module;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
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
    сложение, вычитание, умножение, деление, степень, сравнения.
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

        Fraction power(const Fraction& exponent) const;

        Fraction& operator++();
        Fraction operator++(int);

        void to_string(char* text, std::size_t capacity) const;
    };
}

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

std::uint32_t exact_root(std::uint32_t value, std::uint32_t degree)
{
    if (value < 2 || degree == 1)
        return value;

    if (degree >= 32)
        throw std::domain_error("The result is irrational and is not supported.");

    std::uint32_t low{ 1 }, high{ value };

    while (low <= high)
    {
        std::uint32_t middle{ low + (high - low) / 2 };
        std::uint32_t product{ 1 };
        bool exceeds{ false };

        for (std::uint32_t i{}; i < degree; ++i)
        {
            if (product > value / middle)
            {
                exceeds = true;
                break;
            }

            product *= middle;
        }

        if (!exceeds && product == value)
            return middle;

        if (exceeds)
            high = middle - 1;
        else
            low = middle + 1;
    }

    throw std::domain_error("The result is irrational and is not supported.");
}

Fraction Fraction::power(const Fraction& exponent) const
{
    if (exponent.numerator == 0)
    {
        if (numerator == 0)
            throw std::domain_error("Zero to the power of zero is undefined.");

        return Fraction{ 1 };
    }

    if (numerator == 0)
    {
        if (exponent.numerator < 0)
            throw std::domain_error("Zero cannot have a negative exponent.");

        return Fraction{};
    }

    if (numerator < 0 && exponent.denominator % 2 == 0)
        throw std::domain_error("This power of a negative number has no real result.");

    std::uint32_t n{ exact_root(magnitude(numerator), exponent.denominator) };
    std::uint32_t d{ exact_root(denominator, exponent.denominator) };
    bool negative{ numerator < 0 && magnitude(exponent.numerator) % 2 != 0 };

    if (exponent.numerator < 0)
    {
        std::uint32_t previous{ n };
        n = d;
        d = previous;
    }

    Fraction result{ 1 }, factor{ from_parts(n, d, negative) };
    std::uint32_t remaining{ magnitude(exponent.numerator) };

    while (remaining != 0)
    {
        if (remaining % 2 != 0)
            result = result * factor;

        remaining /= 2;

        if (remaining != 0)
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

    char* end{ text + capacity - 1 };
    std::to_chars_result first{ std::to_chars(text, end, numerator) };

    if (first.ec != std::errc{})
        throw std::length_error("Text buffer is too small.");

    if (denominator == 1)
    {
        *first.ptr = '\0';
        return;
    }

    if (first.ptr == end)
        throw std::length_error("Text buffer is too small.");

    *first.ptr = '/';

    std::to_chars_result second{ std::to_chars(first.ptr + 1, end, denominator) };

    if (second.ec != std::errc{})
        throw std::length_error("Text buffer is too small.");

    *second.ptr = '\0';
}

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
            --length;

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
    const char* format_error{ "Enter an integer, n/d or n d with a positive denominator." };

    if (!read_line(input, text, sizeof(text)))
        return false;

    const char* start{ skip_spaces(text) };
    const char* end{ text + std::strlen(text) };

    if (*start == '+' && start[1] >= '0' && start[1] <= '9')
        ++start;

    std::int32_t n{};
    std::from_chars_result first{ std::from_chars(start, end, n) };

    if (first.ec != std::errc{})
        throw std::invalid_argument(format_error);

    start = skip_spaces(first.ptr);

    if (*start == '\0')
    {
        value = Fraction{ n };
        return true;
    }

    if (*start == '/')
        start = skip_spaces(start + 1);
    else if (start == first.ptr)
        throw std::invalid_argument(format_error);

    if (*start == '+' && start[1] >= '0' && start[1] <= '9')
        ++start;

    std::uint32_t d{};
    std::from_chars_result second{ std::from_chars(start, end, d) };

    if (second.ec != std::errc{} || d == 0 || *skip_spaces(second.ptr) != '\0')
        throw std::invalid_argument(format_error);

    value = Fraction{ n, d };
    return true;
}

export bool read_choice(
    const char* prompt,
    std::uint32_t& value,
    std::uint32_t minimum,
    std::uint32_t maximum
)
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

            if (*start >= '0' && *start <= '9' && *skip_spaces(start + 1) == '\0')
            {
                std::uint32_t selected{ static_cast<std::uint32_t>(*start - '0') };

                if (selected >= minimum && selected <= maximum)
                {
                    value = selected;
                    return true;
                }
            }
        }
        catch (const std::invalid_argument&)
        {
        }

        std::cout << "Enter one integer menu number from " << minimum << " to " << maximum << ".\n";
    }
}

export bool read_fraction_from_console(const char* prompt, Fraction& value)
{
    while (true)
    {
        std::cout << prompt;

        try
        {
            return read_fraction(std::cin, value);
        }
        catch (const std::invalid_argument& error)
        {
            std::cout << error.what() << '\n';
        }
    }
}

export bool read_from_console(Fraction& a, Fraction& b)
{
    Fraction first, second;

    if (!read_fraction_from_console("A (integer, n/d or n d): ", first))
        return false;

    if (!read_fraction_from_console("B (integer, n/d or n d): ", second))
        return false;

    a = first;
    b = second;

    return true;
}

void validate_component(const char* name, std::size_t length)
{
    if (length == 0 || (length == 1 && name[0] == '.') ||
        (length == 2 && name[0] == '.' && name[1] == '.'))
        return;

    if (name[length - 1] == ' ' || name[length - 1] == '.')
        throw std::invalid_argument("File and folder names cannot end with a space or dot.");

    std::size_t stem_length{};

    while (stem_length < length && name[stem_length] != '.')
        ++stem_length;

    while (stem_length != 0 && name[stem_length - 1] == ' ')
        --stem_length;

    if (stem_length > 7)
        return;

    char stem[8]{};

    for (std::size_t i{}; i < stem_length; ++i)
        stem[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[i])));

    if (std::strcmp(stem, "CON") == 0 || std::strcmp(stem, "PRN") == 0 ||
        std::strcmp(stem, "AUX") == 0 || std::strcmp(stem, "NUL") == 0 ||
        std::strcmp(stem, "CONIN$") == 0 || std::strcmp(stem, "CONOUT$") == 0 ||
        (stem_length == 4 && stem[3] >= '1' && stem[3] <= '9' &&
            (std::strncmp(stem, "COM", 3) == 0 || std::strncmp(stem, "LPT", 3) == 0)))
        throw std::invalid_argument("Reserved Windows device names are not allowed.");
}

void validate_filename(const char* name, bool for_output)
{
    std::size_t length{ std::strlen(name) };

    if (length == 0)
        throw std::invalid_argument("File name cannot be empty.");

    if (for_output && (std::strcmp(name, ".") == 0 || std::strcmp(name, "..") == 0))
        throw std::invalid_argument("Enter a file name, not a folder.");

    const char* component{ name };

    for (std::size_t i{}; i <= length; ++i)
    {
        unsigned char symbol{ static_cast<unsigned char>(name[i]) };

        if (symbol == ':' && !for_output && i == 1 &&
            std::isalpha(static_cast<unsigned char>(name[0])))
        {
            component = name + 2;
            continue;
        }

        if ((symbol != 0 && symbol < 32) ||
            (symbol != 0 && std::strchr("<>:\"|?*", symbol) != nullptr))
            throw std::invalid_argument("File name contains a forbidden character.");

        if (symbol == '/' || symbol == '\\')
        {
            if (for_output)
                throw std::invalid_argument("For saving, enter a file name without a path.");
        }

        if (symbol == 0 || symbol == '/' || symbol == '\\')
        {
            validate_component(component, static_cast<std::size_t>(name + i - component));
            component = name + i + 1;
        }
    }
}

export bool read_filename(char* name, std::streamsize capacity, bool for_output)
{
    while (true)
    {
        std::cout << (for_output ? "Output file name (saved beside the program): " : "Input file name or path: ");

        try
        {
            if (!read_line(std::cin, name, capacity))
                return false;

            const char* start{ skip_spaces(name) };
            std::size_t length{ std::strlen(start) };

            while (length != 0 && std::isspace(static_cast<unsigned char>(start[length - 1])))
                --length;

            std::memmove(name, start, length);
            name[length] = '\0';

            if (length >= 2 && name[0] == '"' && name[length - 1] == '"')
            {
                std::memmove(name, name + 1, length - 2);
                name[length - 2] = '\0';
            }

            validate_filename(name, for_output);

            return true;
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
    validate_filename(name, false);

    if (!std::filesystem::is_regular_file(name))
        throw std::runtime_error("Input must be an existing regular file, not a folder or device.");

    std::ifstream file{ name };

    if (!file.is_open())
        throw std::runtime_error("Cannot open input file.");

    Fraction first, second;

    if (!read_fraction(file, first) || !read_fraction(file, second))
        throw std::invalid_argument("File must contain two fractions on separate lines.");

    file >> std::ws;

    if (file.bad())
        throw std::runtime_error("Input file read error.");

    if (!file.eof())
        throw std::invalid_argument("Extra data after the two fractions.");

    a = first;
    b = second;
}

export void save_to_file(
    const char* report,
    const char* name
)
{
    validate_filename(name, true);

    wchar_t executable[1024]{};
    DWORD length{ GetModuleFileNameW(nullptr, executable, 1024) };

    if (length == 0 || length >= 1024)
        throw std::runtime_error("Cannot determine the program folder.");

    std::filesystem::path path{ std::filesystem::path{ executable }.parent_path() / name };

    if (std::filesystem::is_symlink(std::filesystem::symlink_status(path)))
        throw std::runtime_error("Saving through a symbolic link is not allowed.");

    std::ofstream file{ path, std::ios::trunc };

    if (!file.is_open())
        throw std::runtime_error("Cannot open output file.");

    file << report;

    file.close();

    if (!file)
        throw std::runtime_error("Output file write error.");

    std::cout << "Last successful report saved to " << path << " (previous contents replaced).\n";
}
