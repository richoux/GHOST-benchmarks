#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>

#include "targetSelectionConstraint.hpp"

using namespace std;

TargetSelectionConstraint::TargetSelectionConstraint( vector< Unit > *variables )
  : Constraint<Unit>(variables) { }


vector<UnitEnemy> TargetSelectionConstraint::getEnemiesInRange( const Unit &u )
{
  vector<UnitEnemy> inRange;
    
  for( const auto &e : *enemies )
    if( u.isInRange( e ) )
      inRange.push_back( e );
    
  return inRange;
}
  
vector<UnitEnemy> TargetSelectionConstraint::getLivingEnemiesInRange( const Unit &u )
{
  vector<UnitEnemy> inRange;

  for( const auto &e : *enemies )
    if( u.isInRangeAndAlive( e ) )
      inRange.push_back( e );
    
  return inRange;
}

double TargetSelectionConstraint::required_cost() const
{
  double conflicts = 0.;

  for( auto&u : *variables )
  {
    // A unit u is badly assigned to a target t iif:
    // 1. u can shoot
    // 2. u has at least one living reachable target in its range
    // 3. t is the dummy target (-1) or t in unreachable from u or t is dead
    if( u.canShoot()
	&& !domain->getLivingEnemiesInRange( u ).empty()
	&& ( u.get_value() == -1
	     ||
	     !u.isInRangeAndAlive( domain->getEnemyData( u.get_value() ) ) ) )
    {
      ++conflicts;
    }

    // Other situation: u has no living reachable targets but is not affected to -1
    if( domain->getLivingEnemiesInRange( u ).empty() && u.get_value() != -1 )
    {
      ++conflicts;
    }
  }
    
  return conflicts;
}

