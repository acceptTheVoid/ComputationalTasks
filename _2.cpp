#include <cmath>
#include <functional>
#include <iostream>

typedef std::function<double(double)> Expression;

/// Нахождение площади интеграла методом средних прямоугольников
/// @param a - левая граница интеграла
/// @param b - правая граница интеграла
/// @param integral - сам интеграл
/// @param partitions - сколько отрезков посчитает метод (чем больше тем точнее)
double integral_rec(
        double a,
        double b,
        Expression const &integral,
        int partitions = 10
) {
    double inc = (b - a) / partitions;
    double res = 0;
    for(int i = 0; i < partitions; ++i) {
        res += integral(a + inc * i + inc / 2);
    }

    return res * inc;
}

/// Нахождение площади интеграла методом трапеций
/// @param a - левая граница интеграла
/// @param b - правая граница интеграла
/// @param integral - сам интеграл
/// @param partitions - сколько отрезков посчитает метод (чем больше тем точнее)
double integral_trap(
        double a,
        double b,
        Expression const &integral,
        int partitions = 10
) {
    double inc = (b - a) / partitions;
    double res = integral(a) + integral(b);
    res /= 2;
    for(int i = 1; i != partitions; ++i) {
        res += integral(a + i * inc);
    }

    return res * inc;
}

/// Нахождение площади интеграла методом Симпосна
/// @param a - левая граница интеграла
/// @param b - правая граница интеграла
/// @param integral - сам интеграл
/// @param partitions - сколько отрезков посчитает метод (чем больше тем точнее)
double integral_simp(
        double a,
        double b,
        Expression const &integral,
        int partitions = 10
) {
    double inc = (b - a) / partitions;
    double res = 0;

    for(int i = 0; i != partitions; ++i) {
        double x1 = a + i * inc;
        double x2 = a + (i + 1) * inc;

        res += inc/6. * (integral(x1) + 4. * integral(.5 * (x1 + x2)) + integral(x2));
    }

    return res;
}

/// Интеграл из 2.1
double integral_1(double x) {
    return -(x * x * x * x * x) + 2 * (x * x * x * x) + 5 * x;
}

/// Интеграл из 2.2
double integral_2(double x) {
    double tg = tan(x);
    return x / (tg * tg * tg);
}

typedef std::function<double(double, double, Expression, int)> IntegralMethod;
// Метод рунге который вроде было необязательно реализовывать или надо было хз
// Я в любом случае его сделал
double runge(
        double a,
        double b,
        IntegralMethod const &im,
        Expression const &i
) {
    int partitions = 10;

    return fabs(im(a, b, i, partitions * 2) - im(a, b, i, partitions));
}

int main() {
    // Примеры для интегралов
    std::cout << integral_rec(-2, 1, integral_1) << "\n";
    std::cout << integral_trap(-2, 1, integral_1) << "\n";
    std::cout << integral_simp(-2, 1, integral_1) << "\n";
    std::cout << runge(-2, 1, integral_simp, integral_1) << "\n";

    std::cout << integral_rec(0.2, 0.6, integral_2) << "\n";
    std::cout << integral_trap(0.2, 0.6, integral_2) << "\n";
    std::cout << integral_simp(0.2, 0.6, integral_2) << "\n";
    std::cout << runge(0.2, 0.6, integral_simp, integral_2) << "\n";
}
