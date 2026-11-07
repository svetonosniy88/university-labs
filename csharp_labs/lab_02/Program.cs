using System;
using System.Text;

/*
 * Лабораторная работа №2. Вариант 4.
 *
 * Даны действительные числа a1, a2, ..., a2n.
 * Эти точки определяют n интервалов числовой оси:
 * (a1, a2), (a3, a4), ..., (a2n-1, a2n).
 * Является ли интервалом объединение этих интервалов?
 * Если да, то указать концы этого интервала.
 *
 * Выполнил Кузнецов Дмитрий Олегович, группа 91.
 */

class Program
{
    static int read_count()
    {
        while (true)
        {
            Console.Write("Введите количество интервалов: ");

            if (int.TryParse(Console.ReadLine(), out int n)
                && n > 0 && n <= int.MaxValue / 2)
                return n;

            Console.WriteLine("Введите корректное положительное число.");
        }
    }

    static double read_number(string message)
    {
        while (true)
        {
            Console.Write(message);

            if (double.TryParse(Console.ReadLine(), out double value)
                && !double.IsNaN(value) && !double.IsInfinity(value))
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
                a[i] = read_number("Введите левую границу: ");
                a[i + 1] = read_number("Введите правую границу: ");

                if (a[i] >= a[i + 1])
                    Console.WriteLine("Левая граница должна быть меньше правой.");
            }
            while (a[i] >= a[i + 1]);
        }
    }

    static void print_array(double[] a)
    {
        Console.WriteLine("\nИсходный массив:");

        for (int i = 0; i < a.Length; i++)
            Console.Write("{0:G} ", a[i]);

        Console.WriteLine("\n\nИсходные интервалы:");

        for (int i = 0; i < a.Length; i += 2)
            Console.WriteLine("({0:G}; {1:G})", a[i], a[i + 1]);
    }

    static bool find_union(double[] a, out double left, out double right)
    {
        left = a[0];
        right = a[1];
        bool changed;

        do
        {
            changed = false;

            for (int i = 2; i < a.Length; i += 2)
            {
                if (left < a[i + 1] && a[i] < right)
                {
                    if (a[i] < left)
                    {
                        left = a[i];
                        changed = true;
                    }

                    if (a[i + 1] > right)
                    {
                        right = a[i + 1];
                        changed = true;
                    }
                }
            }
        }
        while (changed);

        for (int i = 2; i < a.Length; i += 2)
        {
            if (a[i] < left || a[i + 1] > right)
                return false;
        }

        return true;
    }

    static void Main()
    {
        Console.OutputEncoding = Encoding.UTF8;

        int n = read_count();
        double[] a = new double[2 * n];

        read_array(a);
        print_array(a);

        if (find_union(a, out double left, out double right))
            Console.WriteLine("\nОбъединение является интервалом: ({0:G}; {1:G})", left, right);
        else
            Console.WriteLine("\nОбъединение не является одним интервалом.");
    }
}
