#include "factory_resource.hpp"

FactoryResource::FactoryResource( const vector<Variable>& variables,
                                  const vector< UnitData >& unit_data )
	: FactoryModel( variables ),
	  _unit_data( unit_data )
{ }
	
shared_ptr<Model> FactoryResource::make_model()
{
	constraints.clear();

	constraints.emplace_back( make_shared<Stock>( variables, 20000, ResourceType::Mineral, _unit_data ) );
	constraints.emplace_back( make_shared<Stock>( variables, 14000, ResourceType::Gas, _unit_data ) );
	constraints.emplace_back( make_shared<Stock>( variables, 380, ResourceType::Supply, _unit_data ) );

	objective = make_shared<MaxGroundDPS>( variables, _unit_data );

  return make_shared<Model>( variables, constraints, objective );  
}
