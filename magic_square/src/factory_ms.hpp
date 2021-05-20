#pragma once

#include <memory>
#include <ghost/model.hpp>

#if defined HAMMING
#include "linear-eq_hamming.hpp"
#else
#include "linear-eq_num.hpp"
#endif

using namespace ghost;
using namespace std;

class FactoryMagicSquare : public FactoryModel
{
	int _instance_size;
	int _nb_vars;
	int _constant;
	vector< vector< Variable > > _rows;
  vector< vector< Variable > > _columns;
  vector< vector< Variable > > _diagonals;

public:
	FactoryMagicSquare( const std::vector<Variable>& variables, 
	                    int instance_size );
	
	std::shared_ptr<Model> make_model() override;
};
