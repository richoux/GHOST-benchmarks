#include <iostream>
#include <vector>
#include <string>

#include "builder_wallin.hpp"
#include "building.hpp"
#include "convert.hpp"

#include <ghost/solver.hpp>

using namespace std::literals::chrono_literals;

void print_cost( const std::vector<int>& sol, const std::vector<Building>& buildings )
{
	for( int i = 0 ; i < sol.size() ; ++i )
	{
		if( sol[i] > -1 )
		{
			int var_i_row = index_to_row( sol[i], 12 );
			int var_i_col = index_to_column( sol[i], 12 );
			for( int j = i+1 ; j < sol.size() ; ++j )
			{
				if( sol[j] > -1 )
				{
					int var_j_row = index_to_row( sol[j], 12 );
					int var_j_col = index_to_column( sol[j], 12 );
			
					// when j is above i
					if( var_i_row == var_j_row + buildings[j].get_height()
					    && var_j_col + buildings[j].get_width() >= var_i_col && var_j_col <= var_i_col + buildings[i].get_width() )
					{
						if( buildings[i].get_gap_top() + buildings[j].get_gap_bottom() >= 16 ) // 16 pixels is a gap large enough to let a zergling go through.
						{
							std::cout << "Building " << i << " (top " << buildings[i].get_gap_top() << ") and Building " << j << " (bottom " << buildings[j].get_gap_bottom() << ")\n";
						}
					}

					// when j is below i
					if( var_i_row + buildings[i].get_height() == var_j_row
					    && var_j_col + buildings[j].get_width() >= var_i_col && var_j_col <= var_i_col + buildings[i].get_width() )
					{
						if( buildings[i].get_gap_bottom() + buildings[j].get_gap_top() >= 16 )
						{
							std::cout << "Building " << i << " (bottom " << buildings[i].get_gap_bottom() << ") and Building " << j << " (top " << buildings[j].get_gap_top() << ")\n";
						}
					}

					// when j is on the left of i
					if( var_i_col == var_j_col + buildings[j].get_width()
					    && var_j_row + buildings[j].get_height() >= var_i_row && var_j_row <= var_i_row + buildings[i].get_height() )
					{
						if( buildings[i].get_gap_left() + buildings[j].get_gap_right() >= 16 )
						{
							std::cout << "Building " << i << " (left " << buildings[i].get_gap_left() << ") and Building " << j << " (right " << buildings[j].get_gap_right() << ")\n";
						}
					}

					// when j is on the right of i
					if( var_i_col + buildings[i].get_width() == var_j_col 
					    && var_j_row + buildings[j].get_height() >= var_i_row && var_j_row <= var_i_row + buildings[i].get_height() )
					{
						if( buildings[i].get_gap_right() + buildings[j].get_gap_left() >= 16 )
						{
							std::cout << "Building " << i << " (right " << buildings[i].get_gap_right() << ") and Building " << j << " (left " << buildings[j].get_gap_left() << ")\n";
						}
					}
				}
			}
		}
	}
}


std::string building_to_char( std::string name )
{
	if( name.compare("Terran_Academy") == 0 )
		return "A";
	if( name.compare("Terran_Armory") == 0 )
		return "R";
	if( name.compare("Terran_Barracks") == 0 )
		return "B";
	if( name.compare("Terran_Bunker") == 0 )
		return "U";
	if( name.compare("Terran_Engineering_Bay") == 0 )
		return "E";
	if( name.compare("Terran_Factory") == 0 )
		return "F";
	if( name.compare("Terran_Supply_Depot") == 0 )
		return "S";

	return " ";
}

int main(int argc, char **argv)
{
	std::vector< std::vector<bool> > grid; // 8 rows, 12 columns
	grid.push_back(std::vector<bool>{true,true,true,true,true,true,true,true,true,true,true,true});
	grid.push_back(std::vector<bool>{true,true,true,true,true,true,true,true,true,true,true,true});
	grid.push_back(std::vector<bool>{true,true,true,true,true,true,true,true,true,true,true,true});
	grid.push_back(std::vector<bool>{true,true,true,true,true,true,true,true,false,false,false,false});
	grid.push_back(std::vector<bool>{true,true,true,true,true,true,false,false,false,false,false,false});
	grid.push_back(std::vector<bool>{true,true,true,true,true,true,false,false,false,false,false,false});
	grid.push_back(std::vector<bool>{true,true,true,true,false,false,false,false,false,false,false,false});
	grid.push_back(std::vector<bool>{true,true,true,true,false,false,false,false,false,false,false,false});
	
	std::vector< Building > buildings;
	buildings.push_back( make_building( BuildingType::Academy ) );
	buildings.push_back( make_building( BuildingType::Academy ) );
	buildings.push_back( make_building( BuildingType::Armory ) );
	buildings.push_back( make_building( BuildingType::Armory ) );
	buildings.push_back( make_building( BuildingType::Barracks ) );
	buildings.push_back( make_building( BuildingType::Barracks ) );
	buildings.push_back( make_building( BuildingType::Bunker ) );
	buildings.push_back( make_building( BuildingType::Bunker ) );
	buildings.push_back( make_building( BuildingType::EngineeringBay ) );
	buildings.push_back( make_building( BuildingType::EngineeringBay ) );
	buildings.push_back( make_building( BuildingType::Factory ) );
	buildings.push_back( make_building( BuildingType::Factory ) );
	buildings.push_back( make_building( BuildingType::SupplyDepot ) );
	buildings.push_back( make_building( BuildingType::SupplyDepot ) );
	

	BuilderWallin builder( grid,
	                       coord_to_index(7, 3, 12),
	                       coord_to_index(2, 11, 12),
	                       buildings );
	ghost::Solver solver( builder );

	double cost;
	std::vector<int> solution;
	
	solver.fast_search( cost, solution, 150ms );

	std::cout << "Cost: " << cost << "\n";

	// Building types
	std::vector< std::vector<std::string> > grid_type;
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_type.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});

	for( int s = 0 ; s < solution.size() ; ++s )
		if( solution[s] > -1 )
		{
			int row = index_to_row(solution[s], 12);
			int col = index_to_column(solution[s], 12);

			for( int r = row ; r < row + buildings[s].get_height() ; ++r )
				for( int c = col ; c < col + buildings[s].get_width() ; ++c )
					grid_type[r][c] = building_to_char( buildings[s].get_name() );
		}

	grid_type[7][3] = std::string("s");
	grid_type[2][11] = std::string("t");

	std::cout << "---";
	for( int c = 0 ; c < 11 ; ++c )
		std::cout << "--";

	for( int r = 0 ; r < 8 ; ++r )
	{
		std::cout << "\n|";
		for( int c = 0 ; c < 12 ; ++c )
			std::cout << grid_type[r][c] << "|";
		std::cout << "\n";
		
	std::cout << "---";
	for( int c = 0 ; c < 11 ; ++c )
		std::cout << "--";
	}
	
	std::cout << "\n\n";

	// Building index
	std::vector< std::vector<std::string> > grid_index;
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_index.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});

	for( int s = 0 ; s < solution.size() ; ++s )
		if( solution[s] > -1 )
		{
			int row = index_to_row(solution[s], 12);
			int col = index_to_column(solution[s], 12);

			for( int r = row ; r < row + buildings[s].get_height() ; ++r )
				for( int c = col ; c < col + buildings[s].get_width() ; ++c )
					grid_index[r][c] = std::to_string(s);
		}

	grid_index[7][3] = std::string("s");
	grid_index[2][11] = std::string("t");

	std::cout << "---";
	for( int c = 0 ; c < 11 ; ++c )
		std::cout << "--";

	for( int r = 0 ; r < 8 ; ++r )
	{
		std::cout << "\n|";
		for( int c = 0 ; c < 12 ; ++c )
			std::cout << grid_index[r][c] << "|";
		std::cout << "\n";
		
	std::cout << "---";
	for( int c = 0 ; c < 11 ; ++c )
		std::cout << "--";
	}
	
	std::cout << "\n";

	print_cost(solution, buildings);
}
