#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

using namespace ghost;

class BuilderTarget : public ModelBuilder
{
public:
	BuilderTarget();

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
	void declare_auxiliary_data() override;
};
