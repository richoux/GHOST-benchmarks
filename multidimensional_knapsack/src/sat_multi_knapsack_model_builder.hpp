#pragma once

#include <vector>
#include <memory>
#include <ghost/model_builder.hpp>

class SatMultiKSBuilder : public ghost::ModelBuilder
{
	int _number_variables;
	int _number_constraints;
	int _optimal;
	std::vector<std::vector<int>> _coefficients;
	std::vector<int> _capacities;
	std::vector<int> _values;
		
public:
	SatMultiKSBuilder( int number_variables,
	                   int number_constraints,
	                   int optimal,
	                   const std::vector<std::vector<int>>& coefficients,
	                   const std::vector<int>& capacities,
	                   const std::vector<int>& values );

	void declare_variables() override;
	void declare_constraints() override;
};
