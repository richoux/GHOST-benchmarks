#include <typeinfo>

#include "var_unit.hpp"

double coeffDamageType( DamageType dt, Size s )
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

  
/****************/
/*** UnitData ***/
/****************/  
  
UnitData::UnitData() { }
  
UnitData::UnitData( string name,
		    double hp,
		    int armor,
		    Size size,
		    int canShootIn,
		    int cooldown,
		    int damage,
		    DamageType damageType,
		    Range range,
		    Splash splashRadius,
		    bool doSplash,
		    bool doLinearSplash )
  : name(name),
    hp(hp),
    initialHP(hp),
    armor(armor),
    size(size),
    canShootIn(canShootIn),
    cooldown(cooldown),
    damage(damage),
    damageType(damageType),
    range(range),
    splashRadius(splashRadius),
    doSplash(doSplash),
    doLinearSplash(doLinearSplash)
{ }

UnitData::UnitData( const UnitData &other )
  : name(other.name),
    hp(other.hp),
    initialHP(other.initialHP),
    armor(other.armor),
    size(other.size),
    canShootIn(other.canShootIn),
    cooldown(other.cooldown),
    damage(other.damage),
    damageType(other.damageType),
    range(other.range),
    splashRadius(other.splashRadius),
    doSplash(other.doSplash),
    doLinearSplash(other.doLinearSplash)
{ }

UnitData& UnitData::operator=( UnitData other )
{
  this->swap( other );
  return *this;
}

void UnitData::swap( UnitData &other )
{
  std::swap(this->name, other.name);
  std::swap(this->hp, other.hp);
  std::swap(this->initialHP, other.initialHP);
  std::swap(this->armor, other.armor);
  std::swap(this->size, other.size);
  std::swap(this->canShootIn, other.canShootIn);
  std::swap(this->cooldown, other.cooldown);
  std::swap(this->damage, other.damage);
  std::swap(this->damageType, other.damageType);
  std::swap(this->range, other.range);
  std::swap(this->splashRadius, other.splashRadius);
  std::swap(this->doSplash, other.doSplash);
  std::swap(this->doLinearSplash, other.doLinearSplash);
}

ostream& operator<<( ostream &os, const UnitData &u )
{
  os
    << "Name: " << u.name << endl
    << "Can shoot in: " << u.canShootIn << " seconds" << endl
    << "Cooldown: " <<  u.cooldown << endl
    << "HP: " <<  u.hp << endl
    << "Inital HP: " <<  u.hp << endl
    << "Damage: " <<  u.damage << endl
    << "Armor: " <<  u.armor << endl;
  // << "-------" << endl;
    
  return os;
}
  
  
/*****************/
/*** UnitEnemy ***/
/*****************/  

bool UnitEnemy::isDead()	const { return data.isDead(); }
bool UnitEnemy::canShoot()	const { return data.canShoot(); }
void UnitEnemy::justShot()	      { data.justShot(); }
void UnitEnemy::oneStep()	      { data.oneStep(); }
  
double UnitEnemy::distanceFrom( const Unit &u ) const
{
  return sqrt( pow( u.getX() - coord.x, 2 ) + pow( u.getY() - coord.y, 2 ) );
}

double UnitEnemy::distanceFrom( const UnitEnemy  &u ) const
{
  return sqrt( pow( u.coord.x - coord.x, 2 ) + pow( u.coord.y - coord.y, 2 ) );
}

bool UnitEnemy::isInRange( const Unit &u ) const
{
  return distanceFrom(u) >= data.range.min && distanceFrom(u) <= data.range.max;
}
  
bool UnitEnemy::isInRangeAndAlive( const Unit &u ) const
{
  return !u.isDead() && isInRange( u );
}

double UnitEnemy::doDamageAgainst( int index, vector<Unit> &vecUnit, int num )
{
  double damages = 0.;

  if( canShoot() )
  {
    double hit;
    if( !data.doSplash )
    {
      hit = ( data.damage - vecUnit[index].getArmor() ) * coeffDamageType( data.damageType, vecUnit[index].getSize() );
      hit = std::max( hit, 0.5 );
      damages += hit;
      vecUnit[index].takeHit( hit );
#ifdef DEBUG
      cout << data.name << "@" << num << " does " << std::max( hit, 0.5 ) << " damage to " << vecUnit[index].getFullName() << ":" << vecUnit[index].getId() << " (" << vecUnit[index].getHP() << " HP left)" << endl;
#endif
    }
    else
    {
      for( auto &v : vecUnit )
      {
	if( v.getId() == vecUnit[index].getId() )
	{
	  hit = ( data.damage - vecUnit[index].getArmor() ) * coeffDamageType( data.damageType, vecUnit[index].getSize() );
	  hit = std::max( hit, 0.5 );
	  damages += hit;
	  vecUnit[index].takeHit( hit );
#ifdef DEBUG
	  cout << data.name << "@" << num << " aims " << vecUnit[index].getFullName() << ":" << vecUnit[index].getId() << " (" << vecUnit[index].getHP() << " HP left) and does " << std::max( hit, 0.5 ) << " full splash damage" << endl;
#endif
	}
	else
	{
	  double dist = vecUnit[index].distanceFrom( v );
	  if( dist <= data.splashRadius.ray1 )
	  {
	    hit = ( data.damage - v.getArmor() ) * coeffDamageType( data.damageType, v.getSize() );
	    hit = std::max( hit, 0.5 );
	    damages += hit;
	    v.takeHit( hit );
#ifdef DEBUG
	    cout << data.name << "@" << num << " does " << std::max( hit, 0.5 ) << " full splash damage to " << v.getFullName() << ":" << v.getId() << " (" << v.getHP() << " HP left)" << endl;
#endif
	  }
	  else if( dist > data.splashRadius.ray1 && dist <= data.splashRadius.ray2 )
	  {
	    hit = ( ( data.damage * 0.5 ) - v.getArmor() ) * coeffDamageType( data.damageType, v.getSize() );
	    hit = std::max( hit, 0.5 );
	    damages += hit;
	    v.takeHit( hit );
#ifdef DEBUG
	    cout << data.name << "@" << num << " does " << std::max( hit, 0.5 ) << " half splash damage to " << v.getFullName() << ":" << v.getId() << " (" << v.getHP() << " HP left)" << endl;
#endif
	  }
	  else if( dist > data.splashRadius.ray2 && dist <= data.splashRadius.ray3 )
	  {
	    hit = ( ( data.damage * 0.25 ) - v.getArmor() ) * coeffDamageType( data.damageType, v.getSize() );
	    hit = std::max( hit, 0.5 );
	    damages += hit;
	    v.takeHit( hit );
#ifdef DEBUG
	    cout << data.name << "@" << num << " does " << std::max( hit, 0.5 ) << " quarter splash damage to " << v.getFullName() << ":" << v.getId() << " (" << v.getHP() << " HP left)" << endl;
#endif
	  }
	}
      }
    }
      
    justShot();
  }

  return damages;
}

  
/************/
/*** Unit ***/
/************/  

Unit::Unit( UnitData data,
	    Coord coord,
	    int	size )
  : Variable( data.name, data.name, size + 1, -1 ),
    data(data),
    coord(coord)
{ }

Unit::Unit(UnitData data,
	   int x,
	   int y,
	   int size )
  : Unit( data, { x, y }, size )
{ }

vector<double> Unit::computeDamage( vector<UnitEnemy> *vecUnit ) const
{
  vector<double> hits( vecUnit->size(), 0. );

  if( distanceFrom( vecUnit->at( value ) ) >= getRangeMin() && distanceFrom( vecUnit->at( value ) ) <= getRangeMax() )
  {
    UnitEnemy *u = &vecUnit->at( value );
    double hit;    

    if( !isSplash() )
    {
      hit = ( data.damage - u->data.armor ) * coeffDamageType( data.damageType, u->data.size );
      hits[value] = std::max( hit, 0.5 );
    }
    else
    {
      for( int i = 0 ; i < vecUnit->size() ; ++i )
      {
	if( i == value )
	{
	  hit = ( data.damage - u->data.armor ) * coeffDamageType( data.damageType, u->data.size );
	  hits[value] = std::max( hit, 0.5 );	    
	}
	else if( !vecUnit->at( i ).isDead() )
	{
	  double dist = u->distanceFrom( vecUnit->at( i ) );
	  if( dist <= data.splashRadius.ray1 )
	  {
	    hit = ( data.damage - vecUnit->at( i ).data.armor ) * coeffDamageType( data.damageType, vecUnit->at( i ).data.size );
	    hits[ i ] = std::max( hit, 0.5 );	    
	  }
	  else if( dist > data.splashRadius.ray1 && dist <= data.splashRadius.ray2 )
	  {
	    hit = ( ( data.damage * 0.5 ) - vecUnit->at( i ).data.armor ) * coeffDamageType( data.damageType, vecUnit->at( i ).data.size );
	    hits[ i ] = std::max( hit, 0.5 );	    
	  }
	  else if( dist > data.splashRadius.ray2 && dist <= data.splashRadius.ray3 )
	  {
	    hit = ( ( data.damage * 0.25 ) - vecUnit->at( i ).data.armor ) * coeffDamageType( data.damageType, vecUnit->at( i ).data.size );
	    hits[ i ] = std::max( hit, 0.5 );	    
	  }
	}
      }
    }
  }
 
  return hits;
}


double Unit::doDamage( vector<UnitEnemy> &vecUnit )
{
  double damages = 0.;

  if( canShoot() )
  {
    double hit;
    UnitEnemy *u = &vecUnit[ value ];
      
    if( !isSplash() )
    {
      hit = ( data.damage - u->data.armor ) * coeffDamageType( data.damageType, u->data.size );
      hit = std::max( hit, 0.5 );
      damages += hit;
      u->data.hp -= hit;
#ifdef DEBUG
      cout << getFullName() << ":" << getId() << " does " << std::max( hit, 0.5 ) << " damage to " << u->data.name << "@" << value << " (" << u->data.hp << " HP left)" << endl;
#endif
    }
    else
    {
      for( int i = 0 ; i < vecUnit.size() ; ++i )
      {
	if( i == value )
	{
	  hit = ( data.damage - u->data.armor ) * coeffDamageType( data.damageType, u->data.size );
	  hit = std::max( hit, 0.5 );
	  damages += hit;
	  u->data.hp -= hit;
#ifdef DEBUG
	  cout << getFullName() << ":" << getId() << " aims " << u->data.name << "@" << value << " (" << u->data.hp << " HP left) and does " << std::max( hit, 0.5 ) << " full splash damage" << endl;
#endif
	}
	else if( !vecUnit.at( i ).isDead() )
	{
	  double dist = u->distanceFrom( vecUnit[ i ] );
	  if( dist <= data.splashRadius.ray1 )
	  {
	    hit = ( data.damage - vecUnit[ i ].data.armor ) * coeffDamageType( data.damageType, vecUnit[ i ].data.size );
	    hit = std::max( hit, 0.5 );
	    damages += hit;
	    vecUnit[ i ].data.hp -= hit;
#ifdef DEBUG
	    cout << getFullName() << ":" << getId() << " does " << std::max( hit, 0.5 ) << " full splash damage to " << vecUnit[ i ].data.name << "@" << i << " (" << vecUnit[ i ].data.hp << " HP left)" << endl;
#endif
	  }
	  else if( dist > data.splashRadius.ray1 && dist <= data.splashRadius.ray2 )
	  {
	    hit = ( ( data.damage * 0.5 ) - vecUnit[ i ].data.armor ) * coeffDamageType( data.damageType, vecUnit[ i ].data.size );
	    hit = std::max( hit, 0.5 );
	    damages += hit;
	    vecUnit[ i ].data.hp -= hit;
#ifdef DEBUG
	    cout << getFullName() << ":" << getId() << " does " << std::max( hit, 0.5 ) << " half splash damage to " << vecUnit[ i ].data.name << "@" << i << " (" << vecUnit[ i ].data.hp << " HP left)" << endl;
#endif
	  }
	  else if( dist > data.splashRadius.ray2 && dist <= data.splashRadius.ray3 )
	  {
	    hit = ( ( data.damage * 0.25 ) - vecUnit[ i ].data.armor ) * coeffDamageType( data.damageType, vecUnit[ i ].data.size );
	    hit = std::max( hit, 0.5 );
	    damages += hit;
	    vecUnit[ i ].data.hp -= hit;
#ifdef DEBUG
	    cout << getFullName() << ":" << getId() << " does " << std::max( hit, 0.5 ) << " quarter splash damage to " << vecUnit[ i ].data.name << "@" << i << " (" << vecUnit[ i ].data.hp << " HP left)" << endl;
#endif
	  }
	}
      }
    }
      
    justShot();
  }

  return damages;
}
  
ostream& operator<<( ostream &os, const Unit &u )
{
  os
    << "Type info: " <<  typeid(u).name() << endl
    << "Full name: " << u.fullName << endl
    << "Coord: (" << u.coord.x << ", " << u.coord.y << ")" << endl 
    << "Id num: " << u.id << endl
    << "Value: " <<  u.value << endl
    << u.data;
    
  return os;
}

