#include <vector>
#include <algorithm>
#include <cstdlib>

#include "targetSelectionObjective.hpp"

using namespace std;

/****************************/
/* TargetSelectionObjective */
/****************************/

TargetSelectionObjective::TargetSelectionObjective( const string &name ) : Objective<Unit, TargetSelectionDomain>( name ) { }

double TargetSelectionObjective::expert_postprocess_satisfaction( vector< Unit > *vecVariables,
								  TargetSelectionDomain *domain,
								  double &bestCost,		
								  vector< Unit > &bestSolution,
								  double sat_timeout ) const 

{
  double cost = v_cost( vecVariables, domain );
    
  if( cost < bestCost )
  {
    bestCost = cost;
    copy( begin(*vecVariables), end(*vecVariables), begin(bestSolution) );
  }
}

  
/*************/
/* MaxDamage */
/*************/
MaxDamage::MaxDamage() : TargetSelectionObjective( "MaxDamage" ) { }
  
double MaxDamage::required_cost( vector< Unit > *vecVariables, TargetSelectionDomain *domain ) const
{
  double damages = 0.;
  vector<double> hits;
  vector<UnitEnemy> *enemies = domain->getAllEnemies();

  for( const auto &v : *vecVariables )
    if( v.getValue() != -1 )
    {
      hits = v.computeDamage( enemies );
      for_each( begin( hits ), end( hits ), [&](double d){damages += d;} );
    }

  return 1. / damages;
}

/***********/
/* MaxKill */
/***********/
MaxKill::MaxKill() : TargetSelectionObjective( "MaxKill" ) { }
  
double MaxKill::required_cost( vector< Unit > *vecVariables, TargetSelectionDomain *domain ) const
{
  vector<double> hits;
  vector<UnitEnemy> *enemies = domain->getAllEnemies();

  vector<UnitEnemy> copyEnemies(*enemies);

  for( const auto &v : *vecVariables )
  {
    if( v.getValue() != -1 )
    {
      hits = v.computeDamage( enemies );
      for( int i = 0 ; i < copyEnemies.size() ; ++i )
	copyEnemies[i].data.hp -= hits[i];
    }
  }

  return 1. / count_if( begin(copyEnemies), end(copyEnemies), [](UnitEnemy &u){ return u.isDead(); } );
}
