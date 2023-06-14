#include <iostream>
#include <fstream>
#include "unsga.h"

class Objective : public Optimizor::Objective
{
private:
	size_t decisions_ = 2;

public:
	virtual const double* operator() (const double* decisions, double* objectives)
	{
		objectives[0] = 0;

		for (long i = 0; i < decisions_; ++i)
		{
			objectives[0] += pow(decisions[i] - 1 / sqrt(decisions_), 2);
		}

		objectives[0] = 1 - exp(-objectives[0]);

		objectives[1] = 0;

		for (long i = 0; i < decisions_; ++i)
		{
			objectives[1] += pow(decisions[i] + 1 / sqrt(decisions_), 2);
		}

		objectives[1] = 1 - exp(-objectives[1]);

	}
};

class Constraint : public Optimizor::Constraint
{
public:
	virtual const double* operator() (const double* decisions, const double* objectives, double* voilations = nullptr) {}
};

class Configurations : public Optimizor::Configuration
{
private:
	const size_t decisions_ = 2;
	const size_t objectives_ = 2;
	const double upper_[2] = {1, 1};
	const double lower_[2] = {-1, -1};
	const bool integer_[2] = { false, false };

public:
	virtual size_t scales() const
	{
		return decisions_;
	}

	virtual size_t dimensions() const
	{
		return objectives_;
	}

	virtual size_t constraints() const
	{
		return 0;
	}

	virtual const double* uppers() const
	{
		return upper_;
	}

	virtual const double* lowers() const
	{
		return lower_;
	}

	virtual const bool* integers() const
	{
		return integer_;
	}
};


int main()
{
	std::unique_ptr<Optimization::Configuration> config = std::make_unique<Configurations>();
	config->objective = std::make_unique<Objective>();
	config->constraint = std::make_unique<Constraint>();

	(*config)["cross"] = 0.8f;
	(*config)["mutation"] = 0.8f;
	(*config)["maximum"] = size_t(100);
	(*config)["division"] = size_t(10);
	(*config)["population"] = size_t(1000);

	std::unique_ptr<Optimization::Optimizer> optimizer = std::make_unique<UNSGA>();
	auto results = optimizer->Optimize(config.get());
	results->Write("results.txt");
	results = nullptr;
	std::cout << "hello" << std::endl;
	return 0;
};