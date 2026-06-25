#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

#include "unitType.hpp"
#include "unitData.hpp"

using namespace ghost;

class BuilderTarget : public ModelBuilder
{
	std::vector<UnitType> _my_units;
	int _number_enemies;
	std::vector<UnitData> _my_army;
	std::vector<UnitData> _enemies;
	
	void create_variable( UnitType type );

public:
	BuilderTarget( const std::vector<UnitType>& my_units,
	               int number_enemies,
	               const std::vector<UnitData>& my_army,
	               const std::vector<UnitData>& enemies );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
