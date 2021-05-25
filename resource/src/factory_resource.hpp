#pragma once

#include <memory>
#include <ghost/model.hpp>

#include "unit_data.hpp"
#include "constraint_stock.hpp"
#include "objective_maxground_dps.hpp"

using namespace ghost;
using namespace std;

class FactoryResource : public FactoryModel
{
	vector< UnitData > _unit_data;
	
public:
	FactoryResource( const vector<Variable>& variables, const vector< UnitData >& unit_data );
	
	shared_ptr<Model> make_model() override;
};
