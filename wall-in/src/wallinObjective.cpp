#include <vector>
#include <deque>
#include <algorithm>

#include "wallinObjective.hpp"
#include "building.hpp"
#include "convert.hpp"

WallinObjectives::WallinObjectives( const std::vector<ghost::Variable>& variables,
                                    const std::vector<Building>& vec_b,
                                    int st,
                                    int tt,
                                    int w )
	: Constraint(variables),
	  buildings(vec_b),
	  starting_tile(st),
	  target_tile(tt),
	  width(w)
{ }

double WallinObjectives::expert_postprocess( const std::vector<ghost::Variable*>& variables, double best_cost ) const
{
	// do not remove buildings over the starting or the target tiles.
	int starting_tile_r = index_to_row( starting_tile, width );
	int starting_tile_c = index_to_column( starting_tile, width );

	int target_tile_r = index_to_row( target_tile, width );
	int target_tile_c = index_to_column( target_tile, width );

	int building_over_starting_tile = 0;
	int building_over_target_tile = 0;
	
	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() > -1 )
		{
			int start_r = index_to_row( variables[i]->get_value(), width );
			int start_c = index_to_column( variables[i]->get_value(), width );
			int end_r = start_r + _buildings[i].get_height() - 1;
			int end_c = start_c + _buildings[i].get_width() - 1;
			
			if( start_r <= starting_tile_r && starting_tile_r <= end_r
			    && start_c <= starting_tile_c && starting_tile_c <= end_c	)
				building_over_starting_tile = i;
			
			if( start_r <= target_tile_r && target_tile_r <= end_r
			    && start_c <= target_tile_c && target_tile_c <= end_c	)
				building_over_target_tile = i;
		}

	// computing connected components
	// TODO
	std::deque<int> queue_all;
	std::deque<int> queue_cc;
	std::vector<int> line(width*height);
	
		int connected_components = 0;
	
	std::fill( line.begin(), line.end(), -1 );
	queue_all.clear();
	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() > -1 )
		{
			int start_r = index_to_row( variables[i]->get_value(), _width );
			int start_c = index_to_column( variables[i]->get_value(), _width );
			
			for( int r = start_r ; r < start_r + _buildings[i].get_height() && r < _height ; ++r ) // don't check outside the width x height frame, that is not the purpose of this constraint
				for( int c = start_c ; c < start_c + _buildings[i].get_width() && c < _width ; ++c )
				{
					line[coord_to_index(r,c,_width)] = 0;
					queue_all.push_front( coord_to_index(r,c,_width) );
				}
		}

	queue_cc.clear();
	int current_tile;
	
	while( !queue_all.empty() )
	{
		do
		{
			current_tile = queue_all.back();
			queue_all.pop_back();
		}
		while( line[ current_tile ] != 0 && !queue_all.empty() );

		if( queue_all.empty() )
			break;
		
		queue_cc.push_front( current_tile );
		++connected_components;

		do
		{
			current_tile = queue_cc.back();
			queue_cc.pop_back();
			line[ current_tile ] = connected_components;
				
			int row = index_to_row( current_tile, _width );
			int column = index_to_column( current_tile, _width );

			if( row - 1 >= 0 )
			{
				if( column - 1 >= 0 && line[ coord_to_index( row-1, column-1, _width ) ] == 0 )
				{
					queue_cc.push_front( coord_to_index( row-1, column-1, _width ) );
					line[ coord_to_index( row-1, column-1, _width ) ] = -2; //in process
				}

				if( line[ coord_to_index( row-1, column, _width ) ] == 0 )
				{
					queue_cc.push_front( coord_to_index( row-1, column, _width ) );
					line[ coord_to_index( row-1, column, _width ) ] = -2; //in process
				}

				if( column + 1 < _width && line[ coord_to_index( row-1, column+1, _width ) ] == 0 )
				{
					queue_cc.push_front( coord_to_index( row-1, column+1, _width ) );
					line[ coord_to_index( row-1, column+1, _width ) ] = -2; //in process
				}
			}

			if( column - 1 >= 0 && line[ coord_to_index( row, column-1, _width ) ] == 0 )
			{
				queue_cc.push_front( coord_to_index( row, column-1, _width ) );
				line[ coord_to_index( row, column-1, _width ) ] = -2; //in process
			}
			
			if( column + 1 < _width && line[ coord_to_index( row, column+1, _width ) ] == 0 )
			{
				queue_cc.push_front( coord_to_index( row, column+1, _width ) );
				line[ coord_to_index( row, column+1, _width ) ] = -2; //in process
			}
			
			if( row + 1 < _height )
			{
				if( column - 1 >= 0 && line[ coord_to_index( row+1, column-1, _width ) ] == 0 )
				{
					queue_cc.push_front( coord_to_index( row+1, column-1, _width ) );
					line[ coord_to_index( row+1, column-1, _width ) ] = -2; //in process
				}

				if( line[ coord_to_index( row+1, column, _width ) ] == 0 )
				{
					queue_cc.push_front( coord_to_index( row+1, column, _width ) );
					line[ coord_to_index( row+1, column, _width ) ] = -2; //in process
				}

				if( column + 1 < _width && line[ coord_to_index( row+1, column+1, _width ) ] == 0 )
				{
					queue_cc.push_front( coord_to_index( row+1, column+1, _width ) );
					line[ coord_to_index( row+1, column+1, _width ) ] = -2; //in process
				}
			}
		}
		while( !queue_cc.empty() );
	}
	
	return std::max( 0., static_cast<double>( connected_components - 1 ) );


	
}


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
                              const std::vector<Building>& buildings,
                              int starting_tile,
                              int target_tile,
                              int width,
                              int height )
	: WallinObjectives( variables, buildings, starting_tile, target_tile, width ),
	  _grid(grid),
	  _height(height)
{ }

double MinNumberGaps::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double cost = 0.;
	
	for( int i = 0 ; i < variables.size() ; ++i )
	{
		if( variables[i]->get_value() > -1 )
		{
			int var_i_row = index_to_row( variables[i]->get_value(), width );
			int var_i_col = index_to_column( variables[i]->get_value(), width );
			for( int j = i+1 ; j < variables.size() ; ++j )
			{
				if( variables[j]->get_value() > -1 )
				{
					int var_j_row = index_to_row( variables[j]->get_value(), width );
					int var_j_col = index_to_column( variables[j]->get_value(), width );
			
					// when j is above i
					if( var_i_row == var_j_row + buildings[j].get_height()
					    && var_j_col + buildings[j].get_width() >= var_i_col && var_j_col <= var_i_col + buildings[i].get_width() )
					{
						if( buildings[i].get_gap_top() + buildings[j].get_gap_bottom() >= 16 ) // 16 pixels is a gap large enough to let a zergling go through.
						{
							++cost;
						}
					}

					// when j is below i
					if( var_i_row + buildings[i].get_height() == var_j_row
					    && var_j_col + buildings[j].get_width() >= var_i_col && var_j_col <= var_i_col + buildings[i].get_width() )
					{
						if( buildings[i].get_gap_bottom() + buildings[j].get_gap_top() >= 16 )
						{
							++cost;
						}
					}

					// when j is on the left of i
					if( var_i_col == var_j_col + buildings[j].get_width()
					    && var_j_row + buildings[j].get_height() >= var_i_row && var_j_row <= var_i_row + buildings[i].get_height() )
					{
						if( buildings[i].get_gap_left() + buildings[j].get_gap_right() >= 16 )
						{
							++cost;
						}
					}

					// when j is on the right of i
					if( var_i_col + buildings[i].get_width() == var_j_col 
					    && var_j_row + buildings[j].get_height() >= var_i_row && var_j_row <= var_i_row + buildings[i].get_height() )
					{
						if( buildings[i].get_gap_right() + buildings[j].get_gap_left() >= 16 )
						{
							++cost;
						}
					}
				}
			}
		}
	}
	
	return cost;
}

/****************/
/* MinBuildings */
/****************/
MinBuildings::MinBuildings( const std::vector<ghost::Variable>& variables,
                            const std::vector<Building>& buildings,
                            int starting_tile,
                            int target_tile,
                            int width )
	: WallinObjectives( variables, buildings, starting_tile, target_tile, width )
{ }

double MinBuildings::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	return std::count_if( variables.begin(), variables.end(), [](auto var){ return var->get_value() > -1; });
}

/***************/
/* MinTechTree */
/***************/
MinTechTree::MinTechTree( const std::vector<ghost::Variable>& variables,
                          const std::vector<Building>& buildings,
                          int starting_tile,
                          int target_tile,
                          int width ) 
	: WallinObjectives( variables, buildings, starting_tile, target_tile, width )
{ }

double MinTechTree::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	int max_tree_depth = 0;

	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() != -1 )
			max_tree_depth = std::max( max_tree_depth, buildings[i].get_tree_depth() );

	return static_cast<double>( max_tree_depth );
}
