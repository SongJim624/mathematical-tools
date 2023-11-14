#define __USING_MKL__

#include <tuple>
#include <utility>
#include <cmath>
#include <memory>
#include <functional>

#ifdef __USING_MKL__
#include <mkl.h>
#endif

#ifndef _MATH_BLAS_
#define _MATH_BLAS_
//  allocator of array
namespace math
{
#if defined __INTEL_MKL__
    template<typename T>
    using pointer = std::unique_ptr<T[], void(*)(void*)>;

    template<typename T>
    pointer<T> allocate(size_t size)
    {
        return pointer<T>((T*)MKL_calloc(size, sizeof(T), 64), &MKL_free);
    }

#else
    template<typename T>
    using pointer = std::unique_ptr<T[], void(*)(void*)>;

    template<typename T>
    pointer<T> allocate(size_t size)
    {
        return pointer<T>((T*)std::calloc(size, sizeof(T)), &std::free);
    }
#endif

    template<typename T, typename Operation>
    void operate(Operation operation, size_t size, const T* operand, size_t oinc, T* results, size_t inc)
    {
        for (size_t i = 0; i < size; ++i)
        {
            results[i * inc] = operation(operand[i * oinc]);
        }
    }

    template<typename T, typename Operation>
    void operate(Operation operation, size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* results, size_t inc)
    {
        for (size_t i = 0; i < size; ++i)
        {
            results[i * inc] = operation(lhs[i * linc], rhs[i * rinc]);
        }
    }
};

//  copy fucntion
namespace math
{
    template<typename T>
    void copy(size_t size, const T* source, size_t sinc, T* destination, size_t dinc)
    {
        operate([](T value) { return value; }, size, source, sinc,  destination, dinc);
    }

#ifdef  __INTEL_MKL__
    template<>
    void copy(size_t size, const std::pair<const double*, size_t>& source, std::pair<double*, size_t> destination)
    {
        cblas_dcopy(size, source.first, source.second, destination.first, destination.second);
    }

    template<>
    void copy(size_t size, const std::pair<const float*, size_t>& source, std::pair<float*, size_t> destination)
    {
        cblas_scopy(size, source.first, source.second, destination.first, destination.second);
    }
#endif
}

//  add
namespace math
{
    template<typename T>
    void add(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::plus<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void add(size_t size, const std::pair<const double*, size_t>& left, const std::pair<const double*, size_t>& right, std::pair<double*, size_t> results)
    {
        vdAddI(size, left.first, left.second, right.first, right.second, results.first, results.second);
    }

    template<>
    void add(size_t size, const std::pair<const float*, size_t>& left, const std::pair<const float*, size_t>& right, std::pair<float*, size_t> results)
    {
        vsAddI(size, left.first, left.second, right.first, right.second, results.first, results.second);
    }
#endif

    template<typename T>
    void add(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        add(size, &lhs, 0 , rhs, rinc, res, inc);
    }

    template<typename T>
    void add(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        add(size, lhs, linc, &rhs, 0, res, inc);
    }
}


//  substract
namespace math
{
    template<typename T>
    void sub(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::minus<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void sub(size_t size, const std::pair<const double*, size_t>& lhs, const std::pair<const double*, size_t>& rhs, std::pair<double*, size_t> results)
    {
        vdSubI(size, lhs.first, lhs.second, rhs.first, rhs.second, results.first, results.second);
    }

    template<>
    void sub(size_t size, const std::pair<const float*, size_t>& lhs, const std::pair<const float*, size_t>& rhs, std::pair<float*, size_t> results)
    {
        vsSubI(size, lhs.first, lhs.second, rhs.first, rhs.second, results.first, results.second);
    }
#endif

    template<typename T>
    void sub(size_t size, T lhs, const std::pair<const T*, size_t>& rhs, std::pair<T*, size_t> results)
    {
        sub(size, { &lhs, 0 }, rhs, results);
    }

    template<typename T>
    void sub(size_t size, const std::pair<const T*, size_t>& lhs, T rhs, std::pair<T*, size_t> results)
    {
        sub(size, lhs, { &rhs, 0 }, results);
    }
}

//  multiply
namespace math
{
    template<typename T>
    void mul(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::multiplies<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void mul(size_t size, const std::pair<const double*, size_t>& lhs, const std::pair<const double*, size_t>& rhs, std::pair<double*, size_t> results)
    {
        vdMulI(size, lhs.first, lhs.second, rhs.first, rhs.second, results.first, results.second);
    }

    template<>
    void mul(size_t size, const std::pair<const float*, size_t>& lhs, const std::pair<const float*, size_t>& rhs, std::pair<float*, size_t> results)
    {
        vsMulI(size, lhs.first, lhs.second, rhs.first, rhs.second, results.first, results.second);
    }
#endif

    template<typename T>
    void mul(size_t size, T lhs, const std::pair<const T*, size_t>& rhs, std::pair<T*, size_t> results)
    {
        mul(size, { &lhs, 0 }, rhs, results);
    }

    template<typename T>
    void mul(size_t size, const std::pair<const T*, size_t>& lhs, T rhs, std::pair<T*, size_t> results)
    {
        mul(size, lhs, { &rhs, 0 }, results);
    }
}

//  divide
namespace math
{
    template<typename T>
    void div(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate(std::divides<T>(), size, lhs, linc, rhs, rinc, res, inc);
    }


#ifdef __INTEL_MKL__
    template<>
    void div(size_t size, const std::pair<const double*, size_t>& lhs, const std::pair<const double*, size_t>& rhs, std::pair<double*, size_t> results)
    {
        vdDivI(size, lhs.first, lhs.second, rhs.first, rhs.second, results.first, results.second);
    }

    template<>
    void div(size_t size, const std::pair<const float*, size_t>& lhs, const std::pair<const float*, size_t>& rhs, std::pair<float*, size_t> results)
    {
        vsDivI(size, lhs.first, lhs.second, rhs.first, rhs.second, results.first, results.second);
    }
#endif

    template<typename T>
    void div(size_t size, T lhs, const std::pair<const T*, size_t>& rhs, std::pair<T*, size_t> results)
    {
        div(size, { &lhs, 0 }, rhs, results);
    }

    template<typename T>
    void div(size_t size, const std::pair<const T*, size_t>& lhs, T rhs, std::pair<T*, size_t> results)
    {
        div(size, lhs, { &rhs, 0 }, results);
    }
}

// power
namespace math
{
    template<typename T>
    void pow(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        operate([](T base, T power) { return std::pow(base, power); }, size, lhs, linc, rhs, rinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void pow(size_t size, const std::pair<const double*, size_t>& left, const std::pair<const double*, size_t>& right, std::pair<double*, size_t> results)
    {
        vdPowI(size, left.first, left.second, right.first, right.second, results.first, results.second);
    }

    template<>
    void pow(size_t size, const std::pair<const float*, size_t>& left, const std::pair<const float*, size_t>& right, std::pair<float*, size_t> results)
    {
        vsPowI(size, left.first, left.second, right.first, right.second, results.first, results.second);
    }
#endif

    template<typename T>
    void pow(size_t size, T lhs, const T* rhs, size_t rinc, T* res, size_t inc)
    {
        pow(size, &lhs, 0, rhs, rinc, res, inc);
    }

    template<typename T>
    void pow(size_t size, const T* lhs, size_t linc, T rhs, T* res, size_t inc)
    {
        pow(size, lhs, linc, &rhs, 0, res, inc);
    }
}

//  exponential
namespace math
{
    template<typename T>
    void exp(size_t size, const T* operand, size_t oinc, T* res, size_t inc)
    {
        operate([](T exponent) { return std::exp(exponent); }, size, operand, oinc, res, inc);
    }

#ifdef __INTEL_MKL__
    template<>
    void exp(size_t size, const std::pair<const double*, size_t>& operand, std::pair<double*, size_t> results)
    {
        vdExpI(size, operand.first, operand.second, results.first, results.second);
    }

    template<>
    void exp(size_t size, const std::pair<const float*, size_t>& operand, std::pair<float*, size_t> results)
    {
        vsExpI(size, operand.first, operand.second, results.first, results.second);
    }
#endif
}

namespace math
{
    template<typename T>
    void scal(size_t size, T factor, T* operand, size_t oinc)
    {
        operate([factor](T value) { return factor * value; }, size, operand, oinc, operand, oinc);
    }

#ifdef __INTEL_MKL__
    template<>
    void scal(size_t size, double factor, std::pair<double*, size_t> operand)
    {
        cblas_dscal(size, factor, operand.first, operand.second);
    }

    template<>
    void scal(size_t size, float factor, std::pair<float*, size_t> operand)
    {
        cblas_sscal(size, factor, operand.first, operand.second);
    }
#endif
}

namespace math
{
    template<typename T>
    void axpy(size_t size, T factor, const T* lhs, size_t linc, T* rhs, size_t rinc)
    {
        operation([factor](T l, T r) { return l + factor * r; }, size, lhs, linc, rhs, rinc, rhs, rinc);
    }

#ifdef __INTEL_MKL__
    template<>
    void axpy(size_t size, double factor, const std::pair<const double*, size_t>& lhs, std::pair<double*, size_t> rhs)
    {
        cblas_daxpy(size, factor, lhs.first, lhs.second, rhs.first, rhs.second);
    }

    template<>
    void axpy(size_t size, float factor, const std::pair<const float*, size_t>& lhs, std::pair<float *, size_t > rhs)
    {
        cblas_saxpy(size, factor, lhs.first, lhs.second, rhs.first, rhs.second);
    }
#endif
}

namespace math
{
    template<typename T>
    void axpby(size_t size, T alpha, const T* lhs, size_t linc, T beta, T* rhs, size_t rinc)
    {
        operate([alpha, beta](T l, T r) { return alpha * l + beta * r; }, size, lhs, linc, rhs, rinc, rhs, rinc);
    }

#ifdef __INTEL_MKL__
    template<>
    void axpby(size_t size, double alpha, const std::pair<const double*, size_t>& lhs, double beta, std::pair<double*, size_t> rhs)
    {
        cblas_daxpby(size, alpha, lhs.first, lhs.second, beta, rhs.first, rhs.second);
    }

    template<>
    void axpby(size_t size, float alpha, const std::pair<const float*, size_t>& lhs, float beta, std::pair<float*, size_t > rhs)
    {
        cblas_saxpby(size, alpha, lhs.first, lhs.second, beta, rhs.first, rhs.second);
    }
#endif
}

namespace math
{
    template<typename T>
    T dot(size_t size, const T* lhs, size_t linc, const T* rhs, size_t rinc)
    {
        T results = 0;
        operate([&results](T l, T r) { return results + l * r; }, size, lhs, linc, rhs, rinc, &results, 0);
        return results;
    }

#ifdef __INTEL_MKL__
    template<>
    double dot(size_t length, const std::pair<const double*, size_t>& lhs, const std::pair<const double*, size_t>& rhs)
    {
        return cblas_ddot(length, lhs.first, lhs.second, rhs.first, rhs.second);
    }

    template<>
    float dot(size_t length, const std::pair<const float*, size_t>& lhs, const std::pair<const float*, size_t>& rhs)
    {
        return cblas_sdot(length, lhs.first, lhs.second, rhs.first, rhs.second);
    }
#endif
}

/*
namespace math
{
    template<typename T>
    void gesv(size_t row, size_t column, const std::pair<const T*, size_t>& matrix, const std::pair<const T*, size_t> vector, std::pair<T*, size_t> results)
    {
    }

    template<typename T>
    void gesv(size_t scale, size_t batch, const std::pair<const double*, size_t>& matrix, const std::pair<const double*, size_t> vectors, std::pair<double*, size_t> results)
    {
        int pivots = new int[scale];

        dgesv(CblasRowMajor, scale, batch, matrix.first, lapack_int lda, pivots, vectors.first, lapack_int ldb);
        delete[] pivots;
    }

#ifdef __INTEL_MKL__
    template<>
    void gesv(size_t row, size_t column, const std::pair<const T*, size_t>& matrix, const std::pair<const T*, size_t> vector, std::pair<T*, size_t> results)
    {}
#endif
}
*/
#endif //! _MATH_BLAS_