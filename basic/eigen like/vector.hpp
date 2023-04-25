#include <vector>
#include <cassert>
#include <concepts>
#include <type_traits>

#ifndef _MATH_BASIC_EIGENLIKE_VECTOR_
#define _MATH_BASIC_EIGENLIKE_VECTOR_

template<typename T>
concept Value = std::unsigned_integral<T>;

template<typename T>
class Vector
{
public:
	using base = typename T;

private:
	std::vector<T> data_;

public:
	size_t size() const { return data_.size(); };
	T operator[] (size_t index) const;
	T& operator[] (size_t index);

public:
	auto begin() { return data_.begin(); }
	auto end() { return data_.end(); }

	auto insert(auto pos, auto begin, auto end) { return data_.insert(pos, begin, end); }
	auto insert(auto pos, const Vector<T>& vector) { return data_.insert(pos, vector.begin(), vector.end()); }

public:
	Vector() {};
	Vector(size_t length, T value = 0) { data_.resize(length, value); };
	Vector(const Vector<T>&) = default;
	template<class Expression> Vector(const Expression& expression);

public:
	void resize(auto length, auto value = 0) { data_.resize(length, value);};

public:
	template<class Expression> Vector& operator = (const Expression& expression);
};

template<typename T>
template<typename Expression>
Vector<T>::Vector(const Expression& expression) {
/*
	data_.resize(expression.size());

	for (size_t i = 0; i < data_.size(); ++i) {
		data_[i] = expression[i];
	}
	*/
}

template<typename T>
T Vector<T>::operator [] (size_t index) const {
	return data_[index];
}

template<typename T>
T& Vector<T>::operator [] (size_t index) {
	return data_[index];
}

template<typename T>
template<typename Expression>
Vector<T>& Vector<T>::operator = (const Expression& expression) {
	data_.resize(expression.size());

	for (size_t i = 0; i < data_.size(); ++i) {
		data_[i] = expression[i];
	}

	return* this;
}

template<class L, class R>
class Add
{
public:
	const L& lhs;
	const R& rhs;

	using base = L::base;

	size_t size() const { return lhs.size(); };
public:
	base operator [](size_t index) const {
		return lhs[index] + rhs[index];
	}
};

template<class L, class R>
class Substract
{
public:
	const L& lhs;
	const R& rhs;

	using base = L::base;

	size_t size() const { return lhs.size(); };
public:
	auto operator [](size_t index) const {
		return lhs[index] - rhs[index];
	}
};

template<class L, class R>
class Multiply
{
public:
	const L& lhs;
	const R& rhs;

	using base = L::base;

	size_t size() const { return lhs.size(); };


public:
	base operator [](size_t index) const {
		return lhs[index] * rhs[index];
	}
};

template<class L, class R>
class Divide
{
public:
	const L& lhs;
	const R& rhs;

	size_t size() const { return lhs.size(); };
	using base = L::base;


public:
	auto operator [](size_t index) const {
		return lhs[index] / rhs[index];
	}
};

template<typename T, class V>
class Scale
{
public:
	const T& scalar;
	const V& vector;
	size_t size() const { return vector.size(); };

public:
	T operator [](size_t index) const {
		return  scalar * vector[index];
	}
};

template<typename T, class V>
class Power
{
public:
	const T& exponent;
	const V& base;
	size_t size() const { return base.size(); };

public:
	T operator [](size_t index) const {
		return  pow(base[index], exponent);
	}
};

template<class L, class R>
auto operator + (const L& lhs, const R& rhs) {
	return Add<L, R>{lhs, rhs};
}

template<class L, class R>
auto operator - (const L& lhs, const R& rhs) {
	return Substract<L, R>{lhs, rhs};
}

template<class L, class R>
auto operator * (const L& lhs, const R& rhs) {
	return Multiply<L, R>{lhs, rhs};
}

/*
template<typename T, class V, std::enable_if<std::is_floating_point<T>::value>>
auto operator * (const T& lhs, const V& rhs) {
	return Scale<T, V>{lhs, rhs};
}
*/

template<class L, class R>
auto operator / (const L& lhs, const R& rhs) {
	return Divide<L, R>{lhs, rhs};
}

template<class V, typename T>
auto operator ^ (const V& lhs, const T& rhs) {
	return Power<T, V>{rhs, lhs};
}

template<typename T>
T dot(const Vector<T>& lhs, const Vector<T>& rhs) {
	return 0;
}
#endif //!_MATH_OPTIMIZATION_UNSGA_MATRIX_

