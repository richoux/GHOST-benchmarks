#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

class BuilderBO : public ModelBuilder
{
	//std::vector< std::pair<std::string, int> > _inputs;
	std::map< std::string, std::pair<int, int> > _goals;
	int _domain_size;

	// void makeGoals( std::pair<std::string, int> &input, std::map< std::string, std::pair<int, int> > &goals );
	void makeGoals( std::pair<std::string, int> &input, bool exhaustive_inputs );
	void rec_makeGoals( const Action &action, int count );
	
public:
	BuilderTarget( const std::vector< std::pair<std::string, int> >& inputs );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
