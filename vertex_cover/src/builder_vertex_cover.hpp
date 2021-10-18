#pragma once

#include <memory>
#include <ghost/model_builder.hpp>

#include "linear-geq.hpp"
#include "linear-leq.hpp"

using namespace ghost;
using namespace std;

class BuilderVC : public ModelBuilder
{
	int _constant;
	std::vector<int> _all_variables;
	
public:
	BuilderVC();
	
	void declare_variables() override;
	void declare_constraints() override;
};
