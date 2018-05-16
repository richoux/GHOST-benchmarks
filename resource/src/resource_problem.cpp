#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <functional>

#include <ghost/solver.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"
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

  string race = (string)argv[1];
  int runs = 100;
  sscanf( argv[2], "%i", &runs );
  
  /////////////
  // Protoss //
  /////////////
  
  if( race == "protoss" )
  {
    // Create variables
    vector< Variable > variables_p;
    vector< UnitData > unit_data_p;
    make_protoss( 380, variables_p, unit_data_p );    

    vector< reference_wrapper<Variable> > variables_ref_p( variables_p.begin(), variables_p.end() );

    // Define constraints 
    shared_ptr<Constraint> mineral_p = make_shared<Stock>( variables_ref_p,
							   20000,
							   ResourceType::Mineral,
							   unit_data_p );

    shared_ptr<Constraint> gas_p = make_shared<Stock>( variables_ref_p,
						       14000,
						       ResourceType::Gas,
						       unit_data_p );
    
    shared_ptr<Constraint> supply_p = make_shared<Stock>( variables_ref_p,
							  380,
							  ResourceType::Supply,
							  unit_data_p );
  
    vector< shared_ptr<Constraint> > constraints_p { mineral_p, gas_p, supply_p };

    // Define objective
    shared_ptr<Objective> objective = make_shared<MaxGroundDPS>( unit_data_p );

    // Define solver
    Solver solver_p( variables_p,
		     constraints_p,
		     objective );

    double cost_p = 0.;
    vector<int> solution_p( variables_p.size(), 0 );
  
    int count = 0;
    double total = 0.;

    for(int i = 0 ; i < runs ; ++i )
      if( solver_p.solve( cost_p, solution_p, 200, 130000 ) )
	//if( solver_p.solve( cost_p, solution_p, 2, 130 ) )
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

    vector< reference_wrapper<Variable> > variables_ref_t( variables_t.begin(), variables_t.end() );

    // Define constraints 
    shared_ptr<Constraint> mineral_t = make_shared<Stock>( variables_ref_t,
							   20000,
							   ResourceType::Mineral,
							   unit_data_t );
    
    shared_ptr<Constraint> gas_t = make_shared<Stock>( variables_ref_t,
						       14000,
						       ResourceType::Gas,
						       unit_data_t );
    
    shared_ptr<Constraint> supply_t = make_shared<Stock>( variables_ref_t,
							  380,
							  ResourceType::Supply,
							  unit_data_t );
    
    vector< shared_ptr<Constraint> > constraints_t { mineral_t, gas_t, supply_t };

    // Define objective
    shared_ptr<Objective> objective = make_shared<MaxGroundDPS>( unit_data_t );

    // Define solver
    Solver solver_t( variables_t,
		     constraints_t,
		     objective );
    
    double cost_t = 0.;
    vector<int> solution_t( variables_t.size(), 0 );
    
    int count = 0;
    double total = 0.;
    
    for(int i = 0 ; i < runs ; ++i )
      if( solver_t.solve( cost_t, solution_t, 200, 1000000 ) )
      //if( solver_t.solve( cost_t, solution_t, 2, 130 ) )
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

    vector< reference_wrapper<Variable> > variables_ref_z( variables_z.begin(), variables_z.end() );

    // Define constraints 
    shared_ptr<Constraint> mineral_z = make_shared<Stock>( variables_ref_z,
							   20000,
							   ResourceType::Mineral,
							   unit_data_z );

    shared_ptr<Constraint> gas_z = make_shared<Stock>( variables_ref_z,
						       14000,
						       ResourceType::Gas,
						       unit_data_z );
    
    shared_ptr<Constraint> supply_z = make_shared<Stock>( variables_ref_z,
							  380,
							  ResourceType::Supply,
							  unit_data_z );
  
    vector< shared_ptr<Constraint> > constraints_z { mineral_z, gas_z, supply_z };

    // Define objective
    shared_ptr<Objective> objective = make_shared<MaxGroundDPS>( unit_data_z );

    // Define solver
    Solver solver_z( variables_z,
		     constraints_z,
		     objective );

    double cost_z = 0.;
    vector<int> solution_z( variables_z.size(), 0 );
  
    int count = 0;
    double total = 0.;
  
    for(int i = 0 ; i < runs ; ++i )
      if( solver_z.solve( cost_z, solution_z, 250, 300000 ) )
      //if( solver_z.solve( cost_z, solution_z, 2, 130 ) )
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
