using System;
using System.IO;
using System.Text;

/*
 * Лабораторная работа №3. Вариант 4.
 *
 * В квадратной матрице n x n для каждой строки
 * найти все симметричные числа, например 121 и 4554.
 *
 * Выполнил Кузнецов Дмитрий Олегович, группа 91.
 */

class Program
{
    static int read_number(string message, int min = int.MinValue, int max = int.MaxValue)
    {
        while (true)
        {
            Console.Write(message);
            string? text = Console.ReadLine();

            if (text == null)
                throw new EndOfStreamException();

            if (int.TryParse(text, out int value) && value >= min && value <= max)
                return value;

            Console.WriteLine("Введите целое число от {0} до {1}.", min, max);
        }
    }

    static void read_matrix(int[,] a)
    {
        Console.WriteLine("\nВведите элементы матрицы:");

        for (int i = 0; i < a.GetLength(0); i++)
        {
            for (int j = 0; j < a.GetLength(1); j++)
                a[i, j] = read_number(string.Format("a[{0}, {1}] = ", i + 1, j + 1));
        }
    }

    static void print_matrix(int[,] a)
    {
        Console.WriteLine("\nИсходная матрица:");

        for (int i = 0; i < a.GetLength(0); i++)
        {
            for (int j = 0; j < a.GetLength(1); j++)
                Console.Write("{0,12}", a[i, j]);

            Console.WriteLine();
        }
    }

    static bool is_symmetric(long value)
    {
        value = Math.Abs(value);
        long original = value;
        long reversed = 0;

        while (value > 0)
        {
            reversed = reversed * 10 + value % 10;
            value /= 10;
        }

        return original == reversed;
    }

    static int[] find_symmetric(int n, int row, ref int[,] a, out int count)
    {
        int[] result = new int[n];
        count = 0;

        for (int j = 0; j < n; j++)
        {
            if (is_symmetric(a[row, j]))
            {
                result[count] = a[row, j];
                count++;
            }
        }

        return result;
    }

    static void print_result(int row, int[] result, int count)
    {
        Console.Write("Строка {0}: ", row + 1);

        if (count == 0)
            Console.WriteLine("симметричных чисел нет.");
        else
        {
            for (int i = 0; i < count; i++)
                Console.Write("{0} ", result[i]);

            Console.WriteLine();
        }
    }

    static void Main()
    {
        Console.OutputEncoding = Encoding.UTF8;

        try
        {
            int n = read_number("Введите размер матрицы (от 1 до 100): ", 1, 100);
            int[,] a = new int[n, n];

            read_matrix(a);
            print_matrix(a);

            Console.WriteLine("\nСимметричные числа по строкам:");

            for (int row = 0; row < n; row++)
            {
                int[] result = find_symmetric(n, row, ref a, out int count);
                print_result(row, result, count);
            }
        }
        catch (EndOfStreamException)
        {
            Console.WriteLine("\nВвод завершён. Программа остановлена.");
        }
    }
}
