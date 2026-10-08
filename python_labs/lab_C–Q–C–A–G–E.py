# Лабораторная работа №2
# Вариант: C-Q-C-A-G-E

# C — расположение трёх координатных областей в одном окне в одну строку.
# Q — график гауссовой функции со случайными параметрами a и c.
# C — две кривые в полярной системе координат: r = фи при фи >= 0 и r = -фи-пи при фи <= -пи.
# A — дискретный график последовательности из 30 элементов, где каждый следующий элемент равен предыдущему плюс случайное целое число из диапазона [-5; 10].
# Точки отображаются отдельно, шестиугольными маркерами, цвет совпадает с цветом графика №1.
# G — график №1: чёрная сплошная линия толщиной 4, сетка включена. Полярные кривые: разные оттенки зелёного, сплошные линии толщиной 3, сетка включена.
# E — фамилия на первом графике.

# Выполнил: Кузнецов Дмитрий Олегович, 91 группа

import numpy as np
import matplotlib.pyplot as plt


def draw_graph_1(gr1, rng) -> None:
    a = rng.uniform(1, 5)
    c = rng.uniform(1, 2)

    x = np.linspace(0, 6, 100)
    y = a * np.exp(-((x - 3) ** 2) / (2 * c ** 2))

    gr1.plot(
        x,
        y,
        color="black",
        linestyle="-",
        linewidth=4
    )

    gr1.grid(True)

    gr1.text(
        0.05,
        0.95,
        "Фамилия",
        transform=gr1.transAxes,
        verticalalignment="top"
    )


def draw_graph_2(gr2) -> None:
    phi1 = np.linspace(0, 4 * np.pi, 500)
    r1 = phi1

    phi2 = np.linspace(-5 * np.pi, -np.pi, 500)
    r2 = -phi2 - np.pi

    gr2.plot(
        phi1,
        r1,
        color="darkgreen",
        linestyle="-",
        linewidth=3
    )

    gr2.plot(
        phi2,
        r2,
        color="limegreen",
        linestyle="-",
        linewidth=3
    )

    gr2.grid(True)


def draw_graph_3(gr3, rng) -> None:
    values = np.zeros(30, dtype=int)

    for i in range(1, 30):
        values[i] = values[i - 1] + rng.integers(-5, 11)

    x = np.arange(30)

    gr3.plot(
        x,
        values,
        linestyle="None",
        marker="h",
        color="black"
    )


def main() -> None:
    wind = plt.figure(figsize=(15, 5))
    wind.canvas.manager.set_window_title("Graphics")

    gr1 = wind.add_subplot(1, 3, 1)
    gr2 = wind.add_subplot(1, 3, 2, projection="polar")
    gr3 = wind.add_subplot(1, 3, 3)

    rng = np.random.default_rng()

    draw_graph_1(gr1, rng)
    draw_graph_2(gr2)
    draw_graph_3(gr3, rng)

    plt.show()


if __name__ == "__main__":
    main()