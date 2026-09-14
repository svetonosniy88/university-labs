# У планировщиков есть план.
# И для того, чтобы не держать его весь в голове, они создали календарь на неделю: список из дней (1-7),
# в каждом из которых список из часов (0-23), в этот час можно записать заметки (тоже список).
# Заметок в одном часе не более 3, а длина любой из них не больше N.
# Предусмотреть операции:
# – добавить заметку в указанный день и час,
# – удалить все заметки из указанного дня и часа,
# – календарь должно быть можно ради удобства распечатать двумя способами: чтобы столбцами были дни, строками — часы, и наоборот,
# – вывести список «день недели» — «число заметок»,
# – вывести среднее число заметок в час по всему календарю.
#
# Выполнил Кузнецов Дмитрий Олегович 91 группа

from typing import Literal


class Calendar:
    def __init__(
            self,
            max_note_length: int
    ) -> None:
        self.max_note_length = max_note_length
        self.days = [[[] for _ in range(24)] for _ in range(7)]

    def add_note(
            self,
            day: int,
            hour: int,
            text: str
    ) -> None:
        if not 1 <= day <= 7:
            raise ValueError("Day must be from 1 to 7")

        if not 0 <= hour <= 23:
            raise ValueError("Hour must be from 0 to 23")

        if len(self.days[day - 1][hour]) >= 3:
            raise ValueError("Note limit for this hour reached")

        if len(text) > self.max_note_length:
            raise ValueError("Note exceeds maximum length")

        self.days[day - 1][hour].append(text)

    def delete_notes(
            self,
            day: int,
            hour: int
    ) -> None:
        if not 1 <= day <= 7:
            raise ValueError("Day must be from 1 to 7")

        if not 0 <= hour <= 23:
            raise ValueError("Hour must be from 0 to 23")

        self.days[day - 1][hour].clear()

    def print_calendar(
            self,
            orientation: Literal["days", "hours"]
    ) -> None:
        if orientation == "hours":
            for day in range(7):
                for hour in range(24):
                    print(self.days[day][hour], end='\t')
                print()

        elif orientation == "days":
            for hour in range(24):
                for day in range(7):
                    print(self.days[day][hour], end='\t')
                print()

        else:
            raise ValueError("Unknown calendar orientation")

    def print_days_notes(self) -> None:
        total = 0

        for day in range(7):
            for hour in range(24):
                total += len(self.days[day][hour])

            print(
                "День недели:",
                day + 1,
                "— Количество заметок:",
                total
            )

            total = 0

    def average_notes_per_hour(self) -> None:
        total = 0

        for day in range(7):
            for hour in range(24):
                total += len(self.days[day][hour])

        print(
            "Среднее число заметок в час:",
            total / (7 * 24)
        )


def read_int(
        message: str
) -> int:
    while True:
        try:
            return int(input(message))

        except ValueError:
            print("Enter an integer")


def main() -> None:
    print("Enter maximum note length:")

    max_note_length = read_int("> ")

    while max_note_length <= 0:
        print("Maximum note length must be positive")
        max_note_length = read_int("> ")

    calendar = Calendar(max_note_length)

    choice = -1

    while choice != 0:
        print(
            "\n1. Add note"
            "\n2. Delete notes"
            "\n3. Print calendar: days in columns"
            "\n4. Print calendar: hours in columns"
            "\n5. Print number of notes for each day"
            "\n6. Print average number of notes per hour"
            "\n0. Exit"
        )

        print("Choice:")
        choice = read_int("> ")

        if choice == 1:
            print("Enter day (1-7):")
            day = read_int("> ")

            print("Enter hour (0-23):")
            hour = read_int("> ")

            print("Enter note:")
            text = input("> ")

            try:
                calendar.add_note(
                    day,
                    hour,
                    text
                )

                print("Note added")

            except ValueError as error:
                print(error)

        elif choice == 2:
            print("Enter day (1-7):")
            day = read_int("> ")

            print("Enter hour (0-23):")
            hour = read_int("> ")

            try:
                calendar.delete_notes(
                    day,
                    hour
                )

                print("Notes deleted")

            except ValueError as error:
                print(error)

        elif choice == 3:
            calendar.print_calendar("days")

        elif choice == 4:
            calendar.print_calendar("hours")

        elif choice == 5:
            calendar.print_days_notes()

        elif choice == 6:
            calendar.average_notes_per_hour()

        elif choice == 0:
            break

        else:
            print("Unknown menu item")


if __name__ == "__main__":
    main()