#pragma once

#include <vector>

#include "races.hpp"
#include "actionType.hpp"

using namespace std;

struct ActionData
{
  int			seconds_required;
  int			cost_mineral;
  int			cost_gas;
  int			cost_supply;
  ActionType		actionType;
  vector<string>	dependencies;
  string		creator; 
  Race			race;
  string		name;

  ActionData();
  ActionData( int, int, int, int, ActionType, vector<string>, string, Race, string );
  ActionData( const ActionData& );
  ActionData& operator=( ActionData );

  inline string get_type_string() const	
  { 
    switch( actionType ) 
    {
    case building: return "Building";
    case unit: return "Unit";
    case upgrade: return "Upgrade";
    case research: return "Research";
    default: return "Unknown";
    }
  }
    
  inline string get_race_string() const	
  { 
    switch( race ) 
    {
    case Terran: return "Terran";
    case Protoss: return "Protoss";
    case Zerg: return "Zerg";
    default: return "Unknown";
    }
  }

  inline int decreaseSeconds() { return --seconds_required; }

  void swap( ActionData& );

  friend ostream& operator<<( ostream&, const ActionData& );
};
