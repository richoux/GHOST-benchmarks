#pragma once

#include <vector>
#include <memory>
#include <ghost/objective.hpp>

class QAPObjective : public ghost::Minimize
{
	int _number_variables;
	std::vector< std::vector<int> > _matrix_distances;
	std::vector< std::vector<int> > _matrix_flows;

public:
	QAPObjective( const std::vector<ghost::Variable>& variables,
	              int number_variables,
	              const std::vector< std::vector<int> >& matrix_distances,
	              const std::vector< std::vector<int> >& matrix_flows );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
