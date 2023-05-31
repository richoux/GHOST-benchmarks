#pragma once

#include <string>

#include <ghost/model_builder.hpp>

class BuilderQAP : public ghost::ModelBuilder
{
	int _number_variables;
	std::vector< std::vector<int> > _matrix_distances;
	std::vector< std::vector<int> > _matrix_flows;
	
public:
	BuilderQAP( int number_variables,
	            const std::vector< std::vector<int> >& matrix_distances,
	            const std::vector< std::vector<int> >& matrix_flows );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
