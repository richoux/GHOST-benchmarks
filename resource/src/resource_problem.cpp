#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>

#include <ghost/solver.hpp>

#include "var_unit.hpp"
#include "ctr_stock.hpp"
#include "obj_maxgrounddps.hpp"
#include "factory_unit.hpp"

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

  // Define objective
  shared_ptr<MaxGroundDPS> objective = make_shared<MaxGroundDPS>();

  string race = (string)argv[1];
  int runs = 100;
  sscanf( argv[2], "%i", &runs );
  
  ////////////
  // Terran //
  ////////////
  if( race == "terran" )
  {
    // Create variables
    auto variables_t = make_terran( 380 );    
    
    // Define constraints 
    shared_ptr<Stock> mineral_t = make_shared<Stock>( &variables_t, 20000, ResourceType::Mineral );
    shared_ptr<Stock> gas_t = make_shared<Stock>( &variables_t, 14000, ResourceType::Gas );
    shared_ptr<Stock> supply_t = make_shared<Stock>( &variables_t, 380, ResourceType::Supply );
    
    vector< shared_ptr<Stock> > constraints_t { mineral_t, gas_t, supply_t };
    
    // Define solver
    Solver<Unit, Stock> solver_t( variables_t,
				  constraints_t,
				  objective );
    
    double cost_t = 0.;
    vector<int> solution_t( variables_t.size(), 0 );
    
    int count = 0;
    double total = 0.;
    
    for(int i = 0 ; i < runs ; ++i )
      //if( solver_t.solve( cost_t, solution_t, 20, 5000 ) )
      if( solver_t.solve( cost_t, solution_t, 2, 130 ) )
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
  
  /////////////
  // Protoss //
  /////////////
  
  if( race == "protoss" )
  {
    // Create variables
    auto variables_p = make_protoss( 380 );    

    // Define constraints 
    shared_ptr<Stock> mineral_p = make_shared<Stock>( &variables_p, 20000, ResourceType::Mineral );
    shared_ptr<Stock> gas_p = make_shared<Stock>( &variables_p, 14000, ResourceType::Gas );
    shared_ptr<Stock> supply_p = make_shared<Stock>( &variables_p, 380, ResourceType::Supply );
  
    vector< shared_ptr<Stock> > constraints_p { mineral_p, gas_p, supply_p };

    // Define solver
    Solver<Unit, Stock> solver_p( variables_p,
  				  constraints_p,
  				  objective );

    double cost_p = 0.;
    vector<int> solution_p( variables_p.size(), 0 );
  
    int count = 0;
    double total = 0.;
  
    for(int i = 0 ; i < runs ; ++i )
      if( solver_p.solve( cost_p, solution_p, 2, 130 ) )
      {
  	++count;
  	total += cost_p;
      }

    // process_mem_usage(vm, rss);
  
    // cout << "*** Protoss ***\n"
    //      << "Success rate: " << count << "%\n"
    //      << "Mean score: " << total/count << "\n";
    //<< "Memory usage: " << rss << "/" << vm << "\n\n";

    // cout << "var-------\n";
    // for( auto& v : variables_p )
    //   cout << v.get_name() << ":" << v.get_value() << "\n";
    // cout << "sol-------\n";
    // for( auto& v : variables_p )
    //     cout << v.get_name() << ":" << solution_p[ v.get_id() ] << "\n";

    // cout << "\n\n";
  }
  
  //////////
  // Zerg //
  //////////

  if( race == "zerg" )
  {
    // Create variables
    auto variables_z = make_zerg( 380 );    

    // Define constraints 
    shared_ptr<Stock> mineral_z = make_shared<Stock>( &variables_z, 20000, ResourceType::Mineral );
    shared_ptr<Stock> gas_z = make_shared<Stock>( &variables_z, 14000, ResourceType::Gas );
    shared_ptr<Stock> supply_z = make_shared<Stock>( &variables_z, 380, ResourceType::Supply );
  
    vector< shared_ptr<Stock> > constraints_z { mineral_z, gas_z, supply_z };

    // Define solver
    Solver<Unit, Stock> solver_z( variables_z,
  				  constraints_z,
  				  objective );

    double cost_z = 0.;
    vector<int> solution_z( variables_z.size(), 0 );
  
    int count = 0;
    double total = 0.;
  
    for(int i = 0 ; i < runs ; ++i )
      if( solver_z.solve( cost_z, solution_z, 2, 130 ) )
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
}
