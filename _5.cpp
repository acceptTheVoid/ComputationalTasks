#include <cmath>
#include <functional>
#include <iostream>

typedef std::function<double(double)> Expression;

/*
* Дружище, внимание
* Всю секцию ниже я уже нихуя не понимал так как у нас такой хуйни просто не было
* Так что я ее просто спиздил с какого-то сайта и сделал чуть более читабельной и модульной
* Надеюсь ты не сойдешь с ума пытаясь познать эту тему
* */

struct vec2 {
    double x;
    double y;
};

//Вычисление одномерной функции для нахождения шага методом золотого сечения
double MakeSimplefx(
        double x,
        vec2 grad,
        vec2 xj,
        std::function<double(vec2)> const &expr
) {
    vec2 buffer = {
            xj.x - x * grad.x,
            xj.y - x * grad.y
    };

    return expr(buffer);
}

//Метод золотого сечения для нахождения шага (lambda)
double GoldenSelection(
        double a,
        double b,
        double eps,
        vec2 gradient,
        vec2 x,
        std::function<double(vec2)> const &expr
) {
    const double fi = 1.6180339887;

    double x1 = b - ((b - a) / fi);
    double x2 = a+((b-a)/fi);
    double y1 = MakeSimplefx(x1, gradient, x, expr);
    double y2 = MakeSimplefx(x2, gradient, x, expr);

    while(fabs(b - a) > eps) {
        if(y1 <= y2) {
            b = x2;
            x2 = x1;
            x1 = b - ((b - a) / fi);
            y2 = y1;
            y1 = MakeSimplefx(x1, gradient, x, expr);
        } else {
            a = x1;
            x1 = x2;
            x2 = a + ((b - a) / fi);
            y1 = y2;
            y2 = MakeSimplefx(x2, gradient, x, expr);
        }
    }

    return (a + b) / 2;
}

//Функция вычисления нового приближения
vec2 Calculate(vec2 x, vec2 gradient, double lambda) {
    vec2 buffer = {
            x.x - lambda * gradient.x,
            x.y - lambda * gradient.y
    };

    return buffer;
}

/// Метод наискорейшего спуска
/// @param x - начальное приближение
/// @param eps - точность
/// @param expr - функция для которой ищется минимум
/// @param gradient - функция градиента для первой функции
/// @param counter - костыльный счетчик итераций
vec2 GradDown(
        vec2 x,
        double eps,
        std::function<double(vec2)> const &expr,
        std::function<vec2(vec2)> const &gradient,
        unsigned *counter = nullptr
) {
    vec2 current = x;
    vec2 last{};

    do {
        if(counter) ++*counter;

        last = current; //Запоминаем предыдущее значение
        vec2 grad = gradient(current); //Вычисляем градиент
        //Находим шаг вычислений методом золотого сечения
        double lambda = GoldenSelection(0, 0.05, eps, grad, current, expr);
        current = Calculate(current, grad, lambda); //Вычисляем новое приближение
    } while(fabs(expr(current) - expr(last)) > eps); //Проверяем условие

    return current; //Возвращаем результат
}

/// Выражение из 5
double equation_3(vec2 vec) {
    return -vec.x * vec.x * vec.y - 2 * vec.x * vec.y + vec.y * vec.y + 8 * vec.y - 1;
}

/// Градиент выражения из 5
vec2 grad(vec2 vec) {
    return { -2 * (vec.x + 1) * vec.y, -(vec.x * vec.x) - 2 * vec.x + 2 * vec.y + 8 };
}

int main() {
    // Примеры для нахождения локального минимума в пространстве и количества итераций
    unsigned counter = 0;

    auto aboba = GradDown({ 0, 0 }, 0.001, equation_3, grad, &counter);
    std::cout << aboba.x << " " << aboba.y << "\n" << counter << "\n";
}