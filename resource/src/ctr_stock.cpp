#include <algorithm>
#include <exception>

#include "ctr_stock.hpp"

using namespace std;
using namespace ghost;

Stock::Stock( vector< Variable >& variables,
	      int quantity,
	      ResourceType type,
	      const vector< UnitData >& unit_data )
  : Constraint	( variables ),
    _quantity	( quantity ),
    _type	( type ),
    _unit_data	( unit_data )
{ }

double Stock::required_cost() const 
{
  double sum = 0.;
  double costValue;

  for( int i = 0 ; i < _unit_data.size() ; ++i )
  {
    switch( _type )
    {
    case Mineral:
      costValue = _unit_data[i].get_mineral();
      break;
    case Gas:
      costValue = _unit_data[i].get_gas();
      break;
    case Supply:
      costValue = _unit_data[i].get_supply();
      break;
    default:
      throw 0;
    }
    
    sum += ( variables[i].get_value() * costValue );
  }

  return std::max( 0., sum - _quantity );
}

