#include "../Distribution.h"
#include <limits>
#include "windows.h"

/*
#ifndef _Density_
#define _Density_
template<typename T>
class Density
{
private:
    size_t row, column;
    std::vector<T> data;

private:
    bool Check(const Density& A)
    {
        if (row != A.row) { MessageBox("row not equal!"); return false; }
        if (column != A.column) {
            MessageBox("column not equal!"); return false;
        }
        return true;
    }

    bool Check(const size_t&r, const size_t& c)
    {
        if (r > row) { MessageBox("row exceeds!"); return false; }
        if (c > column) { MessageBox("column exceeds!"); return false; }
        return true;
    }

public:

    Density& operator + (const Density& A)
    {
        if (Check(A))
        {
            for (size_t i = 0; i < data.size(); ++i)
            {
                data[i] += A.data[i];
            }
        }

        return *this;
    }

    Density& operator - (const Density& A)
    {
        if (Check(A))
        {
            for (size_t i = 0 i < data.size(); ++i)
            {
                data[i] -= A.data[i];
            }
        }
        return *this;
    }

//row domain, begin with 1
    T& operator ()(const size_t& r, const size_t& c)
    {
        if (Check(r, c)) { return data[column * (r - 1) + (c - 1)]; }
    }

    std::vector<T>& Row(const size_t& r)
    {
        if (Check(r, 0))
        {
            return 
        }
    }
//

};
#endif // !_Matrix_
*/

#ifndef _KDE_
#define _KDE_
//Kernel density estimation via diffusion
class KDE
{
private:
//    float * mu, sig, w;
    size_t size;
//    float MAX, MIN, scaling;

private:
    std::vector<float> mean_, variation_, weight_;
    std::function<(size_t, const float *, float *)> kernel_;
    float maximum_, minmum_, scaling_;

private:
    void SetKernel(const std::string& function);

private:
    virtual void density(size_t length, const float * X, float * Y);
    virtual void probability(size_t length, const float * X, float * Y);

public:
    KDE (const std::vector<float>& X);
};
#endif