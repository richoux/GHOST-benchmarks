#pragma once

#include <vector>
#include <ghost/factory_model.hpp>

#include "unit_data.hpp"
#include "constraint_stock.hpp"
#include "objective_maxground_dps.hpp"

using namespace ghost;
using namespace std;

class FactoryResource : public FactoryModel
{
protected:
	int _supply;
	
public:
	FactoryResource( int supply );
	virtual ~FactoryResource() = default;

	void declare_constraints() override;
	void declare_objective() override;
};

class FactoryTerran : public FactoryResource
{
public:
	FactoryTerran( int supply );
	void declare_variables() override;
	void declare_auxiliary_data() override;
};

class FactoryProtoss : public FactoryResource
{
public:
	FactoryProtoss( int supply );
	void declare_variables() override;
	void declare_auxiliary_data() override;
};

class FactoryZerg : public FactoryResource
{
public:
	FactoryZerg( int supply );
	void declare_variables() override;
	void declare_auxiliary_data() override;
};
