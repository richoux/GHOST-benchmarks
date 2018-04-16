#pragma once

#include <vector>
#include <cmath>

#include "ghost/variable.hpp"

using namespace std;

enum Size{ Small, Medium, Large };
enum DamageType{ Concussive, Normal, Explosive };
enum Race{ Terran, Protoss, Zerg, Unknown };

struct Coord
{
  int x, y;

  Coord( int x, int y) : x(x), y(y) { }
};

struct Range
{
  double min, max;
};

struct Splash
{
  double ray1, ray2, ray3;
};

struct UnitData
{
  UnitData();
  UnitData( string, double, int, Size, int, int, int, DamageType, Range, Splash, bool, bool = false );
  UnitData( const UnitData& );
  UnitData& operator=( UnitData );

  inline double takeHit( double point )	{ return hp -= point; }
  inline bool isDead()		const	{ return hp <= 0.; }
  inline bool canShoot()	const	{ return canShootIn == 0; }
  inline void justShot()		{ canShootIn = cooldown; }
  inline void oneStep()			{ --canShootIn; }

  string	name;
  double	hp;
  double	initialHP;
  int		armor;
  Size		size;
  int		canShootIn;
  int		cooldown;
  int		damage;
  DamageType	damageType;
  Range		range;
  Splash	splashRadius;
  bool		doSplash;
  bool		doLinearSplash;

  friend ostream& operator<<( ostream&, const UnitData& );

private:
  void swap( UnitData& );
};


  
class Unit;
struct UnitEnemy
{
  UnitData data;
  Coord coord;

  UnitEnemy( UnitData data )
    : data(data)
  { }

  UnitEnemy( UnitData data, Coord coord )
    : data(data), coord(coord)
  { }

  bool isDead()				const;
  bool canShoot()			const;
  void justShot();
  void oneStep();
  double distanceFrom( const Unit& )	const;
  double distanceFrom(const UnitEnemy&)	const;
  bool isInRange( const Unit& )		const;
  bool isInRangeAndAlive( const Unit& )	const;
  double doDamageAgainst( int index, vector<Unit> &vecUnit, int num );
};

  
  
class Unit : public Variable
{
public:
  Unit( UnitData, Coord, int );
  Unit( UnitData, int, int, int );

  inline bool isSelected()		const	{ return value != -1; }
  inline double takeHit( double point )		{ return data.takeHit( point ); }
  inline bool isDead()			const	{ return data.isDead(); }
  inline bool canShoot()		const	{ return data.canShoot(); }
  inline void justShot()			{ data.justShot(); }
  inline void oneStep()				{ data.oneStep(); }

  inline double distanceFrom( const UnitEnemy &u ) const
  { return sqrt( pow( u.coord.x - coord.x, 2 ) + pow( u.coord.y - coord.y, 2 ) ); }

  inline double distanceFrom( const Unit &u ) const
  { return sqrt( pow( u.coord.x - coord.x, 2 ) + pow( u.coord.y - coord.y, 2 ) ); }

  inline bool isInRange( const UnitEnemy &u ) const
  { return distanceFrom(u) >= data.range.min && distanceFrom(u) <= data.range.max; }

  inline bool isInRangeAndAlive( const UnitEnemy &u ) const
  { return !u.isDead() && isInRange( u ); }

  inline UnitData getData()		const { return data; }
  inline void setData( UnitData u )	      { data = u; }

  inline Coord getCoord()		const { return coord; }
  inline int getX()			const { return coord.x; }
  inline int getY()			const { return coord.y; }

  inline void setCoord( Coord c )	      { coord = c; }
  inline void setX( int x )		      { coord.x = x; }
  inline void setY( int y )		      { coord.y = y; }
    
  inline double getHP()		const { return data.hp; }
  inline double getInitialHP()	const { return data.initialHP; }
  inline int canShootIn()		const { return data.canShootIn; }
  inline int getArmor()		const { return data.armor; }
  inline Size getSize()		const { return data.size; }
  inline string getSizeString()	const
  {
    switch( data.size ) 
    {
    case Small: return "small";
    case Medium: return "medium";
    case Large: return "large";
    default: return "unknown";
    }
  }

  inline int getCooldown()		const { return data.cooldown; }
  inline int getDamage()		const { return data.damage; }

  inline DamageType getDamageType()	const { return data.damageType; }
  inline string getDamageTypeString()	const	
  { 
    switch( data.damageType ) 
    {
    case Concussive: return "concussive";
    case Normal: return "normal";
    case Explosive: return "explosive";\
    default: return "unknown";
    }
  }
    
  inline Range getRange()		const { return data.range; }
  inline double getRangeMin()		const { return data.range.min; }
  inline double getRangeMax()		const { return data.range.max; }
  inline Splash getSplashRadius()	const { return data.splashRadius; }
  inline double getSplashRadiusMin()	const { return data.splashRadius.ray1; }
  inline double getSplashRadiusMed()	const { return data.splashRadius.ray2; }
  inline double getSplashRadiusMax()	const { return data.splashRadius.ray3; }

  inline bool isSplash()		const { return data.doSplash; }
  inline bool isLinearSplash()	const { return data.doLinearSplash; }
    
  double doDamage( vector<UnitEnemy> &vecEnemy );
  vector<double> computeDamage( vector<UnitEnemy> *vecEnemy ) const;
        
  friend ostream& operator<<( ostream&, const Unit& );

private:
  UnitData data;
  Coord	coord;
};

