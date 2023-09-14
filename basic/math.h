#include <array>
#include <cmath>
#include <cstdlib>
#include <memory>
#include <algorithm>
#include <functional>
#include <numeric>

#ifndef _MATH_BASIC_ARTHEMIC_
#define _MATH_BASIC_ARTHEMIC_
namespace math
{

    double* allocate(size_t length);
    void free(double* vector);
    void copy(size_t length, const double* sources, size_t step, double*, size_t increment);

    void add(size_t length, const double* left, const double* right, double* results);
    void sub(size_t length, const double* left, const double* right, double* results);
    void mul(size_t length, const double* left, const double* right, double* results);
    void div(size_t length, const double* left, const double* right, double* results);
    void pow(size_t length, const double* base, const double * power, double* results);
    void exp(size_t length, const double* exponential, double* results);

    void addI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc);
    void subI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc);
    void mulI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc);
    void divI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc);
    void powI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc);
    void expI(size_t length, const double* expontential, size_t einc, double* results, size_t inc);

    void scal(size_t length, double factor, double* vector, size_t inc);
    double dot(size_t length, const double* left, size_t linc, const double* right, size_t rinc);
    void xpby(size_t length, const double* left, size_t linc, double factor, double* right, size_t rinc);
}
#endif //! _MATH_BASIC_ARTHEMIC_