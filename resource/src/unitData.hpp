#pragma once

#include <string>

using namespace std;

class UnitData
{
  int		_mineral;
  int		_gas;
  double	_supply;
  double	_dps;

public:
  UnitData() = default;
  
  UnitData( int m,
	    int g,
	    double s,
	    double dps );

  inline int	get_mineral()	const { return _mineral; }
  inline int	get_gas()	const { return _gas; }
  inline double get_supply()	const { return _supply; }
  inline double	get_dps()	const { return _dps; }
};
