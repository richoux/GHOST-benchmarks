#pragma once

#include <vector>
#include <cmath>
#include <string>

#include "races.hpp"
#include "sizes.hpp"
#include "damageTypes.hpp"

using namespace std;

struct Coord
{
  int x, y;
};

struct Range
{
  double min, max;
};

struct Splash
{
  double ray1, ray2, ray3;
};

/****************/
/*** UnitData ***/
/****************/  
struct UnitData
{
  UnitData() = default;
  UnitData( string,
	    double,
	    int,
	    Size,
	    int,
	    int,
	    int,
	    DamageType,
	    Range,
	    Splash,
	    Coord,
	    bool,
	    bool = false );
  UnitData( const UnitData& );
  UnitData& operator=( UnitData );

  string	name;
  double	hp;
  double	initial_HP;
  int		armor;
  Size		size;
  int		can_shoot_in;
  int		cooldown;
  int		damage;
  DamageType	damage_type;
  Range		range;
  Splash	splash_radius;
  Coord		coord;
  bool		do_splash;
  bool		do_linear_splash;

  inline double take_hit( double point )		{ return hp -= point; }
  inline bool is_dead()			const	{ return hp <= 0.; }
  inline bool is_alive()			const	{ return hp > 0.; }
  inline bool can_shoot()			const	{ return can_shoot_in == 0; }
  inline void just_shot()				{ can_shoot_in = cooldown; }
  inline void one_step()				{ if( !can_shoot() ) --can_shoot_in; }

  inline double distance_from( const UnitData& u ) const
  { return sqrt( pow( u.coord.x - coord.x, 2 ) + pow( u.coord.y - coord.y, 2 ) ); }

  inline bool is_in_range( const UnitData& u ) const
  { return distance_from( u ) >= range.min && distance_from( u ) <= range.max; }

  inline bool is_in_range_and_alive( const UnitData& u ) const
  { return u.is_alive() && is_in_range( u ); }

  inline string get_size_string() const
  {
    switch( size ) 
    {
    case Small: return "small";
    case Medium: return "medium";
    case Large: return "large";
    default: return "unknown";
    }
  }

  inline string get_damage_type_string() const	
  { 
    switch( damage_type ) 
    {
    case Concussive: return "concussive";
    case Normal: return "normal";
    case Explosive: return "explosive";
    default: return "unknown";
    }
  }

  // friend ostream& operator<<( ostream&, const UnitData& );

private:
  void swap( UnitData& );
};
