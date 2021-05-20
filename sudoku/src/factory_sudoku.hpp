#pragma once

#include <memory>
#include <ghost/model.hpp>

#include "all-diff.hpp"
#include "fix_value.hpp"

using namespace ghost;
using namespace std;

class FactorySudoku : public FactoryModel
{
	int _instance_size;
	int _side_size;
	bool _hard_instance;
	vector< vector< Variable > > _rows;
  vector< vector< Variable > > _columns;
  vector< vector< Variable > > _squares;

public:
	FactorySudoku( const std::vector<Variable>& variables, 
	               int instance_size,
	               bool hard_instance );
	
	std::shared_ptr<Model> make_model() override;
};
