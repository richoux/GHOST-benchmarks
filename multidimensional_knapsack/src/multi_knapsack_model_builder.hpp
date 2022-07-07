#pragma once

#include <vector>
#include <memory>
#include <ghost/model_builder.hpp>

class MultiKSBuilder : public ghost::ModelBuilder
{
	int _number_variables;
	int _number_constraints;
	std::vector<std::vector<int>> _coefficients;
	std::vector<int> _capacities;
	std::vector<int> _values;
	
public:
	MultiKSBuilder( int number_variables,
	                int number_constraints,
	                const std::vector<std::vector<int>>& coefficients,
	                const std::vector<int>& capacities,
	                const std::vector<int>& values );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
