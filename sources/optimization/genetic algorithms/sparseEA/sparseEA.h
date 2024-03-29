#include <list>
#include <map>
#include <string>
#include <time.h>
#include <algorithm>
#include <memory>
#include <array>
#include <random>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <variant>
#include <exception>
#include <numeric>
#include <utility>
#include <vector>

#include "../../../basic/math.h"
#include "../evolutionary.h"

/*
 *  article information :
 *	[1] Improved SparseEA for sparse large-scale multi-objective optimization problems. Complex & Intelligent Systems
 *	[2]
 */

#ifndef _math_optimization_evolutionary_sparseEA_
#define _math_optimization_evolutionary_sparseEA_
using Pointer = std::unique_ptr<double[], decltype(&math::free<double>)>;
Pointer create(size_t length);

class Individual
{
private:
	Pointer data_;
public:
	double *decisions, *objectives, *voilations, *masks;

public:
	Individual(size_t scale, size_t dimension, size_t constraint);
	~Individual();
};

class Reference : public Evolutionary::Selector<Individual>
{
private:
	size_t dimension_, scale_, constraint_, selection_;
	Pointer ideal_, interception_;

//	simplified reference plain
	std::list<std::tuple<Pointer, size_t, std::list<Individual*>>> associations_;

private:
	void dispense(size_t needed, std::list<Individual*>& elites, std::list<Individual*>& cirticals);

public:
	virtual std::list<std::list<Individual*>> sort(const std::list<Individual*>& population) const;
	virtual std::pair<std::list<Individual*>, std::list<Individual*>> select(const std::list<Individual*>& population);

public:
	Reference(const math::Optimizor::Configuration& configuration);
	virtual ~Reference() {}
};

class Reproducor : public Evolutionary::Reproducor<Individual>
{
private:
	size_t scale_, dimension_, *importances_;
	double cross_, mutation_, threshold_;
	Pointer upper_, lower_, integer_;
	math::Optimizor::Objective *function_;

private:
	std::mt19937_64 generator_;

private:
	virtual void check(Individual& individuals);
	virtual void cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter);
	virtual void mutate(Individual& individua);

private:
	virtual std::list<Individual*> reproduce(std::pair<std::list<Individual*>, std::list<Individual*>>&& population);

public:
	Reproducor(math::Optimizor::Configuration& configuration, size_t *importances);
	virtual ~Reproducor();
};

class Population : public Evolutionary::Population<Individual>
{
private:
	std::unique_ptr<Reference> selector_;
	std::unique_ptr<Reproducor> reproducor_;

public:
	virtual Evolutionary::Selector<Individual>& selector();
	virtual Evolutionary::Reproducor<Individual>& reproducor();

public:
	size_t scale, dimension, constraint, *importances;
	std::list<Individual*> individuals;

public:
	Population(math::Optimizor::Configuration& configuration);
	virtual ~Population();
};


class SparseEA : public Evolutionary::Evolutionary
{
private:
	std::unique_ptr<Population> population_;

protected:
	virtual void write(const char* filepath);
	virtual void evolve(size_t generation);
	virtual math::Optimizor::Result& optimize(math::Optimizor::Configuration& configuration);

public:
	virtual ~SparseEA() {}
};

extern "C" __declspec(dllexport) void* __cdecl construct();
#endif //!_math_optimization_evolutionary_sparseEA_