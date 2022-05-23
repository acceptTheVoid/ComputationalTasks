#include <iostream>
#include <cmath>
#include <functional>
#include <vector>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace math {
    typedef std::function<double(double)> Expression;

    namespace _1 {
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
    }

    namespace _2 {
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
    }

    namespace _3 {
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
    }

    namespace _4 {
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
    }

    /*
     * Дружище, внимание
     * Всю секцию ниже я уже нихуя не понимал так как у нас такой хуйни просто не было
     * Так что я ее просто спиздил с какого-то сайта и сделал чуть более читабельной и модульной
     * Надеюсь ты не сойдешь с ума пытаясь познать эту тему
     * */

    namespace _5 {
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
    }
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

/// Выражение из 3
double equation(double x) {
    return 2 * x * x * x + 5 * x * x - x - 6;
}

/// Производная выражения из 3
double derivative(double x) {
    return 6 * x * x + 10 * x - 1;
}

/// Выражение из 4
double equation_2(double x) {
    return 3 * x * x * x * x + 5 * x * x * x + 3 * x * x - 18 * x - 9;
}

/// Выражение из 5
double equation_3(math::_5::vec2 vec) {
    return -vec.x * vec.x * vec.y - 2 * vec.x * vec.y + vec.y * vec.y + 8 * vec.y - 1;
}

/// Градиент выражения из 5
math::_5::vec2 grad(math::_5::vec2 vec) {
    return { -2 * (vec.x + 1) * vec.y, -(vec.x * vec.x) - 2 * vec.x + 2 * vec.y + 8 };
}

typedef std::function<double(double, double, math::Expression, int)> IntegralMethod;
// Метод рунге который вроде было необязательно реализовывать или надо было хз
// Я в любом случае его сделал
double runge(
    double a,
    double b,
    IntegralMethod const &im,
    math::Expression const &i
) {
    int partitions = 10;

    return fabs(im(a, b, i, partitions * 2) - im(a, b, i, partitions));
}

int main() {
    // Примеры для интегралов
    /*std::cout << math::integral_rec(0.2, 0.6, integral_2) << "\n";
    std::cout << math::integral_trap(0.2, 0.6, integral_2) << "\n";
    std::cout << math::integral_simp(0.2, 0.6, integral_2) << "\n";
    std::cout << runge(0.2, 0.6, math::integral_simp, integral_2) << "\n";*/

    // Примеры для нелинейных уравнений
    /*std::cout << math::newtone_method(0, 0.0001, equation, derivative) << "\n";
    std::cout << math::dichtomia(-3, -1.75, 0.0001, equation) << "\n";*/

    // Примеры для нахождения локального минимума и количества итераций
    /*unsigned counter = 0;

    std::cout << math::golden_ratio(-10, 10, 0.001, equation_2, &counter) << "\n";
    std::cout << counter << "\n";*/

    // Примеры для нахождения локального минимума в пространстве и количества итераций
    /*unsigned counter = 0;

    auto aboba = math::_5::GradDown({ 0, 0 }, 0.001, equation_3, grad, &counter);
    std::cout << aboba.x << " " << aboba.y << " " << counter << "\n";*/
}

#pragma clang diagnostic pop
