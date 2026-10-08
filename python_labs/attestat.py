def round_result(func):
    def wrapper(self):
        return round(func(self))

    return wrapper


class Attestat:
    def __init__(self, full_name: str) -> None:
        self.__full_name = full_name
        self.__grades = {}

    @property
    def full_name(self) -> str:
        return self.__full_name

    @full_name.setter
    def full_name(self, value: str) -> None:
        self.__full_name = value

    @property
    def grades(self) -> dict:
        return self.__grades.copy()

    @round_result
    def average_grade(self) -> float:
        if len(self.__grades) == 0:
            return 0

        return sum(self.__grades.values()) / len(self.__grades)

    def __iadd__(self, item):
        subject, grade = item
        self.__grades[subject] = grade
        return self

    def __len__(self) -> int:
        return len(self.__grades)

    def __getitem__(self, subject: str) -> int:
        return self.__grades[subject]

    def __setitem__(self, subject: str, grade: int) -> None:
        self.__grades[subject] = grade

    def __delitem__(self, subject: str) -> None:
        del self.__grades[subject]

    def __contains__(self, subject: str) -> bool:
        return subject in self.__grades

    def __str__(self) -> str:
        result = f"Аттестат: {self.__full_name}\n"

        if len(self.__grades) == 0:
            return result + "Оценок нет"

        for subject, grade in self.__grades.items():
            result += f"{subject}: {grade}\n"

        return result.rstrip()