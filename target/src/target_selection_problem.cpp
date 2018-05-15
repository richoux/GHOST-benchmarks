#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <string>
#include <limits>

#include <ghost/misc/random>
#include <ghost/variable>
#include <ghost/constraint>
#include <ghost/objective>
#include <ghost/solver>

#include "constraint_shootable.hpp"
#include "objectives_target.hpp"
#include "unitData.hpp"
#include "unitType.hpp"
#include "damageFunctions.hpp"
#include "factoryUnitData.hpp"

using namespace ghost;
using namespace std;

vector<int> get_living_enemies_in_range( const UnitData& u, const vector< UnitData >& vec )
{
  vector<int> in_range;

  for( int j = 0 ; j < vec.size() ; ++j )
    if( u.is_in_range_and_alive( vec[ j ] ) )
      in_range.push_back( j );
      
  return in_range;
}

// int get_lowest_HP_unit( const vector<int> &inRange, const vector<Unit> &vec, Random &random )
// {
//   double minHP = std::numeric_limits<double>::max();
//   vector<int> ties;
  
//   for( int i = 0 ; i < inRange.size() ; ++i )
//     if( vec[i].getHP() == minHP )
//       ties.push_back( i );
//     else if( vec[i].getHP() < minHP )
//     {
//       ties.clear();
//       ties.push_back( i );
//       minHP = vec[i].getHP();
//     }

//   return random.getRandNum( ties.size() );
// }

int get_lowest_HP_ratio_unit( const vector<int>& in_range, const vector< UnitData >& vec, Random& random )
{
  double minHP = std::numeric_limits<double>::max();
  double ratio;
  vector<int> ties;
  
  for( int i = 0 ; i < in_range.size() ; ++i )
    if( ( ratio = vec[i].hp / vec[i].initial_HP ) == minHP )
      ties.push_back( i );
    else if( ratio < minHP )
    {
      ties.clear();
      ties.push_back( i );
      minHP = ratio;
    }

  int unit_index = ties[ 0 ];
  
  if( ties.size() > 1 )
    unit_index = ties[ random.get_random_number( ties.size() ) ];

  return unit_index;
}


int main(int argc, char **argv)
{
  // input
  // vector< pair<string, int> > input;
  
  // ifstream inputFile;
  // ofstream outputFile;
  // char read[256];
  // string str;
  // int count = 0;

  // string action;
  // int time;

  int sat = 20;
  int opt = 30;
  if( argc > 1 )
    sat = stoi(argv[1]);
  if( argc > 2 )
    opt = stoi(argv[2]);

  // inputFile.open( argv[1], std::ifstream::in );
  // if( inputFile.is_open() && inputFile.peek() != std::ifstream::traits_type::eof() )
  // {
  //   while( inputFile >> read )
  //   {
  //     str = read;

  //     if( count % 3 == 0 )
  //     {
  // 	if( isdigit( str[0] ) )
  // 	{
  // 	  time = stoi(str);
  // 	  break;
  // 	}
  // 	else
  // 	  action = str;
  //     }
  //     else if( count % 3 == 1 )
  //     {
  // 	input.emplace_back( action, stoi(str) );
  //     }

  //     ++count;
  //   }
  // }
  
  // Define objective
  // nil

  // Define our army data
  vector< UnitData > my_army
  {
    factory_unitData( UnitType::Marine, {30, 30} ), // 0
      factory_unitData( UnitType::Marine, {50, 30} ), // 1
      factory_unitData( UnitType::Marine, {70, 30} ), // 2
      factory_unitData( UnitType::Marine, {90, 30} ), // 3
      factory_unitData( UnitType::Marine, {110, 30} ), // 4
      
      factory_unitData( UnitType::Goliath, {10, 65} ), // 5
      factory_unitData( UnitType::Vulture, {50, 65} ), // 6
      factory_unitData( UnitType::Vulture, {90, 65} ), // 7
      factory_unitData( UnitType::Goliath, {130, 65} ), // 8

      factory_unitData( UnitType::SiegeTankTankMode, {10, 102} ), // 9
      factory_unitData( UnitType::Ghost, {65, 102} ), // 10
      factory_unitData( UnitType::Ghost, {75, 102} ), // 11
      factory_unitData( UnitType::SiegeTankTankMode, {130, 102} ), // 12

      factory_unitData( UnitType::SiegeTankSiegeMode, {70, 139} ), // 13
  };

  // Define enemies, mirror to our units
  vector< UnitData > enemies;
  for( int i = 0; i < my_army.size(); ++i )
  {
    enemies.push_back( my_army[ i ] );
    enemies[ i ].coord.y *= -1;
  }

  
  
  // Define constraints
  vector< shared_ptr<TargetSelectionConstraint> > vecConstraints { make_shared<TargetSelectionConstraint>( &vec, &domain ) };

  // Define objective
  shared_ptr<TargetSelectionObjective> objective = make_shared<MaxDamage>();
  
  Solver<Unit, TargetSelectionDomain, TargetSelectionConstraint> solver( &vec, &domain, vecConstraints, objective );
  // Solver<Unit, TargetSelectionDomain, TargetSelectionConstraint> solver( &vec, &domain, vecConstraints );

  Random random;
  vector<int> inRange;

  int numUnits = vec.size();
  int numEnemy = enemies.size();

  vector<int> aimedUnits( numUnits, -1 );

  int deadUnits;
  int deadEnemy;

  double totalDamages;
  double totalDamagesEnemy;

  int tour = 1;

  vector< UnitEnemy > copyEnemies( enemies );

  do
  {
    if( none_of( begin(vec), end(vec), [&](Unit &u){return !u.isDead() && u.canShoot() && !domain.get_living_enemies_in_range( u ).empty(); } ) 
	&&
	none_of( begin(enemies), end(enemies), [&](UnitEnemy &u){return !u.isDead() && u.canShoot() && !get_living_enemies_in_range( u, vec ).empty(); } ) )
    {
      for_each( begin(vec), end(vec), [](Unit &u){u.oneStep();} );
      for_each( begin(enemies), end(enemies), [](UnitEnemy &u){u.oneStep();} );
      continue;
    }

    solver.solve( sat, opt );

    totalDamages = 0.;
    totalDamagesEnemy = 0.;

#ifndef NDEBUG
    cout << "Tour " << tour++ << endl;

    // My units attack
    cout << ":::: My turn ::::" << endl;
#endif

    for( int i = 0 ; i < enemies.size() ; ++i )
      copyEnemies[i].data.hp = enemies[i].data.hp;
    
    for( auto &v : vec )
      if( !v.isDead() )
	if( v.canShoot() && v.getValue() != -1 )
	  totalDamages += v.doDamage( copyEnemies );
	else
	{
#ifndef NDEBUG
	  cout << v.getFullName() << ":" << v.getId() << " HP=" << v.getHP() << ", wait=" << v.canShootIn() << " (value="<< v.getValue() <<")" << endl;
#endif
	  if( !v.canShoot() )
	    v.oneStep();
	}
    
#ifndef NDEBUG
    // cout << endl << endl << "Simulation" << endl;
    // for( auto &v : vec )
    // {
    //   if( !v.isDead() && v.canShootIn() == v.getCooldown() )
    //   {
    // 	int backup = v.getValue();
	
    // 	for( int j = 0 ; j < copyEnemies.size() ; ++j )
    // 	{
    // 	  v.setValue( j );
    // 	  auto hits = v.computeDamage( &copyEnemies );
    // 	  if( hits.at( j ) != 0. )
    // 	    cout << v.getFullName() << ":" << v.getId() << " can hit " << copyEnemies.at(j).data.name << "@" << j << " with " << hits.at( j ) << " (dist=" << v.distanceFrom(copyEnemies.at(j)) << ")" << endl;
    // 	}
	
    // 	v.setValue( backup );
    //   }
    // }
    // cout << endl << endl;
#endif
	


    // The enemy attacks
#ifndef NDEBUG
    cout << "@@@@ Enemy's turn @@@@" << endl;
#endif
    std::fill( aimedUnits.begin(), aimedUnits.end(), -1 );

    // choosing a target for each enemy unit
    for( int i = 0 ; i < enemies.size() ; ++i )
      if( !enemies[i].isDead() && enemies[i].canShoot() )
      {
	inRange = get_living_enemies_in_range( enemies[i], vec );
	
	if( !inRange.empty() )
	  // RANDOM SHOT
	  // aimedUnits[ i ] = inRange[ random.getRandNum( inRange.size() ) ];
	  
	  // LOW-HP SHOT
	  // aimedUnits[ i ] = inRange[ getLowestHPUnit( inRange, vec, random ) ];

      	  // LOW-HP RATIO SHOT
	  aimedUnits[ i ] = inRange[ getLowestHPRatioUnit( inRange, vec, random ) ];
}

    // print stuff AND decrement cooldown (yes, it's bad to do it within the same loop, but whatever) 
    for( int i = 0 ; i < enemies.size() ; ++i )
    {
      if( !enemies[i].isDead() )
      {
	if( enemies[i].canShoot() )
	  if( aimedUnits[ i ] != -1 )
	    totalDamagesEnemy += enemies[i].doDamageAgainst( aimedUnits[ i ], vec, i );
#ifndef NDEBUG
	  else
	    cout << enemies[i].data.name << "@" << i << " HP=" << enemies[i].data.hp << ", wait=" << enemies[i].data.canShootIn << endl;	    
#endif
	else
	{
#ifndef NDEBUG
	  cout << enemies[i].data.name << "@" << i << " HP=" << enemies[i].data.hp << ", wait=" << enemies[i].data.canShootIn << endl;
#endif
	  // decrement cooldown
	  if( !enemies[i].canShoot() )
	    enemies[i].oneStep();
	}
      }
    }

    for( int i = 0 ; i < enemies.size() ; ++i )
      enemies[i].data.hp = copyEnemies[i].data.hp;
    
    deadUnits = count_if( begin(vec), end(vec), [](Unit &u){ return u.isDead(); } );
    deadEnemy = count_if( begin(enemies), end(enemies), [](UnitEnemy &u){ return u.isDead(); } );
#ifndef NDEBUG
    cout << "XXXX Turns over XXXX" << endl
    	 << "Total damages from you: " << totalDamages << endl 
    	 << "Total damages from the enemy: " << totalDamagesEnemy << endl
    	 << "Number of dead units: " << deadUnits << endl 
    	 << "Number of dead enemies: " << deadEnemy << endl;
#endif

  } while( deadUnits < numUnits && deadEnemy < numEnemy );

  double total_hp = 0.;
  
  if( count_if( begin(enemies), end(enemies), [&](UnitEnemy &u){ return u.isDead(); } ) == numEnemy
      &&
      count_if( begin(vec), end(vec), [&](Unit &u){ return u.isDead(); } ) < numUnits)
  {
    for( const auto &v : vec )
      if( !v.isDead() )
	total_hp += v.getHP();
    
    cout << "Winner: You!" << endl
	 << "Diff: " << deadEnemy - deadUnits << endl
	 << "HP: " << total_hp << endl;
    
#ifndef NDEBUG
    for( const auto &v : vec )
      cout << v.getFullName() << ":" << v.getId() << " " << v.getHP() << " HP left" << endl;
#endif
  }
  else if( count_if( begin(enemies), end(enemies), [&](UnitEnemy &u){ return u.isDead(); } ) < numEnemy
	   &&
	   count_if( begin(vec), end(vec), [&](Unit &u){ return u.isDead(); } ) == numUnits)
  {
    for( const auto &e : enemies )
      if( !e.isDead() )
	total_hp += e.data.hp;

    cout << "Winner: The enemy..." << endl
      	 << "Diff: " << deadEnemy - deadUnits << endl
      	 << "HP: " << total_hp << endl;
    
#ifndef NDEBUG
    for( int i = 0 ; i < enemies.size() ; ++i )
      cout << enemies[i].data.name << "@" << i << " " << enemies[i].data.hp << " HP left" << endl;
#endif
  }
  else
  {
    cout << "Draw!" << endl;
  }
}
