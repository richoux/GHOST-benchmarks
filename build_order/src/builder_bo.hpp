#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

#include "actionData.hpp"

class BuilderBO : public ghost::ModelBuilder
{
	//std::vector< std::pair<std::string, int> > _inputs;
	std::map< std::string, std::pair<int, int> > _goals;
	int _domain_size;
	bool _exhaustive_inputs;
	
	// void makeGoals( std::pair<std::string, int> &input, std::map< std::string, std::pair<int, int> > &goals );
	void makeGoals( std::string, int );
	void rec_makeGoals( const ActionData &action, int count );
	
public:
	BuilderBO( const std::vector< std::pair<std::string, int> >& inputs, bool exhaustive_inputs = true );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
