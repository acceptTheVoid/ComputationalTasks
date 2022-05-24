#include <cmath>
#include <functional>
#include <iostream>

typedef std::function<double(double)> Expression;

/// Функция для поиска минимума методом золотого сечения
/// @param a - левая граница приближения
/// @param b - правая граница приближения
/// @param eps - точность
/// @param expr - функция для которой будет найден минимум
/// @param counter - костыльный счетчик итераций
double golden_ratio(
        double a,
        double b,
        double eps,
        Expression const &expr,
        // Господи какой же это костыль
        unsigned *counter = nullptr
) {
    const double PHI = 1.618;

    while(fabs(b - a) > eps) {
        if(counter) ++*counter;

        double x1 = b - (b - a) / PHI;
        double x2 = a + (b - a) / PHI;
        expr(x1) >= expr(x2) ? a = x1 : b = x2;
    }

    return (a + b) / 2;
}

/// Выражение из 4
double equation_2(double x) {
    return 3 * x * x * x * x + 5 * x * x * x + 3 * x * x - 18 * x - 9;
}

int main() {
    // Примеры для нахождения локального минимума и количества итераций
    unsigned counter = 0;

    std::cout << golden_ratio(-10, 10, 0.001, equation_2, &counter) << "\n";
    std::cout << counter << "\n";
}