#include "data.hpp"

Data::Data( const std::vector<Variable>& variables, std::vector<UnitData> unit_data )
	: AuxiliaryData( variables ),
	  unit_data( unit_data )
{ }

void Data::update( const std::vector<Variable*>& variables, int index, int new_value ) { }
