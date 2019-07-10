#include "unitData.hpp"

UnitData::UnitData( int m,
		    int g,
		    double s,
		    double dps )
  : _mineral	( m ),
    _gas	( g ),
    _supply	( s ),
    _dps	( dps )
{ }
