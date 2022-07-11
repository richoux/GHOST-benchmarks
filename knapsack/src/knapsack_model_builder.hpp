#pragma once

#include <vector>
#include <memory>
#include <ghost/model_builder.hpp>

class KSBuilder : public ghost::ModelBuilder
{
	std::vector<double> _weights;
	std::vector<double> _values;
	
public:
	KSBuilder( const std::vector<double>& weights,
	           const std::vector<double>& values );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
