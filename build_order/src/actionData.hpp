#pragma once

#include <vector>

#include "races.hpp"
#include "actionType.hpp"

struct ActionData
{
	int seconds_required; // remaining seconds before completion
	int cost_mineral;
	int cost_gas;
	int cost_supply;
	ActionType actionType;
	std::vector<string> dependencies;
	std::string creator; 
	Race race;
	std::string name;

	ActionData();
	ActionData( int, int, int, int, ActionType, std::vector<std::string>, std::string, Race, std::string );
	ActionData( const ActionData& );
	ActionData& operator=( ActionData );

	inline std::string get_type_string() const	
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
    
	inline std::string get_race_string() const	
	{ 
		switch( race ) 
		{
		case Terran: return "Terran";
		case Protoss: return "Protoss";
		case Zerg: return "Zerg";
		default: return "Unknown";
		}
	}

	inline int decrease_seconds() { return --seconds_required; }

	void swap( ActionData& );

	friend std::ostream& operator<<( std::ostream&, const ActionData& );
};
