#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <string>
#include <limits>
#include <functional>

#include <ghost/misc/random.hpp>
#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>
#include <ghost/objective.hpp>
#include <ghost/solver.hpp>

#include "constraint_shootable.hpp"
#include "objectives_target.hpp"
#include "unitData.hpp"
#include "unitType.hpp"
#include "damageFunctions.hpp"
#include "factory_variable_target.hpp"

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

  int sat = 20000;
  int opt = 30000;
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

  // Define our army variables and data
  vector< Variable > variables;
  vector< UnitData > my_army;
  make_my_terran( variables, my_army );
  
  // Define enemies, mirror to our units
  vector< UnitData > enemies;
  make_enemy_terran( enemies );
 
  // Define constraints
  vector< reference_wrapper<Variable> > variables_ref( variables.begin(), variables.end() );
  vector< shared_ptr<Constraint> > constraints { make_shared<Shootable>( variables_ref, my_army, enemies ) };

  // Define objective
  shared_ptr<Objective> objective = make_shared<MaxDamage>( my_army, enemies );

  // Define solver
  Solver solver( variables, constraints, objective );

  Random random;
  vector<int> in_range;

  int num_units = my_army.size();
  int num_enemy = enemies.size();

  vector<int> aimed_units( num_units, -1 );

  int dead_units = 0;
  int dead_enemy = 0;

  double total_damages;
  double total_damages_enemy;

#ifndef NDEBUG
  int tour = 1;
#endif

  vector< UnitData > copy_enemies( enemies );

  double cost = 0.;
  vector<int> solution( variables.size(), 0 );

  do
  {
    // If no one can shoot, is alive or has enemy in range, go next step
    if( all_of( begin( my_army ), end( my_army ), [&](UnitData& u){return ( !u.can_shoot() || u.is_dead() || get_living_enemies_in_range( u, enemies ).empty() );} ) 
	&&
	all_of( begin( enemies ), end( enemies ), [&](UnitData& u){return ( !u.can_shoot() || u.is_dead() || get_living_enemies_in_range( u, my_army ).empty() );} ) )
    {
      for_each( begin( my_army ), end( my_army ), [](UnitData& u){ u.one_step(); } );
      for_each( begin( enemies ), end( enemies ), [](UnitData& u){ u.one_step(); } );
      continue;
    }

    solver.solve( cost, solution, sat, opt );

    total_damages = 0.;
    total_damages_enemy = 0.;

#ifndef NDEBUG
    cout << "Tour " << tour++ << "\n";

    // My units attack
    cout << ":::: My turn ::::" << "\n";
#endif

    for( int i = 0 ; i < num_enemy ; ++i )
      copy_enemies[ i ].hp = enemies[ i ].hp;
    
    for( int i = 0; i < num_units; ++i )
    {
      auto& var_data = my_army[ i ];
      if( var_data.is_alive() )
      {
	auto& var = variables[ i ];
	if( var_data.can_shoot() && var.get_value() != -1 )
	{
	  auto vec_damages = compute_damage( var_data, var.get_value(), copy_enemies ); //compute_my_shoot_damage( i, variables );

	  double hit = 0.;
	  for( auto& value: vec_damages )
	    hit += value;

	  total_damages += hit;
	  copy_enemies[ var.get_value() ].hp -= hit;

	  var_data.just_shot();
	}
	else
	{
#ifndef NDEBUG
	  cout << var_data.name << ":" << var.get_id()
	       << " HP=" << var_data.hp
	       << ", wait=" << var_data.can_shoot_in
	       << " (value="<< var.get_value() <<")" << "\n";
#endif
	  if( !var_data.can_shoot() )
	    var_data.one_step();
	}
      }
    }
    
#ifndef NDEBUG
    // cout << "\n" << "\n" << "Simulation" << "\n";
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
    // 	    cout << v.getFullName() << ":" << v.getId() << " can hit " << copyEnemies.at(j).data.name << "@" << j << " with " << hits.at( j ) << " (dist=" << v.distanceFrom(copyEnemies.at(j)) << ")" << "\n";
    // 	}
	
    // 	v.setValue( backup );
    //   }
    // }
    // cout << "\n" << "\n";
#endif
	


    // The enemy attacks
#ifndef NDEBUG
    cout << "@@@@ Enemy's turn @@@@" << "\n";
#endif
    std::fill( aimed_units.begin(), aimed_units.end(), -1 );

    // choosing a target for each enemy unit
    for( int i = 0; i < num_enemy; ++i )
      if( enemies[ i ].is_alive() && enemies[ i ].can_shoot() )
      {
	in_range = get_living_enemies_in_range( enemies[ i ], my_army );
	
	if( !in_range.empty() )
	  // RANDOM SHOT
	  // aimedUnits[ i ] = inRange[ random.getRandNum( inRange.size() ) ];
	  
	  // LOW-HP SHOT
	  // aimedUnits[ i ] = inRange[ getLowestHPUnit( inRange, vec, random ) ];

      	  // LOW-HP RATIO SHOT
	  aimed_units[ i ] = in_range[ get_lowest_HP_ratio_unit( in_range, my_army, random ) ];
}

    // print stuff AND decrement cooldown (yes, it's bad to do it within the same loop, but whatever) 
    for( int i = 0; i < num_enemy; ++i )
    {
      if( enemies[ i ].is_alive() )
      {
	auto& var_data = enemies[ i ];
	if( var_data.can_shoot() )
	{
	  if( aimed_units[ i ] != -1 )
	  {
	    auto vec_damages = compute_damage( var_data, aimed_units[ i ], my_army ); //compute_enemy_shoot_damage( i, aimed_units[ i ] );

	    double hit = 0.;
	    for( auto value: vec_damages )
	      hit += value;
	    
	    total_damages_enemy += hit;
	    my_army[ aimed_units[ i ] ].hp -= hit;

	    var_data.just_shot();
	  }
#ifndef NDEBUG
	  else
	    cout << var_data.name << "@" << i << " HP=" << var_data.hp << ", wait=" << var_data.can_shoot_in << "\n";	    
#endif
	}
	else
	{
#ifndef NDEBUG
	  cout << var_data.name << "@" << i << " HP=" << var_data.hp << ", wait=" << var_data.can_shoot_in << "\n";
#endif
	  // decrement cooldown
	  if( !var_data.can_shoot() )
	    var_data.one_step();
	}
      }
    }

    for( int i = 0; i < num_enemy; ++i )
      enemies[ i ].hp = copy_enemies[ i ].hp;
    
    dead_units = count_if( begin(my_army), end(my_army), [](UnitData &u){ return u.is_dead(); } );
    dead_enemy = count_if( begin(enemies), end(enemies), [](UnitData &u){ return u.is_dead(); } );
#ifndef NDEBUG
    cout << "XXXX Turns over XXXX" << "\n"
    	 << "Total damages from you: " << total_damages << "\n" 
    	 << "Total damages from the enemy: " << total_damages_enemy << "\n"
    	 << "Number of dead units: " << dead_units << "\n" 
    	 << "Number of dead enemies: " << dead_enemy << "\n";
#endif

  } while( dead_units < num_units && dead_enemy < num_enemy );

  double total_hp = 0.;
  
  if( dead_units < num_units )
  {
    for( const auto &v : my_army )
      if( v.is_alive() )
	total_hp += v.hp;
    
    cout << "Winner: You!" << "\n"
	 << "Diff: " << dead_enemy - dead_units << "\n"
	 << "HP: " << total_hp << "\n";
    
#ifndef NDEBUG
    for( int i = 0; i < num_units; ++i )
      cout << my_army[ i ].name << ":" << variables[ i ].get_id() << " " << my_army[ i ].hp << " HP left" << "\n";
#endif
  }
  else if( dead_enemy < num_enemy )
  {
    for( const auto &e : enemies )
      if( e.is_alive() )
	total_hp += e.hp;

    cout << "Winner: The enemy..." << "\n"
      	 << "Diff: " << dead_enemy - dead_units << "\n"
      	 << "HP: " << total_hp << "\n";
    
#ifndef NDEBUG
    for( int i = 0; i < num_enemy; ++i )
      cout << enemies[ i ].name << "@" << i << " " << enemies[ i ].hp << " HP left" << "\n";
#endif
  }
  else
  {
    cout << "Draw!" << "\n";
  }
}
