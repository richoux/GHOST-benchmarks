#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

#include "unitType.hpp"
#include "unitData.hpp"

using namespace ghost;

class BuilderTarget : public ModelBuilder
{
	std::vector<UnitType> _my_units;
	std::vector<UnitData> _my_army;
	std::vector<UnitData> _enemies;
	size_t _number_enemies;
	
	void create_variable_from_type( UnitType type );

public:
	BuilderTarget( const std::vector<UnitType>& my_units,
	               const std::vector<UnitData>& my_army,
	               const std::vector<UnitData>& enemies );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
