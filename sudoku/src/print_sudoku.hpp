#pragma once

#include <sstream>
#include <vector>

#include "ghost/misc/print.hpp"
#include "ghost/variable.hpp"

using namespace std;
using namespace ghost;

class PrintSudoku : public Print
{
public:
	stringstream print_candidate( const std::vector<Variable>& variables ) const override;
};
