# Лабораторная работа №3
# Вариант 11 — «Аттестат»
#
# Написать декоратор, который округляет числовой результат до целого.
# Написать класс «Аттестат», который хранит ФИО ученика и набор пар
# «предмет — оценка». Реализовать метод вычисления среднего балла
# и применить к нему декоратор.
# Перегрузить оператор += для добавления оценки за предмет.
# Реализовать магические методы класса-контейнера:
# __len__, __getitem__, __setitem__, __delitem__, __contains__.
# Программа должна позволять создавать аттестат и работать с ним,
# в том числе выполнять ввод и вывод через файл.
#
# Выполнил Кузнецов Дмитрий Олегович, 91 группа

from attestat import Attestat


def read_int(message: str) -> int:
    while True:
        try:
            return int(input(message))

        except ValueError:
            print("Введите целое число")


def create_attestat() -> Attestat:
    print("Введите ФИО ученика:")
    full_name = input("> ")

    return Attestat(full_name)


def save_to_file(attestat: Attestat, filename: str) -> None:
    with open(filename, "w", encoding="utf-8") as file:
        file.write(attestat.full_name + "\n")

        for subject, grade in attestat.grades.items():
            file.write(f"{subject}\t{grade}\n")


def load_from_file(filename: str) -> Attestat:
    with open(filename, "r", encoding="utf-8") as file:
        full_name = file.readline().rstrip("\n")

        if full_name == "":
            raise ValueError("В файле отсутствует ФИО")

        attestat = Attestat(full_name)

        for line in file:
            line = line.rstrip("\n")

            if line == "":
                continue

            subject, grade = line.rsplit("\t", 1)
            attestat += (subject, int(grade))

    return attestat


def print_menu() -> None:
    print(
        "\n1. Создать аттестат"
        "\n2. Загрузить аттестат из файла"
        "\n3. Сохранить аттестат в файл"
        "\n4. Вывести аттестат"
        "\n5. Добавить предмет и оценку через +="
        "\n6. Получить оценку по предмету"
        "\n7. Установить оценку по предмету"
        "\n8. Удалить предмет"
        "\n9. Проверить наличие предмета"
        "\n10. Вывести количество предметов"
        "\n11. Вывести средний балл"
        "\n0. Завершить"
    )


def main() -> None:
    attestat = None

    while True:
        print_menu()

        print("Выберите действие:")
        choice = read_int("> ")

        if choice != 0 and choice != 1 and choice != 2 and attestat is None:
            print("Сначала создайте или загрузите аттестат")
            continue

        match choice:
            case 1:
                attestat = create_attestat()
                print("Аттестат создан")

            case 2:
                print("Введите имя файла:")
                filename = input("> ")

                try:
                    attestat = load_from_file(filename)
                    print("Аттестат загружен")

                except (OSError, ValueError) as error:
                    print("Ошибка:", error)

            case 3:
                print("Введите имя файла:")
                filename = input("> ")

                try:
                    save_to_file(attestat, filename)
                    print("Аттестат сохранён")

                except OSError as error:
                    print("Ошибка:", error)

            case 4:
                print(attestat)

            case 5:
                print("Введите предмет:")
                subject = input("> ")

                print("Введите оценку:")
                grade = read_int("> ")

                attestat += (subject, grade)
                print("Оценка добавлена")

            case 6:
                print("Введите предмет:")
                subject = input("> ")

                try:
                    print("Оценка:", attestat[subject])

                except KeyError:
                    print("Такого предмета нет")

            case 7:
                print("Введите предмет:")
                subject = input("> ")

                print("Введите оценку:")
                grade = read_int("> ")

                attestat[subject] = grade
                print("Оценка установлена")

            case 8:
                print("Введите предмет:")
                subject = input("> ")

                try:
                    del attestat[subject]
                    print("Предмет удалён")

                except KeyError:
                    print("Такого предмета нет")

            case 9:
                print("Введите предмет:")
                subject = input("> ")

                if subject in attestat:
                    print("Предмет есть в аттестате")
                else:
                    print("Такого предмета нет")

            case 10:
                print("Количество предметов:", len(attestat))

            case 11:
                print("Средний балл:", attestat.average_grade())

            case 0:
                break

            case _:
                print("Неизвестный пункт меню")


if __name__ == "__main__":
    main()