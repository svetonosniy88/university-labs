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

class Calendar:
    def __init__(self, max_note_length: int) -> None:
        self.max_note_length = max_note_length
        self.days = [[[] for _ in range(24)] for _ in range(7)]

    def add_note(self, day: int, hour: int, text: str) -> None:
        if len(self.days[day-1][hour]) >= 3:
            raise ValueError("Note limit for this hour reached")
        if len(text) > self.max_note_length:
            raise ValueError("Note exceeds maximum length")