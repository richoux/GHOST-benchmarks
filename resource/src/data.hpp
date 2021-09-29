#pragma once

#include <ghost/auxiliary_data.hpp>
#include "unit_data.hpp"

using namespace ghost;

class Data : public AuxiliaryData
{
public:
	std::vector<UnitData> unit_data;
	
	Data( const std::vector<Variable>& variables, std::vector<UnitData> unit_data );

	void required_update( const std::vector<Variable*>& variables, int index, int new_value ) override;
};
