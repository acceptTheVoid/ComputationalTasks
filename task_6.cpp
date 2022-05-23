#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// Условие окончания
bool converge(double const *xk, double const *xkp, size_t n, double eps) {
    double norm = 0;
    for (int i = 0; i < n; i++) {
        norm += (xk[i] - xkp[i]) * (xk[i] - xkp[i]);
    }
    return (sqrt(norm) < eps);
}

double okr(double x, double eps) {
    int i = 0;
    double neweps = eps;
    while (neweps < 1) {
        i++;
        neweps *= 10;
    }
    int okr = pow(double(10), i);
    x = int(x * okr + 0.5) / double(okr);

    return x;
}

bool diagonal(double **a, size_t n) {
    int i, j, k = 1;
    double sum;
    for (i = 0; i < n; i++) {
        sum = 0;
        for (j = 0; j < n; j++) sum += abs(a[i][j]);
        sum -= abs(a[i][i]);
        if (sum > a[i][i]) {
            k = 0;
            cout << a[i][i] << " < " << sum << endl;
        } else {
            cout << a[i][i] << " > " << sum << endl;
        }
    }

    return (k == 1);
}

int main() {
    ifstream fin("input.txt");
    size_t size; fin >> size;
    double eps; fin >> eps;

    double **a, *b, *cur, *prev;
    a = new double *[size];
    *a = new double [size * size];
    for(size_t i = 1; i != size; ++i) {
        a[i] = a[i - 1] + size;
    }
    b = new double [size];
    cur = new double [size];
    prev = new double [size];

    for(size_t i = 0; i != size; ++i) {
        for(size_t j = 0; j != size; ++j) {
            fin >> a[i][j];
        }
    }

    for(size_t i = 0; i != size; ++i) {
        fin >> b[i];
    }

    /*
    Ход метода, где:
    a[n][n] - Матрица коэффициентов
    x[n], p[n] - Текущее и предыдущее решения
    b[n] - Столбец правых частей
    Все перечисленные массивы вещественные и
    должны быть определены в основной программе,
    также в массив x[n] следует поместить начальное
    приближение столбца решений (например, все нули)
    */

    for(int i = 0; i < size; i++)
        cur[i] = 1;

    if(diagonal(a, size)) {
        do {
            for(size_t i = 0; i < size; i++)
                prev[i] = cur[i];
            for(size_t i = 0; i < size; i++) {
                double var = 0;
                for (size_t j = 0; j < size; j++)
                    if(j != i) var += (a[i][j] * cur[j]);

                cur[i] = (b[i] - var) / a[i][i];
            }
        } while(!converge(cur, prev, size, eps));

        cout << "Solution: " << endl << endl;
        for(size_t i = 0; i < size; i++) cout << "x" << i << " = " << okr(cur[i], eps) << "\n";
    } else {
        cout << "System doesn't have a solution" << endl;
    }

    delete[] prev;
    delete[] cur;
    delete[] b;
    delete[] *a;
    delete[] a;

    return 0;
}
