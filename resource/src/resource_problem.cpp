#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>
// #include <functional>

#include <ghost/solver.hpp>

#include "factory_variable_resource.hpp"
#include "factory_resource.hpp"
#include "print_resource.hpp"

using namespace ghost;
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

// void process_mem_usage(double& vm_usage, double& resident_set)
// {
//    using std::ios_base;
//    using std::ifstream;
//    using std::string;

//    vm_usage     = 0.0;
//    resident_set = 0.0;

//    // 'file' stat seems to give the most reliable results
//    //
//    ifstream stat_stream("/proc/self/stat",ios_base::in);

//    // dummy vars for leading entries in stat that we don't care about
//    //
//    string pid, comm, state, ppid, pgrp, session, tty_nr;
//    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
//    string utime, stime, cutime, cstime, priority, nice;
//    string O, itrealvalue, starttime;

//    // the two fields we want
//    //
//    unsigned long vsize;
//    long rss;

//    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
//                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
//                >> utime >> stime >> cutime >> cstime >> priority >> nice
//                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

//    stat_stream.close();

//    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
//    vm_usage     = vsize / 1024.0;
//    resident_set = rss * page_size_kb;
// }


int main(int argc, char **argv)
{

  // double vm, rss;
  // process_mem_usage(vm, rss);
  // cout << "Memory usage: " << rss << "/" << vm << "\n\n";

	int timeout = 100;
	bool parallel = false;
	int cores = -1;
  int runs = 100;
  string race;
	bool random = false;
  
  
	if( argc == 1 )
	{
		std::cout << "Usage: " << argv[0] << " protoss|terran|zerg [random=0/1; default is 0] [timeout=100ms] [#runs=100] [parallel=0/1; default is 0] [number_threads]\n";
		return EXIT_FAILURE;
	}
	else
	{
		race = (string)argv[1];

		if( argc >= 3 )
			random = ( std::stoi( argv[2] ) != 0 );
		if( argc >= 4 )
			timeout = std::stoi( argv[3] );
		if( argc >= 5 )
			runs = std::stoi( argv[4] );
		if( argc >= 6 )
			parallel = ( std::stoi( argv[5] ) != 0 );
		if( argc == 7 && parallel )
			cores = std::stoi( argv[6] );
	}

  std::shared_ptr<Print> printer = std::make_shared<PrintResource>();
  Options options;
  options.print = printer;

  if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );

	if( !random )
		options.custom_starting_point = true;

  /////////////
  // Protoss //
  /////////////
  
  if( race == "protoss" )
  {
    // Create variables
    vector< Variable > variables_p;
    vector< UnitData > unit_data_p;
    make_protoss( 380, variables_p, unit_data_p );    

    // // Define constraints 
    // Stock mineral_p( variables_p,
    //                  20000,
    //                  ResourceType::Mineral,
    //                  unit_data_p );

    // Stock gas_p( variables_p,
    //              14000,
    //              ResourceType::Gas,
    //              unit_data_p );
    
    // Stock supply_p( variables_p,
    //                 380,
    //                 ResourceType::Supply,
    //                 unit_data_p );
  
    // vector< variant<Stock> > constraints_p { mineral_p, gas_p, supply_p };

    // Define objective
    // shared_ptr<Objective> objective = make_shared<MaxGroundDPS>(  variables_p, unit_data_p );
    // MaxGroundDPS objective( variables_p, unit_data_p );

    FactoryResource factory_p( variables_p, unit_data_p );

    // Define solver
    //Solver<MaxGroundDPS,Stock> solver_p( variables_p, constraints_p, objective );
    Solver solver_p( factory_p );
    
    double cost_p = 0.;
    vector<int> solution_p( variables_p.size(), 0 );
  
    int count = 0;
    double total = 0.;

    for(int i = 0 ; i < runs ; ++i )
	    if( solver_p.solve( cost_p, solution_p, timeout * 1000, options ) )
      {
	      ++count;
	      total += cost_p;
      }

    // process_mem_usage(vm, rss);
  
    // cout << "*** Protoss ***\n"
    //      << "Success rate: " << count << "%\n"
    //      << "Mean score: " << total/count << "\n";
    //   //<< "Memory usage: " << rss << "/" << vm << "\n\n";

    // cout << "var-------\n";
    // for( auto& v : variables_p )
    //   cout << v.get_name() << ":" << v.get_value() << "\n";
    // cout << "sol-------\n";
    // for( auto& v : variables_p )
    //     cout << v.get_name() << ":" << solution_p[ v.get_id() ] << "\n";

    // cout << "\n\n";
  }
  
  ////////////
  // Terran //
  ////////////
  if( race == "terran" )
  {
    // Create variables
    vector< Variable > variables_t;
    vector< UnitData > unit_data_t;
    make_terran( 380, variables_t, unit_data_t );    

    // Define constraints 
    // Stock mineral_t( variables_t,
    //                  20000,
    //                  ResourceType::Mineral,
    //                  unit_data_t );
    
    // Stock gas_t( variables_t,
    //              14000,
    //              ResourceType::Gas,
    //              unit_data_t );
    
    // Stock supply_t( variables_t,
    //                 380,
    //                 ResourceType::Supply,
    //                 unit_data_t );
    
    // vector< variant<Stock> > constraints_t { mineral_t, gas_t, supply_t };

    // // Define objective
    // MaxGroundDPS objective( variables_t, unit_data_t );

    FactoryResource factory_t( variables_t, unit_data_t );

    // // Define solver
    // Solver<MaxGroundDPS,Stock> solver_t( variables_t, constraints_t, objective );
    Solver solver_t( factory_t );
    
    double cost_t = 0.;
    vector<int> solution_t( variables_t.size(), 0 );
    
    int count = 0;
    double total = 0.;
    
    for(int i = 0 ; i < runs ; ++i )
      if( solver_t.solve( cost_t, solution_t, timeout * 1000, options ) )
      {
	      ++count;
	      total += cost_t;
      }
    
    // process_mem_usage(vm, rss);
    
    // cout << "*** Terran ***\n"
    //      << "Success rate: " << count << "%\n"
    //      << "Mean score: " << total/count << "\n";
    //<< "Memory usage: " << rss << "/" << vm << "\n\n";
    
    // cout << "var-------\n";
    // for( auto& v : variables_t )
    //   cout << v.get_name() << ":" << v.get_value() << "\n";
    // cout << "sol-------\n";
    // for( auto& v : variables_t )
    //   cout << v.get_name() << ":" << solution_t[ v.get_id() ] << "\n";
    
    // cout << "\n\n";
  }
  
  //////////
  // Zerg //
  //////////

  if( race == "zerg" )
  {
    // Create variables
    vector< Variable > variables_z;
    vector< UnitData > unit_data_z;
    make_zerg( 380, variables_z, unit_data_z );    

    // // Define constraints 
    // Stock mineral_z( variables_z,
    //                  20000,
    //                  ResourceType::Mineral,
    //                  unit_data_z );

    // Stock gas_z( variables_z,
    //              14000,
    //              ResourceType::Gas,
    //              unit_data_z );
    
    // Stock supply_z( variables_z,
    //                 380,
    //                 ResourceType::Supply,
    //                 unit_data_z );
  
    // vector< variant<Stock> > constraints_z { mineral_z, gas_z, supply_z };

    // // Define objective
    // MaxGroundDPS objective( variables_z, unit_data_z );

    FactoryResource factory_z( variables_z, unit_data_z );

    // // Define solver
    // Solver<MaxGroundDPS,Stock> solver_z( variables_z, constraints_z, objective );
    Solver solver_z( factory_z );
    
    double cost_z = 0.;
    vector<int> solution_z( variables_z.size(), 0 );
  
    int count = 0;
    double total = 0.;
  
    for(int i = 0 ; i < runs ; ++i )
      if( solver_z.solve( cost_z, solution_z, timeout * 1000, options ) )
      {
	      ++count;
	      total += cost_z;
      }

    // process_mem_usage(vm, rss);

    // cout << "*** Zerg ***\n"
    //      << "Success rate: " << count << "%\n"
    //      << "Mean score: " << total/count << "\n";
    //<< "Memory usage: " << rss << "/" << vm << "\n\n";

    // cout << "var-------\n";
    // for( auto& v : variables_z )
    //   cout << v.get_name() << ":" << v.get_value() << "\n";
    // cout << "sol-------\n";
    // for( auto& v : variables_z )
    //   cout << v.get_name() << ":" << solution_z[ v.get_id() ] << "\n";

    // cout << "\n\n";
  }

  return EXIT_SUCCESS;
}
