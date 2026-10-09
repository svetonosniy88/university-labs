using System;
using System.Globalization;
using System.Text;

// Лабораторная работа №2. Вариант 4.
// Даны 2n действительных чисел, задающих n открытых интервалов.
// Определить, является ли их объединение одним интервалом.
// Если да, указать его концы.
// Выполнил Кузнецов Дмитрий Олегович, группа 91.

Console.OutputEncoding = Encoding.UTF8;

int n = read_count();
double[] a = new double[2 * n];

read_array(a);
print_array(a);
sort_intervals(a);

if (find_union(a, out double left, out double right))
    Console.WriteLine($"\nОбъединение является интервалом: ({left:G}; {right:G})");
else
    Console.WriteLine("\nОбъединение не является одним интервалом.");


static int read_count()
{
    while (true)
    {
        Console.Write("Количество интервалов: ");

        if (int.TryParse(Console.ReadLine(), out int n) && n > 0 && n <= int.MaxValue / 2)
            return n;

        Console.WriteLine("Введите положительное целое число.");
    }
}

static double read_number(string message)
{
    while (true)
    {
        Console.Write(message);
        string input = (Console.ReadLine() ?? "").Replace(',', '.');

        if (double.TryParse(input, NumberStyles.Float,
            CultureInfo.InvariantCulture, out double value) && double.IsFinite(value))
            return value;

        Console.WriteLine("Введите корректное действительное число.");
    }
}

static void read_array(double[] a)
{
    for (int i = 0; i < a.Length; i += 2)
    {
        do
        {
            a[i] = read_number($"Интервал {i / 2 + 1}, левый конец: ");
            a[i + 1] = read_number("Правый конец: ");

            if (a[i] >= a[i + 1])
                Console.WriteLine("Левый конец должен быть меньше правого.");
        }
        while (a[i] >= a[i + 1]);
    }
}

static void print_array(double[] a)
{
    Console.WriteLine("\nИсходные интервалы:");

    for (int i = 0; i < a.Length; i += 2)
        Console.WriteLine($"({a[i]:G}; {a[i + 1]:G})");
}

static void sort_intervals(double[] a)
{
    for (int i = 2; i < a.Length; i += 2)
    {
        double left = a[i];
        double right = a[i + 1];
        int j = i - 2;

        while (j >= 0 && a[j] > left)
        {
            a[j + 2] = a[j];
            a[j + 3] = a[j + 1];
            j -= 2;
        }

        a[j + 2] = left;
        a[j + 3] = right;
    }
}

static bool find_union(double[] a, out double left, out double right)
{
    left = a[0];
    right = a[1];

    for (int i = 2; i < a.Length; i += 2)
    {
        if (a[i] >= right)
            return false;

        if (a[i + 1] > right)
            right = a[i + 1];
    }

    return true;
}
