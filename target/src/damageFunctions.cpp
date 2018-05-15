#include "damageFunctions.hpp"

double coeff_damage_type( DamageType dt, Size s )
{
  if( dt == Normal )
  {
    return 1.;
  }
  else if( dt == Concussive )
  {
    if( s == Small ) return 1.;
    else if( s == Medium ) return 0.5;
    else return 0.25;
  }
  else
  {
    if( s == Small ) return 0.5;
    else if( s == Medium ) return 0.75;
    else return 1;
  }
}

vector<double> compute_damage( UnitData& shooter, int target_index, vector< UnitData >& units )
{
  vector<double> hits( units.size(), 0. );

  auto& target = units[ target_index ];
          
  if( target_index != -1 && shooter.can_shoot() )
  {
    if( shooter.distance_from( target ) >= shooter.range.min && shooter.distance_from( target ) <= shooter.range.max )
    {
      double hit;    
	
      if( !shooter.do_splash )
      {
	hit = ( shooter.damage - target.armor ) * coeff_damage_type( shooter.damage_type, target.size );
	hits[ target_index ] = std::max( hit, 0.5 );
      }
      else
      {
	for( int i = 0 ; i < units.size() ; ++i )
	{
	  if( i == target_index )
	  {
	    hit = ( shooter.damage - target.armor ) * coeff_damage_type( shooter.damage_type, target.size );
	    hits[ target_index ] = std::max( hit, 0.5 );
	  }
	  else if( units[ i ].isAlive() )
	  {
	    double dist = target.distance_from( units[ i ] );
	    if( dist <= shooter.splash_radius.ray1 )
	    {
	      hit = ( shooter.damage - units[ i ].armor ) * coeff_damage_type( shooter.damage_type, units[ i ].size );
	      hits[ i ] = std::max( hit, 0.5 );	    
	    }
	    else if( dist > shooter.splash_radius.ray1 && dist <= shooter.splash_radius.ray2 )
	    {
	      hit = ( ( shooter.damage * 0.5 ) - units[ i ].armor ) * coeff_damage_type( shooter.damage_type, units[ i ].size );
	      hits[ i ] = std::max( hit, 0.5 );	    
	    }
	    else if( dist > shooter.splash_radius.ray2 && dist <= shooter.splash_radius.ray3 )
	    {
	      hit = ( ( shooter.damage * 0.25 ) - units[ i ].armor ) * coeff_damage_type( shooter.damage_type, units[ i ].size );
	      hits[ i ] = std::max( hit, 0.5 );	    
	    }
	  }
	}
      }
    }
  }
 
  return hits;
}

vector<double> compute_my_shoot_damage( int shooter_index, const vector< ghost::Variable >& vec_variables )
{
  return compute_damage( _my_army[ shooter_index ], vec_variables[ shooter_index ], _enemy_army ); 
}

vector<double> compute_enemy_shoot_damage( int shooter_index, int target_index )
{
  return compute_damage( _enemy_army[ shooter_index ], target_index, _my_army ); 
}

