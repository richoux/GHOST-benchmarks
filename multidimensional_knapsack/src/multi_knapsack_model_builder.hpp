#pragma once

#include <vector>
#include <memory>
#include <ghost/model_builder.hpp>

class MultiKSBuilder : public ghost::ModelBuilder
{
	int _number_variables;
	int _number_constraints;
	std::vector<std::vector<double>> _coefficients;
	std::vector<int> _capacities;
	std::vector<double> _values;
	
public:
	MultiKSBuilder( int number_variables,
	                int number_constraints,
	                const std::vector<std::vector<double>>& coefficients,
	                const std::vector<int>& capacities,
	                const std::vector<double>& values );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
