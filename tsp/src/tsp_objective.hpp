#pragma once

#include <vector>
#include <memory>
#include <ghost/objective.hpp>

class TSPObjective : public ghost::Minimize
{
	int _number_variables;
	std::vector< std::vector<double> > _matrix_distances;

public:
	TSPObjective( const std::vector<ghost::Variable>& variables,
	              int number_variables,
	              const std::vector< std::vector<double> >& matrix_distances );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
