module;

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <cstdio>
#include <cerrno>
#include <system_error>
#include <filesystem>
#include <cstdint>
#include <cstddef>
#include <cctype>
#include <cstring>
#include <sstream>
#include <limits>
#include <stdexcept>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

export module lab_io;
import lab_queue;

/*
 * Лабораторная работа № 3, вариант 4B.
 * Сервисные функции: проверенный ввод, загрузка и сохранение очереди.
 * Кузнецов Дмитрий Олегович, группа 91.
 */

// Читает строку целиком; возвращает false, если ввод закончился до первого символа.
// Принимает последнюю строку без перевода строки; отвергает слишком длинные строки и нулевые байты.
bool read_line(std::istream& input, char* text, std::streamsize capacity)
{
    input.getline(text, capacity);
    std::streamsize count{ input.gcount() };

    if (input.bad())
        throw std::runtime_error("Input read error.");

    if (input.eof() && count == 0)
        return false;

    if (input.fail() && !input.eof())
    {
        input.clear();
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Input line is too long.");
    }

    // getline учитывает извлечённый перевод строки в gcount; при конце файла его нет.
    std::streamsize length{ count - (input.eof() ? 0 : 1) };
    if (static_cast<std::streamsize>(std::strlen(text)) != length)
        throw std::invalid_argument("Input line contains a zero byte.");

    return true;
}

// Пропускает пробельные символы и возвращает указатель на первый непробельный символ.
const char* skip_spaces(const char* text)
{
    while (std::isspace(static_cast<unsigned char>(*text)))
        ++text;
    return text;
}

// Читает одну цифру меню в заданном диапазоне и повторяет запрос при ошибке.
export bool read_choice(const char* prompt, std::uint32_t& value, std::uint32_t minimum, std::uint32_t maximum)
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
            if (*start >= '0' && *start <= '9' &&
                *skip_spaces(start + 1) == '\0')
            {
                std::uint32_t chosen{ static_cast<std::uint32_t>(*start - '0') };
                if (chosen >= minimum && chosen <= maximum)
                {
                    value = chosen;
                    return true;
                }
            }
        }
        catch (const std::invalid_argument&)
        {
            // После неверной или слишком длинной строки повторяем запрос.
        }
        std::cout << "Enter one menu number from " << minimum << " to " << maximum << ".\n";
    }
}

// Консоль Windows отдаёт символы Unicode, файлы и перенаправленный ввод используют UTF-8.
bool read_console_line(std::string& line)
{
#ifdef _WIN32
    HANDLE console{ GetStdHandle(STD_INPUT_HANDLE) };
    DWORD mode{};
    if (GetConsoleMode(console, &mode))
    {
        std::wstring wide;
        wchar_t chunk[256]{};
        DWORD count{};
        std::cout.flush();
        do
        {
            if (!ReadConsoleW(console, chunk, 256, &count, nullptr))
            {
                std::cin.setstate(std::ios::badbit);
                throw std::runtime_error("Console read error.");
            }
            if (count == 0 || chunk[0] == 26)
            {
                std::cin.setstate(std::ios::eofbit | std::ios::failbit);
                return false;
            }
            wide.append(chunk, count);
        } while (wide.back() != L'\n');

        wide.pop_back();
        if (!wide.empty() && wide.back() == L'\r')
            wide.pop_back();
        int bytes{ WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr) };
        line.resize(bytes);
        if (bytes)
            WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), line.data(), bytes, nullptr, nullptr);
        return true;
    }
#endif
    return static_cast<bool>(std::getline(std::cin, line));
}

#ifdef _WIN32
// На время работы программы выводим UTF-8 в консоль Windows.
export class ConsoleEncoding
{
    UINT previous{ GetConsoleOutputCP() };
public:
    ConsoleEncoding() { if (previous) SetConsoleOutputCP(CP_UTF8); }
    ~ConsoleEncoding() { if (previous) SetConsoleOutputCP(previous); }
};
#endif

// Формат записи одинаков для клавиатуры, поиска и файла.
export template<class X>
bool read_value_from_console(const char* format, X& value)
{
    std::string line;
    while (true)
    {
        std::cout << "Enter " << format << ": ";
        if (!read_console_line(line))
            return false;
        if (parse_record(line, value))
            return true;
        std::cout << "Invalid value. Expected " << format << ".\n";
    }
}

namespace fs = std::filesystem;

// Выводит путь без потери русских букв в консоли Windows и в перенаправленном выводе.
void print_path(const fs::path& path)
{
#ifdef _WIN32
    HANDLE console{ GetStdHandle(STD_OUTPUT_HANDLE) };
    DWORD mode{}, written{};
    if (GetConsoleMode(console, &mode))
    {
        std::cout.flush();
        WriteConsoleW(console, path.c_str(), static_cast<DWORD>(path.native().size()), &written, nullptr);
        return;
    }
#endif
    std::u8string bytes{ path.u8string() };
    std::cout.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}

// Использует тот же ввод в UTF-8, что и текстовые элементы очереди.
bool read_path_line(fs::path& path)
{
    std::string line;
    if (!read_console_line(line))
        return false;
    path = std::u8string(line.begin(), line.end());
    return true;
}

// Убирает внешние пробелы и кавычки. Пустая строка отменяет выбор файла.
bool read_path(fs::path& path, const char* prompt)
{
    std::cout << "Working directory: ";
    print_path(fs::current_path());
    std::cout << '\n' << prompt << " (Enter to cancel): ";
    if (!read_path_line(path))
        return false;

    fs::path::string_type text{ path.native() };
    const fs::path::string_type spaces{ fs::path(" \t\r\n").native() };
    std::size_t first{ text.find_first_not_of(spaces) };
    if (first == text.npos)
    {
        path.clear();
        return true;
    }
    text = text.substr(first, text.find_last_not_of(spaces) - first + 1);
    if (text.size() >= 2 && text.front() == '"' && text.back() == '"')
        text = text.substr(1, text.size() - 2);
    if (text.empty() || text.find('\0') != text.npos)
        throw std::invalid_argument("Enter a valid file path.");

    path = text;
#ifdef _WIN32
    fs::path native{ path };
    native.make_preferred();
    if (native.native().starts_with(L"\\\\?\\") || native.native().starts_with(L"\\\\.\\"))
        throw std::invalid_argument("Enter a regular file path.");
    for (const fs::path& component : native.relative_path())
    {
        const std::wstring& part{ component.native() };
        if (part.empty() || part == L"." || part == L"..")
            continue;
        if (part.find_first_of(L"<>:\"|?*") != part.npos || part.back() == '.' || part.back() == ' ')
            throw std::invalid_argument("Invalid Windows file name.");
        for (wchar_t ch : part)
            if (ch < 32)
                throw std::invalid_argument("Invalid Windows file name.");
        std::wstring stem{ part.substr(0, part.find('.')) };
        stem.erase(stem.find_last_not_of(L' ') + 1);
        for (wchar_t& ch : stem)
            if (ch >= L'a' && ch <= L'z')
                ch = static_cast<wchar_t>(ch - L'a' + L'A');
        bool port{ stem.size() == 4 && (stem.starts_with(L"COM") || stem.starts_with(L"LPT")) &&
                   std::wstring_view(L"123456789¹²³").find(stem.back()) != std::wstring_view::npos };
        if (port || stem == L"CON" || stem == L"NUL" || stem == L"PRN" || stem == L"AUX" ||
            stem == L"CONIN$" || stem == L"CONOUT$")
            throw std::invalid_argument("Reserved Windows device name.");
    }
#endif
    if (!path.has_filename() || path.filename() == "." || path.filename() == "..")
        throw std::invalid_argument("Enter a file name, not a folder.");
    path = fs::absolute(path);
    return true;
}

// Открывает обычный файл. Queue::load проверяет записи до замены очереди.
template<class X>
void load_from_file(Queue<X>& queue, const fs::path& name)
{
    if (!fs::exists(name))
        throw std::runtime_error("Input file does not exist.");
    if (!fs::is_regular_file(name))
        throw std::runtime_error("Input must be an existing regular file.");

    std::ifstream file{ name, std::ios::binary };
    if (!file.is_open())
        throw std::runtime_error("Cannot open input file.");

    queue.load(file);
}

// Повторяет запрос файла при ошибке. Очередь заменяется только после успешного чтения.
export template<class X>
void load_queue(Queue<X>& queue)
{
    while (true)
    {
        try
        {
            fs::path path;
            if (!read_path(path, "Input file path") || path.empty())
                return;
            load_from_file(queue, path);
            std::cout << "Loaded elements: " << queue.size() << '\n';
            return;
        }
        catch (const std::exception& error)
        {
            std::cout << "File error: " << error.what() << '\n';
            if (!std::cin)
                return;
        }
    }
}

// Пишет во временный файл в той же папке. Заменяет целевой файл только после успешного закрытия.
void save_to_file(const char* content, const fs::path& path, bool overwrite)
{
    fs::path temp;
    std::FILE* file{};
    for (unsigned i{}; i < 1000; ++i)
    {
        temp = path.parent_path() / (".queue-" + std::to_string(i) + ".tmp");
        if (temp.filename() == path.filename())
            continue;
#ifdef _WIN32
        if (_wcsicmp(temp.filename().c_str(), path.filename().c_str()) == 0)
            continue;
        _wfopen_s(&file, temp.c_str(), L"wbx");
#else
        file = std::fopen(temp.c_str(), "wbx");
#endif
        if (file)
            break;
        if (errno != EEXIST)
            throw std::system_error(errno, std::generic_category(), "Cannot create output file");
    }
    if (!file)
        throw std::runtime_error("Cannot create a temporary file.");

    try
    {
        bool written{ std::fputs(content, file) >= 0 };
        if (std::fclose(file) != 0 || !written)
            throw std::runtime_error("Cannot write output file.");

#ifdef _WIN32
        if (!MoveFileExW(temp.c_str(), path.c_str(), overwrite ? MOVEFILE_REPLACE_EXISTING : 0))
            throw std::system_error(GetLastError(), std::system_category(), "Cannot save output file");
#else
        if (overwrite)
            fs::rename(temp, path);
        else
        {
            fs::create_hard_link(temp, path);
            fs::remove(temp);
        }
#endif
    }
    catch (...)
    {
        std::error_code ignored;
        fs::remove(temp, ignored);
        throw;
    }
}

// Запрашивает путь, подтверждение создания папок и перезаписи очереди.
export template<class X>
void save_queue(const Queue<X>& queue)
{
    while (true)
    {
        try
        {
            fs::path path;
            if (!read_path(path, "Output file path") || path.empty())
                return;

            std::uint32_t confirm{};
            if (!fs::exists(path.parent_path()))
            {
                if (!read_choice("Folder does not exist. Create it? 1. Yes  0. No: ", confirm, 0, 1) || !confirm)
                    return;
                fs::create_directories(path.parent_path());
            }

            fs::file_status status{ fs::symlink_status(path) };
            bool exists{ fs::exists(status) };
            if (exists)
            {
                if (!fs::is_regular_file(status) || fs::hard_link_count(path) > 1)
                    throw std::runtime_error("Choose a regular file, not a directory or linked file.");
                if (!read_choice("File exists. Overwrite? 1. Yes  0. No: ", confirm, 0, 1) || !confirm)
                    return;
            }

            std::ostringstream content;
            content.exceptions(std::ios::badbit | std::ios::failbit);
            queue.print(content);
            save_to_file(content.str().c_str(), path, exists);
            std::cout << "Queue saved to ";
            print_path(path);
            std::cout << '\n';
            return;
        }
        catch (const std::exception& error)
        {
            std::cout << "File error: " << error.what() << '\n';
            if (!std::cin)
                return;
        }
    }
}
