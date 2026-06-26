#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <string>
#include <limits>
#include <functional>

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
#include "builder_target.hpp"

std::vector<int> get_living_enemies_in_range( const UnitData& u, const std::vector< UnitData >& vec )
{
  std::vector<int> in_range;

  for( int i = 0 ; i < vec.size() ; ++i )
    if( u.is_in_range_and_alive( vec[ i ] ) )
      in_range.push_back( i );
      
  return in_range;
}

// int get_lowest_HP_unit( const std::vector<int> &inRange, const std::vector<Unit> &vec, randutils::mt19937_rng& rng )
// {
//   double minHP = std::numeric_limits<double>::max();
//   std::vector<int> ties;
  
//   for( int i = 0 ; i < inRange.size() ; ++i )
//     if( vec[i].getHP() == minHP )
//       ties.push_back( i );
//     else if( vec[i].getHP() < minHP )
//     {
//       ties.clear();
//       ties.push_back( i );
//       minHP = vec[i].getHP();
//     }

//   return rng.pick( ties );
// }

int get_lowest_HP_ratio_unit( const std::vector<int>& in_range, const std::vector< UnitData >& vec, randutils::mt19937_rng& rng )
{
  double minHP = std::numeric_limits<double>::max();
  double ratio;
  std::vector<int> ties;
  
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
	  unit_index = rng.pick( ties );

  return unit_index;
}

void print_setup( const std::vector<UnitData>& my_army, const std::vector<UnitData>& enemies, const std::vector<int>& solution )
{
  cout << "My units:\n";
  for( int i= 0; i < my_army.size(); ++i )
  {
    auto var_data = my_army[ i ];
    
    cout << var_data.name << ":" << i << "\n"
         << "Coordinates: (" << var_data.coord.x << ", " << var_data.coord.y << ")\n"
         << "Can shoot in: " << var_data.can_shoot_in << " frames" << "\n"
         << "Cooldown: " <<  var_data.cooldown << "\n"
         << "HP: " <<  var_data.hp << "\n"
         << "Inital HP: " <<  var_data.initial_HP << "\n"
         << "Size: " <<  var_data.get_size_string() << "\n"
         << "Damage: " <<  var_data.damage << "\n"
         << "Damage Type: " <<  var_data.get_damage_type_string() << "\n"
         << "Armor: " <<  var_data.armor << "\n"
         << "Target: " << solution[ i ] << "\n"
         << "-------" << "\n\n";
  }

  cout << "===============\nEnemies:\n";
  for( int i= 0; i < enemies.size(); ++i )
  {
    auto var_data = enemies[ i ];
    
    cout << var_data.name << ":" << i << "\n" 
         << "Coordinates: (" << var_data.coord.x << ", " << var_data.coord.y << ")\n"
         << "Can shoot in: " << var_data.can_shoot_in << " frames" << "\n"
         << "Cooldown: " <<  var_data.cooldown << "\n"
         << "HP: " <<  var_data.hp << "\n"
         << "Inital HP: " <<  var_data.initial_HP << "\n"
         << "Size: " <<  var_data.get_size_string() << "\n"
         << "Damage: " <<  var_data.damage << "\n"
         << "Damage Type: " <<  var_data.get_damage_type_string() << "\n"
         << "Armor: " <<  var_data.armor << "\n"
         << "-------" << "\n";
  }  
}

int main(int argc, char **argv)
{
	bool parallel = false;
	int cores = -1;

	if( argc > 3 )
	{
		std::cout << "Usage: " << argv[0] << " [parallel=0/1] [number_threads]\n";
		return EXIT_FAILURE;
	}
	else
	{
		if( argc >= 2 )
			parallel = ( std::stoi( argv[1] ) != 0 );
		if( argc == 3 && parallel )
			cores = std::stoi( argv[2] );
	}
		
  ghost::Options options;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
  
  // Define our army variables and data
  std::vector< UnitType > my_units_type;
  std::vector< UnitData > my_army;
  make_my_terran( my_units_type, my_army );
  
  // Define enemies, mirror to our units
  std::vector< UnitData > enemies;
  make_enemy_terran( enemies );

  std::vector<int> in_range;

  int num_units = static_cast<int>( my_army.size() );
  int num_enemy = static_cast<int>( enemies.size() );

  std::vector<int> aimed_units( num_units, -1 );

  int dead_units = 0;
  int dead_enemy = 0;

#if defined DEBUG
  double total_damages;
  double total_damages_enemy;
#endif
  
  double error;
  std::vector<int> solution;

#if defined DEBUG
  int tour = 1;
  // print_setup( my_army, enemies, solution );
#endif

  std::vector< UnitData > copy_enemies( enemies );

  bool draw = false;
	randutils::mt19937_rng rng;
  
  do
  {
	  BuilderTarget builder( my_units_type, my_army, enemies );
	  ghost::Solver solver( builder );

    // If no one can shoot, is alive or has enemy in range, go next step
	  if( all_of( begin( my_army ), end( my_army ), [&](UnitData& u){return ( !u.can_shoot() || u.is_dead() || get_living_enemies_in_range( u, enemies ).empty() );} ) 
	      &&
	      all_of( begin( enemies ), end( enemies ), [&](UnitData& u){return ( !u.can_shoot() || u.is_dead() || get_living_enemies_in_range( u, my_army ).empty() );} ) )
	  {
      for_each( begin( my_army ), end( my_army ), [](UnitData& u){ u.one_step(); } );
      for_each( begin( enemies ), end( enemies ), [](UnitData& u){ u.one_step(); } );
      continue;
    }
	  
	  if( all_of( begin( my_army ), end( my_army ), [&](UnitData& u){return ( u.is_dead() || ( u.can_shoot() && get_living_enemies_in_range( u, enemies ).empty() ) );} ) 
	      &&
	      all_of( begin( enemies ), end( enemies ), [&](UnitData& u){return ( u.is_dead() || ( u.can_shoot() && get_living_enemies_in_range( u, my_army ).empty() ) );} ) )
	  {
		  draw = true;
		  break;
	  }
	  
    solver.fast_search( error, solution, 100ms, options );

#if defined DEBUG
    total_damages = 0.;
    total_damages_enemy = 0.;

    cout << "Tour " << tour++ << "\n";

    // My units attack
    cout << ":::: My turn ::::" << "\n";

    // if( tour == 2 )
    //   print_setup( my_army, enemies, solution );
#endif
    
    for( int i = 0 ; i < num_enemy ; ++i )
	    copy_enemies[ i ].hp = enemies[ i ].hp;
    
    for( int i = 0; i < num_units; ++i )
    {
	    auto& var_data = my_army[ i ];

	    if( var_data.is_alive() )
	    {
#if defined DEBUG
		    int cooldown = var_data.can_shoot_in;
#endif
		    if( var_data.can_shoot() && solution[ i ] != -1 )
		    {
			    auto vec_damages = compute_damage( var_data, solution[ i ], copy_enemies );

#if defined DEBUG
			    double hit = 0.;
			    for( auto& value: vec_damages )
				    hit += value;

			    total_damages += hit;
#endif
			    for( int i = 0 ; i < num_enemy ; ++i )
				    copy_enemies[ i ].hp -= vec_damages[ i ];

			    var_data.just_shot();
		    }
		    else
			    if( !var_data.can_shoot() )
				    var_data.one_step();
#if defined DEBUG
		    string dead_or_alive = var_data.is_alive() ? "alive" : "DEAD";
		    cout << var_data.name << ":" << i
		         << " HP=" << var_data.hp
		         << ", status=" << dead_or_alive
		         << ", wait=" << cooldown
		         << " value="<< solution[ i ] << "(" << copy_enemies[ solution[ i ] ].hp << " HP left)\n";

		    // if( solution[ i ] == -1 && cooldown == 0 )
		    //   for( int j = 0; j < copy_enemies.size(); ++j )
		    //     if( var_data.is_in_range_and_alive( copy_enemies[ j ] ) ) 
		    //       cout << "==> " << var_data.name << ":" << i << " could shoot " << copy_enemies[ j ].name << "@" << j << "\n";
#endif
	    }
    }
    
#if defined DEBUG
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
#if defined DEBUG
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
			    // aimedUnits[ i ] = rng.pick( inRange );
	  
			    // LOW-HP SHOT
			    // aimedUnits[ i ] = inRange[ getLowestHPUnit( inRange, vec, rng ) ];

			    // LOW-HP RATIO SHOT
			    aimed_units[ i ] = in_range[ get_lowest_HP_ratio_unit( in_range, my_army, rng ) ];
	    }

    // print stuff AND decrement cooldown (yes, it's bad to do it within the same loop, but whatever) 
    for( int i = 0; i < num_enemy; ++i )
    {
	    auto& var_data = enemies[ i ];

	    if( var_data.is_alive() )
	    {
#if defined DEBUG
		    int cooldown = var_data.can_shoot_in;
#endif
		    if( var_data.can_shoot() && aimed_units[ i ] != -1 )
		    {
			    auto vec_damages = compute_damage( var_data, aimed_units[ i ], my_army );
	  
#if defined DEBUG
			    double hit = 0.;
			    for( auto value: vec_damages )
				    hit += value;
	  
			    total_damages_enemy += hit;
#endif
			    for( int j = 0 ; j < num_units ; ++j )
				    my_army[ j ].hp -= vec_damages[ j ];
	  
			    var_data.just_shot();
		    }
		    else
		    {
			    // decrement cooldown
			    if( !var_data.can_shoot() )
				    var_data.one_step();
		    }
#if defined DEBUG
		    string dead_or_alive = var_data.is_alive() ? "alive" : "DEAD";
		    cout << var_data.name << "@" << i
		         << " HP=" << var_data.hp
		         << ", status=" << dead_or_alive
		         << ", wait=" << cooldown
		         << ", target="<< aimed_units[ i ] << "(" << my_army[ aimed_units[ i ] ].hp << " HP left)\n";
#endif
	    }
    }

    for( int i = 0; i < num_enemy; ++i )
	    enemies[ i ].hp = copy_enemies[ i ].hp;
    
    dead_units = count_if( begin(my_army), end(my_army), [](UnitData &u){ return u.is_dead(); } );
    dead_enemy = count_if( begin(enemies), end(enemies), [](UnitData &u){ return u.is_dead(); } );
#if defined DEBUG
    // if( tour == 2 )
    //   print_setup( my_army, enemies, solution );

    cout << "XXXX Turns over XXXX" << "\n"
         << "Total damages from you: " << total_damages << "\n" 
         << "Total damages from the enemy: " << total_damages_enemy << "\n"
         << "Number of dead units: " << dead_units << "\n" 
         << "Number of dead enemies: " << dead_enemy << "\n";
#endif

  } while( dead_units < num_units && dead_enemy < num_enemy );

  double total_hp = 0.;

  if( draw )
  {
	  cout << "Draw!" << "\n";
  }
  else if( dead_units < num_units )
  {
	  for( const auto &v : my_army )
		  if( v.is_alive() )
			  total_hp += v.hp;
    
	  cout << "Winner: You!" << "\n"
	       << "Diff: " << dead_enemy - dead_units << "\n"
	       << "HP: " << total_hp << "\n";
    
#if defined DEBUG
	  for( int i = 0; i < num_units; ++i )
		  cout << my_army[ i ].name << ":" << i << " " << my_army[ i ].hp << " HP left" << "\n";
#endif
  }
  else // then necessary, we have dead_enemy < num_enemy
  {
	  for( const auto &e : enemies )
		  if( e.is_alive() )
			  total_hp += e.hp;

	  cout << "Winner: The enemy..." << "\n"
	       << "Diff: " << dead_enemy - dead_units << "\n"
	       << "HP: " << total_hp << "\n";
    
#if defined DEBUG
	  for( int i = 0; i < num_enemy; ++i )
		  cout << enemies[ i ].name << "@" << i << " " << enemies[ i ].hp << " HP left" << "\n";
#endif
  }
}
