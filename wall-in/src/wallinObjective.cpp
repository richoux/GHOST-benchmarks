#include <vector>
#include <algorithm>

#include "wallinObjective.hpp"
#include "building.hpp"
#include "convert.hpp"

// double WallinObjective::v_postprocessSatisfaction( vector< Building > *vecVariables,
//                                                    WallinDomain *domain,
//                                                    double &bestCost,
//                                                    vector< Building > &bestSolution,
//                                                    double sat_timeout ) const 
// {
// 	chrono::time_point<chrono::high_resolution_clock> startPostprocess = chrono::high_resolution_clock::now(); 

// 	bool change;
// 	double cost;
// 	NoHoles nh( vecVariables, domain );

// 	// find all buildings accessible from the starting building and remove all others
// 	int nberCurrent = *( domain->buildingsAt( domain->getStartingTile() ).begin() );
// 	Building current = vecVariables->at( nberCurrent );
// 	set< Building > toVisit = domain->getBuildingsAround( current, vecVariables );
// 	set< Building > visited;
// 	set< Building > neighbors;
    
// 	visited.insert( current );
    
// 	while( !toVisit.empty() )
// 	{
// 		auto first = *( toVisit.begin() );
// 		current = first;
// 		toVisit.erase( first );
// 		neighbors = domain->getBuildingsAround( current, vecVariables );
      
// 		visited.insert( current );
      
// 		for( const auto &n : neighbors )
// 			if( visited.find( n ) == visited.end() )
// 				toVisit.insert( n );
// 	}
    
// 	// remove all unreachable buildings from the starting building out of the domain
// 	for( auto &b : *vecVariables )
// 		if( visited.find( b ) == visited.end() )
// 		{
// 			domain->clear( b );
// 			b.setValue( -1 );
// 		}

// 	vector<double> varSimCost( vecVariables->size() );

// 	// clean wall from unnecessary buildings.
// 	do
// 	{
// 		for( auto &b : *vecVariables )
// 			if( ! domain->isStartingOrTargetTile( b.getId() ) )
// 			{
// 				change = false;
// 				if( b.isSelected() )
// 				{
// 					cost = 0.;
// 					fill( varSimCost.begin(), varSimCost.end(), 0. );
	      
// 					cost = nh.postprocess_simulateCost( b, -1, varSimCost );
	      
// 					if( cost == 0. )
// 					{
// 						domain->clear( b );
// 						b.setValue( -1 );
// 						change = true;
// 					}	  
// 				}
// 			}
// 	} while( change );

// 	double objectiveCost = this->cost( vecVariables, domain );
// 	int currentSizeWall = std::count_if( vecVariables->begin(), vecVariables->end(), []( const Building &b ){ return b.isSelected(); });

// 	if( objectiveCost < bestCost || ( objectiveCost == bestCost && currentSizeWall < sizeWall ) )
// 	{
// 		sizeWall = currentSizeWall;
// 		bestCost = objectiveCost;
// 		for( int i = 0; i < vecVariables->size(); ++i )
// 			bestSolution[i] = vecVariables->at(i);
// 	}

// 	return (chrono::high_resolution_clock::now() - startPostprocess).count();
// }

// double WallinObjective::v_postprocessOptimization( vector< Building > *vecVariables,
//                                                    WallinDomain *domain,
//                                                    double &bestCost,
//                                                    double opt_timeout ) 
// {
// 	chrono::time_point<chrono::high_resolution_clock> startPostprocess = chrono::high_resolution_clock::now(); 
// 	chrono::duration<double,micro> postprocesstimer(0);

// 	vector<int> tabuList( vecVariables->size() );
// 	std::fill( tabuList.begin(), tabuList.end(), 0 );

// 	multimap<int, Building> buildingSameSize;
    
// 	for( const auto &v : *vecVariables )
// 		buildingSameSize.insert( make_pair( v.getSurface(), v ) );

// 	Building *oldVariable;
// 	vector<int> goodVar;
// 	Building *toSwap;
// 	bool mustSwap;
    
// 	bestCost = v_cost( vecVariables, domain );
// 	double currentCost = bestCost;

// 	int postprocessTimeLimit = std::max( 1, static_cast<int>( ceil( static_cast<double>(opt_timeout) / 100) ) );

// 	while( (postprocesstimer = chrono::high_resolution_clock::now() - startPostprocess).count() < postprocessTimeLimit && bestCost > 0 )
// 	{
// 		goodVar.clear();

// 		for( int i = 0; i < tabuList.size(); ++i )
// 		{
// 			if( tabuList[i] <= 1 )
// 				tabuList[i] = 0;
// 			else
// 				--tabuList[i];
// 		}

// 		for( int i = 0; i < vecVariables->size(); ++i )
// 		{
// 			if( tabuList[i] == 0 )
// 				goodVar.push_back( i );
// 		}

// 		if( goodVar.empty() )
// 			for( int i = 0; i < vecVariables->size(); ++i )
// 				goodVar.push_back( i );	

// 		int index = v_heuristicVariable( goodVar, vecVariables, domain );
// 		oldVariable = &vecVariables->at( index );
// 		auto surface = buildingSameSize.equal_range( oldVariable->getSurface() );
	
// 		for( auto &it = surface.first; it != surface.second && bestCost != 0; ++it )
// 		{
// 			mustSwap = false;
// 			if( it->second.getId() != oldVariable->getId() )
// 			{
// 				domain->swap( vecVariables->at(it->second.getId()), *oldVariable );
// 				currentCost = v_cost( vecVariables, domain );
// 				if( currentCost < bestCost )
// 				{
// 					bestCost = currentCost;
// 					toSwap = &( vecVariables->at( it->second.getId() ) );
// 					mustSwap = true;
// 				}

// 				domain->swap( vecVariables->at( it->second.getId() ), *oldVariable );
// 			}
	  
// 			if( mustSwap )
// 				domain->swap( *toSwap, *oldVariable );
// 		}

// 		tabuList[ index ] = 2;
// 	}

// 	return postprocesstimer.count();
// }

  
/*****************/
/* MinNumberGaps */
/*****************/
MinNumberGaps::MinNumberGaps( const std::vector<ghost::Variable>& variables,
                              const std::vector<std::vector<bool>>& grid,
                              int width,
                              int height,
                              const std::vector<Building>& buildings )
	: Minimize( variables ),
	  _grid(grid),
	  _width(width),
	  _height(height),
	  _buildings(buildings)
{ }

double MinNumberGaps::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double cost = 0.;

	for( int i = 0 ; i < variables.size() ; ++i )
	{
		int var_i_row = index_to_row( i, _width );
		int var_i_col = index_to_column( i, _width );
		for( int j = i+1 ; j < variables.size() ; ++j )
		{
			int var_j_row = index_to_row( j, _width );
			int var_j_col = index_to_column( j, _width );
			
			// when j is above i
			if( var_i_row == var_j_row + _buildings[j].get_height()
			    && var_j_col + _buildings[j].get_width() >= var_i_col && var_j_col <= var_i_col + _buildings[i].get_width() )
			{
				if( _buildings[i].get_gap_top() + _buildings[j].get_gap_bottom() >= 16 ) // 16 pixels is a gap large enough to let a zergling go through.
					++cost;
			}

			// when j is below i
			if( var_i_row + _buildings[i].get_height() == var_j_row
			    && var_j_col + _buildings[j].get_width() >= var_i_col && var_j_col <= var_i_col + _buildings[i].get_width() )
			{
				if( _buildings[i].get_gap_bottom() + _buildings[j].get_gap_top() >= 16 )
					++cost;
			}

			// when j is on the left of i
			if( var_i_col == var_j_col + _buildings[j].get_width()
			    && var_j_row + _buildings[j].get_height() >= var_i_row && var_j_row <= var_i_row + _buildings[i].get_height() )
			{
				if( _buildings[i].get_gap_left() + _buildings[j].get_gap_right() >= 16 )
					++cost;
			}

			// when j is on the right of i
			if( var_i_col + _buildings[i].get_width() == var_j_col 
			    && var_j_row + _buildings[j].get_height() >= var_i_row && var_j_row <= var_i_row + _buildings[i].get_height() )
			{
				if( _buildings[i].get_gap_right() + _buildings[j].get_gap_left() >= 16 )
					++cost;
			}
		}
	}

	return cost;
}

/****************/
/* MinBuildings */
/****************/
MinBuildings::MinBuildings( const std::vector<ghost::Variable>& variables )
	: Minimize( variables )
{ }

double MinBuildings::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	return static_cast<double>( std::count_if( variables.begin(), variables.end(), [](auto var){ return var->get_value() != 1; }) );
}

/***************/
/* MinTechTree */
/***************/
MinTechTree::MinTechTree( const std::vector<ghost::Variable>& variables,
                          const std::vector<Building>& buildings ) 
	: Minimize( variables ),
	  _buildings(buildings)
{ }

double MinTechTree::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	int max_tree_depth = 0;

	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() != -1 )
			max_tree_depth = std::max( max_tree_depth, _buildings[i].get_tree_depth() );

	return static_cast<double>( max_tree_depth );
}
