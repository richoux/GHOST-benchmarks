#pragma once

#include <string>

#include <ghost/model_builder.hpp>

class BuilderTSP : public ghost::ModelBuilder
{
	int _number_variables;
	std::vector< std::vector<double> > _matrix_distances;
	
public:
	BuilderTSP( int number_variables, const std::vector< std::vector<double> >& matrix_distances );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
