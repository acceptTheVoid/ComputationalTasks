#include <cmath>
#include <limits>
#include <iostream>

double atanh(double x) {
    // Библиотечная функция на значениях |x| == 1 выдает бесконечность
    // С соответствующим знаком (умножение на x для этого)
    if(fabs(x) == 1) return x * std::numeric_limits<double>::infinity();

    // Для чисел |x| > 1 библиотечная функция выдает nan
    if(fabs(x) > 1) return std::numeric_limits<double>::quiet_NaN();

    double res = x;
    double power = x;

    // Теоретически количество итераций можно было бы сбавить потому что требовние к точности до 10^-4 выполняется
    // С лихвой но как же мне похуй....
    for(unsigned i = 1; i <= 500; ++i) {
        power *= x * x;
        double div = 2 * (double) i + 1;
        res += power / div;
    }

    return res;
}

int main() {
    std::cout << "Разница моей функции с библиотечной ~равна: " << fabs(atanh(0.5) - std::atanh(0.5)) << "\n";
}
