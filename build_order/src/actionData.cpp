#include <typeinfo>

#include "actionData.hpp"

ActionData::ActionData(int second, 
                       int mineral, 
                       int gas, 
                       int supply,
                       ActionType actionType,
                       std::vector<std::string> dep, 
                       std::string creator, 
                       Race race,
                       std::string name)
	: seconds_required(second),
	  cost_mineral(mineral),
	  cost_gas(gas), 
	  cost_supply(supply),
	  actionType(actionType),
	  dependencies(dep),
	  creator(creator),
	  race(race),
	  name(name)
{ }

// ActionData::ActionData( const ActionData &other )
//   : seconds_required(other.seconds_required),
//     cost_mineral(other.cost_mineral),
//     cost_gas(other.cost_gas), 
//     cost_supply(other.cost_supply), 
//     actionType(other.actionType), 
//     dependencies(other.dependencies), 
//     creator(other.creator),
//     race(other.race),
//     name(other.name)
// { }

// ActionData& ActionData::operator=( ActionData other )
// {
//   this->swap( other );
//   return *this;
// }

// void ActionData::swap( ActionData &other )
// {
//   std::swap(this->seconds_required, other.seconds_required);
//   std::swap(this->cost_mineral, other.cost_mineral);
//   std::swap(this->cost_gas, other.cost_gas);
//   std::swap(this->cost_supply, other.cost_supply);
//   std::swap(this->actionType, other.actionType);
//   std::swap(this->dependencies, other.dependencies);
//   std::swap(this->creator, other.creator);
//   std::swap(this->race, other.race);
//   std::swap(this->name, other.name);
// }

// std::ostream& operator<<( std::ostream &os, const ActionData &a )
// {
// 	os << "Name: " << a.name << std::endl
// 	   << "Seconds required: " << a.seconds_required << std::endl
// 	   << "Cost Mineral: " <<  a.cost_mineral << std::endl
// 	   << "Cost Gas: " <<  a.cost_gas << std::endl
// 	   << "Cost Supply: " <<  a.cost_supply << std::endl
// 	   << "Built/Trained/Researched/Upgraded/Morphed from: " <<  a.creator << std::endl
// 	   << "Dependencies: ";
    
// 	for( const auto& d : a.dependencies )
// 		os << d << "  "; 
    
// 	os << std::endl << "-------" << std::endl;
    
// 	return os;
// }
