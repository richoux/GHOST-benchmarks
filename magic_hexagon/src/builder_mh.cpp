#include <ghost/global_constraints/linear_equation_eq.hpp>

#include "builder_mh.hpp"
#if defined MINMH
#include "min_sum_corners.hpp"
#endif

BuilderMagicHexagon::BuilderMagicHexagon( int instance_size )
	: ghost::ModelBuilder( true ),
	  _instance_size( instance_size ),
	  _hexagon( _instance_size ),
	  _nb_vars( _hexagon._nvar )
{ }

void BuilderMagicHexagon::declare_variables()
{
  // Create variables, with domains starting from value _hexagon.min_val
	create_n_variables( _nb_vars, _hexagon._min_val, _nb_vars );
	
	for( int i = 0; i < _nb_vars; ++i )
		variables[i].set_value( i + _hexagon._min_val );
}

void BuilderMagicHexagon::declare_constraints()
{
	for( size_t i = 0; i < _hexagon._rows.size() ; ++i )
		constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationEq>( _hexagon._rows[i], 0 ) );
}

#if defined MINMH
void BuilderMagicHexagon::declare_objective()
{
	objective = std::make_shared<MinCorners>( variables, _instance_size );
}
#endif
