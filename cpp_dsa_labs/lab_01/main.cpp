#include "Tasks.h"

#include <exception>
#include <filesystem>
#include <stdexcept>

int read_choice()
{
    std::string line;
    int choice{};
    bool ready{ false };

    while (!ready && std::getline(std::cin, line))
    {
        std::size_t first{ line.find_first_not_of(" \t\r") };
        std::size_t last{ line.find_last_not_of(" \t\r") };
        if (first != std::string::npos && first == last
            && line[first] >= '0' && line[first] <= '4')
        {
            choice = line[first] - '0';
            ready = true;
        }
        else
            std::cout << "Enter an integer from 0 to 4: ";
    }
    return choice;
}

bool open_input(std::ifstream& input, bool writes_output)
{
    bool opened{ false };
    std::string name;
    std::cout << "Input file [input.txt]: ";

    while (!opened && std::getline(std::cin, name))
    {
        if (name.empty())
            name = "input.txt";

        std::error_code error;
        bool same_file{ writes_output
            && std::filesystem::equivalent(name, "output.txt", error) };
        if (same_file)
            std::cout << "Input must differ from output.txt. Enter another file: ";
        else if (!std::filesystem::is_regular_file(name, error))
            std::cout << "Cannot read this file. Enter another file: ";
        else
        {
            input.open(name);
            opened = input.is_open();
            if (!opened)
            {
                input.clear();
                std::cout << "Cannot open this file. Enter another file: ";
            }
        }
    }
    return opened;
}

void run_list_task()
{
    std::ifstream input;
    if (open_input(input, true))
    {
        flist::FLIST list;
        list.create_by_queue(input);
        list.sorting();
        std::cout << "Sorted list: ";
        list.print();

        bool found{ task15(list) };
        std::ofstream output("output.txt");
        if (!output)
            throw std::runtime_error("Cannot open output.txt for writing.");
        list.print(output);
        if (!found)
            output << "No group of at least two odd numbers in the list.\n";
        output.close();
        if (!output)
            throw std::runtime_error("Failed to write output.txt.");

        std::cout << "Result: ";
        list.print();
        if (!found)
            std::cout << "No group of at least two odd numbers in the list.\n";
        std::cout << "Result saved to output.txt.\n";
    }
}

void run_words_task()
{
    std::ifstream input;
    if (open_input(input, true))
    {
        QueueL words;
        std::size_t maximum{ task12(input, words) };
        if (input.bad())
            throw std::runtime_error("Failed to read the input file.");
        input.close();

        std::size_t count{ words.size() };
        std::ofstream output("output.txt");
        if (!output)
            throw std::runtime_error("Cannot open output.txt for writing.");

        while (!words.empty())
        {
            output << words.front() << '\n';
            words.pop();
        }
        output.close();
        if (!output)
            throw std::runtime_error("Failed to write output.txt.");

        std::cout << "Maximum length: " << maximum << "\nWords: " << count
            << "\nResult saved to output.txt.\n";
    }
}

void run_brackets_task(bool use_array)
{
    std::ifstream input;
    if (open_input(input, false))
    {
        std::string line;
        while (std::getline(input, line))
        {
            bool correct{};
            if (use_array)
            {
                StackA stack(line.size());
                correct = balanced(line, stack);
            }
            else
            {
                StackL stack;
                correct = balanced(line, stack);
            }
            std::cout << line << " -> " << (correct ? "Yes" : "No") << '\n';
        }
        if (input.bad())
            throw std::runtime_error("Failed to read the input file.");
    }
}

int main()
{
    int choice{};
    do
    {
        std::cout << "\n1. List: task 15\n2. Queue: task 12 (longest words)\n"
            << "3. Brackets: StackL\n4. Brackets: StackA\n0. Exit\nChoice: ";
        try
        {
            choice = read_choice();
            switch (choice)
            {
            case 1: run_list_task(); break;
            case 2: run_words_task(); break;
            case 3: run_brackets_task(false); break;
            case 4: run_brackets_task(true); break;
            }
        }
        catch (const std::exception& error)
        {
            std::cerr << "Error: " << error.what() << '\n';
        }
    } while (choice != 0);
    return 0;
}
