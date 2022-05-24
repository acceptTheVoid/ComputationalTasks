#include <cmath>
#include <functional>
#include <iostream>

typedef std::function<double(double)> Expression;

/// Решение нелинейного уравнения методом касательных
/// @param guess - предположение где будет находиться корень уравнения
/// @param eps - точность
/// @param equation - функция для которой будет найдено решение
/// @param derivative - производная функции
double newtone_method(
        double guess,
        double eps,
        Expression const &equation,
        Expression const &derivative
) {
    auto diff = guess - equation(guess) / derivative(guess);
    while(fabs(guess - diff) > eps) {
        guess = diff;
        diff = guess - equation(guess) / derivative(guess);
    }
    return guess;
}

/// Решение нелинейного уравнения методом дихтомии
/// @param a - левая граница
/// @param b - правая граница
/// @param eps - точность
/// @param equation - функция для которой будет найдено решение
double dichtomia(
        double a,
        double b,
        double eps,
        Expression const &equation
) {
    double delta = 2 * eps;
    while(b - a > delta) {
        double c = (a + b) / 2;
        double tmp = equation(a) * equation(c);
        (tmp <= 0 ? b : a) = c;
    }

    return (a + b) / 2;
}

/// Выражение из 3
double equation(double x) {
    return 2 * x * x * x + 5 * x * x - x - 6;
}

/// Производная выражения из 3
double derivative(double x) {
    return 6 * x * x + 10 * x - 1;
}

int main() {
    // Примеры для нелинейных уравнений
    std::cout << newtone_method(0, 0.0001, equation, derivative) << "\n";
    std::cout << dichtomia(-3, -1.75, 0.0001, equation) << "\n";
}