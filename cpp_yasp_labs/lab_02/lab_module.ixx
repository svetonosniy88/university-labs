module;

#include <cstdint>
#include <cstddef>
#include <charconv>
#include <limits>
#include <stdexcept>

export module lab_module;

/*
 * Лабораторная работа № 2, вариант 4.
 * Класс Fraction: числитель со знаком, беззнаковый знаменатель;
 * арифметика, рациональная степень, сравнения, инкремент.
 * Кузнецов Дмитрий Олегович, группа 91.
 */

export class Fraction;

export Fraction operator+(const Fraction& a, const Fraction& b);
export Fraction operator-(const Fraction& a, const Fraction& b);
export Fraction operator*(const Fraction& a, const Fraction& b);
export Fraction operator/(const Fraction& a, const Fraction& b);

export bool operator==(const Fraction& a, const Fraction& b);
export bool operator!=(const Fraction& a, const Fraction& b);
export bool operator<(const Fraction& a, const Fraction& b);
export bool operator>(const Fraction& a, const Fraction& b);
export bool operator<=(const Fraction& a, const Fraction& b);
export bool operator>=(const Fraction& a, const Fraction& b);

export class Fraction
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

// Возвращает беззнаковый модуль числа, включая минимальное значение int32_t.
std::uint32_t magnitude(std::int32_t value)
{
    if (value < 0)
        return static_cast<std::uint32_t>(-(value + 1)) + 1;

    return static_cast<std::uint32_t>(value);
}

// Находит наибольший общий делитель двух чисел алгоритмом Евклида.
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

// Умножает два беззнаковых числа после проверки на переполнение.
std::uint32_t multiply(std::uint32_t a, std::uint32_t b)
{
    if (b != 0 && a > std::numeric_limits<std::uint32_t>::max() / b)
        throw std::overflow_error("Intermediate product exceeds 32 bits.");

    return a * b;
}

// Сокращает дробь, проверяет диапазон числителя и восстанавливает его знак.
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

    constexpr std::uint32_t positive_max{ static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) };

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

// Создаёт сокращённую дробь с положительным знаменателем.
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

// Складывает или вычитает дроби с учётом знаков и сокращения знаменателей.
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

// Возвращает сумму двух дробей.
Fraction operator+(const Fraction& a, const Fraction& b)
{
    return Fraction::add(a, b, false);
}

// Возвращает разность двух дробей.
Fraction operator-(const Fraction& a, const Fraction& b)
{
    return Fraction::add(a, b, true);
}

// Перемножает дроби, предварительно сокращая числители со знаменателями.
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

// Делит первую дробь на вторую с проверкой деления на ноль и сокращением.
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

// Проверяет равенство числителей и знаменателей уже сокращённых дробей.
bool operator==(const Fraction& a, const Fraction& b)
{
    return a.numerator == b.numerator && a.denominator == b.denominator;
}

// Проверяет неравенство дробей через отрицание результата сравнения на равенство.
bool operator!=(const Fraction& a, const Fraction& b)
{
    return !(a == b);
}

// Сравнивает знаки, целые части и остатки дробей без перекрёстного умножения.
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

// Проверяет, больше ли первая дробь второй, меняя порядок сравнения.
bool operator>(const Fraction& a, const Fraction& b)
{
    return b < a;
}

// Проверяет, что первая дробь не больше второй.
bool operator<=(const Fraction& a, const Fraction& b)
{
    return !(b < a);
}

// Проверяет, что первая дробь не меньше второй.
bool operator>=(const Fraction& a, const Fraction& b)
{
    return !(a < b);
}

// Ищет точный целый корень двоичным поиском; неточный корень вызывает ошибку.
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

// Возводит дробь в рациональную степень через точные корни и быстрое целочисленное возведение.
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

// Увеличивает дробь на единицу и возвращает ссылку на изменённый объект.
Fraction& Fraction::operator++()
{
    *this = *this + Fraction{ 1 };

    return *this;
}

// Сохраняет прежнюю дробь, увеличивает объект на единицу и возвращает прежнее значение.
Fraction Fraction::operator++(int)
{
    Fraction previous{ *this };

    ++(*this);

    return previous;
}

// Записывает дробь в символьный буфер; целые числа выводит без знаменателя.
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

