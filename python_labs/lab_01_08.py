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

from typing import Literal

class Calendar:
    def __init__(self, max_note_length: int) -> None:
        self.max_note_length = max_note_length
        self.days = [[[] for _ in range(24)] for _ in range(7)]

    def add_note(self, day: int, hour: int, text: str) -> None:
        if len(self.days[day-1][hour]) >= 3:
            raise ValueError("Note limit for this hour reached")
        if len(text) > self.max_note_length:
            raise ValueError("Note exceeds maximum length")

        self.days[day-1][hour].append(text)

    def delete_notes(self, day: int, hour: int) -> None:
        self.days[day-1][hour].clear()

    def print_calendar(
            self,
            orientation: Literal["days", "hours"]
    ) -> None:
        if orientation == "hours":
            for day in range(7):
                for hour in range(24):
                    print(self.days[day][hour],  end = '\t')
                print('\n')
        elif orientation == "days":
            for hour in range(24):
                for day in range(7):
                    print(self.days[day][hour], end = '\t')
                print()
        else:
            raise ValueError("Unknown calendar orientation")

    def print_days_notes(self) -> None:
        total = 0
        for day in range(7):
            for hour in range(24):
                total += len(self.days[day][hour])
            print("День недели: ", day+1, " — Количество заметок:  ", total)
            total = 0

    def average_notes_per_hour(self) -> None:
        total = 0
        for day in range(7):
            for hour in range(24):
                total += len(self.days[day][hour])
        print("Среднее число заметок в час: ", total/(7*24))
