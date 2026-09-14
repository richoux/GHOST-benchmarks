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
                                    int w,
                                    int h )
	: Minimize(variables),
	  buildings(vec_b),
	  starting_tile(st),
	  target_tile(tt),
	  width(w),
	  height(h)
{ }

// double WallinObjectives::expert_postprocess( const std::vector<ghost::Variable*>& variables, double best_cost ) const
// {
// 	// 1. Initialize a grid with value -1
// 	// 2. Fill the grid with building ids+1 (to start at 1).
// 	// 3. Perform A* from start to target, marking 0 all tiles of a building i. Use Chebyshev distance as heuristics.
// 	// 4. Scan the grid and note all building ids remaining on the grid (tiles with values > 0).
// 	// 5. Remove these buildings.	

// 	// 1. Initialize a grid with value -1
// 	std::vector< std::vector<int> > grid( height );
// 	for( int row = 0 ; row < height ; ++row )
// 		grid[row] = std::vector<int>( -1, width );

// 	// 2. Fill the grid with building ids+1 (to start at 1).
// 	for( int i = 0 ; i < variables.size() ; ++i )
// 		if( variables[i]->get_value() > -1 )
// 		{
// 			int start_r = index_to_row( variables[i]->get_value(), width );
// 			int start_c = index_to_column( variables[i]->get_value(), width );
// 			for( int row = start_r ; row < start_r + buildings[i].get_height() ; ++row )
// 				for( int col = start_c ; col < start_c + buildings[i].get_width() ; ++col )
// 					grid[row][col] = variables[i]->get_id() + 1;
// 		}

// 	// 3. Perform A* from start to target, marking 0 all tiles of a building i.
	
	
// 	// 4. Scan the grid and note all building ids remaining on the grid (tiles with values > 0).
	
	
// 	// 5. Remove these buildings.	
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
	: WallinObjectives( variables, buildings, starting_tile, target_tile, width, height ),
	  _grid(grid)
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

	// Consider the number of gap as main cost (coefficient 100), then the number of buildings, to have a wall as short as possible.
	return 100 * cost + std::count_if( variables.begin(), variables.end(), [](auto var){ return var->get_value() > -1; });
}

/****************/
/* MinBuildings */
/****************/
MinBuildings::MinBuildings( const std::vector<ghost::Variable>& variables,
                            const std::vector<Building>& buildings,
                            int starting_tile,
                            int target_tile,
                            int width,
                            int height )
	: WallinObjectives( variables, buildings, starting_tile, target_tile, width, height )
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
                          int width,
                          int height ) 
	: WallinObjectives( variables, buildings, starting_tile, target_tile, width, height )
{ }

double MinTechTree::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	int max_tree_depth = 0;

	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() != -1 )
			max_tree_depth = std::max( max_tree_depth, buildings[i].get_tree_depth() );

	return static_cast<double>( max_tree_depth );
}
