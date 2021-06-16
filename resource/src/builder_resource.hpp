#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

#include "unit_data.hpp"
#include "constraint_stock.hpp"
#include "objective_maxground_dps.hpp"

using namespace ghost;
using namespace std;

class BuilderResource : public ModelBuilder
{
protected:
	int _supply;
	
public:
	BuilderResource( int supply );
	virtual ~BuilderResource() = default;

	void declare_constraints() override;
	void declare_objective() override;
};

class BuilderTerran : public BuilderResource
{
public:
	BuilderTerran( int supply );
	void declare_variables() override;
	void declare_auxiliary_data() override;
};

class BuilderProtoss : public BuilderResource
{
public:
	BuilderProtoss( int supply );
	void declare_variables() override;
	void declare_auxiliary_data() override;
};

class BuilderZerg : public BuilderResource
{
public:
	BuilderZerg( int supply );
	void declare_variables() override;
	void declare_auxiliary_data() override;
};
