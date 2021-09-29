#include "knapsack_coefficients.hpp"

KSCoefficients::KSCoefficients()
	: AuxiliaryData(),
	  values( std::vector<int>{4,2,2,1,10} ),
	  weights( std::vector<int>{12,2,1,1,4} )
{ }

void KSCoefficients::required_update( const std::vector<ghost::Variable*>& variables, int index, int new_value ) { }
