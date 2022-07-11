#pragma once

#include <memory>
#include <ghost/model_builder.hpp>

class BuilderVC : public ghost::ModelBuilder
{
	int _constant;
	std::vector<int> _all_variables;
	
public:
	BuilderVC();
	
	void declare_variables() override;
	void declare_constraints() override;
};
