#include <iostream>
#include <vector>
#include <string>

#include "builder_wallin.hpp"
#include "building.hpp"
#include "convert.hpp"

#include <ghost/solver.hpp>

using namespace std::literals::chrono_literals;

std::string building_to_char( std::string name )
{
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

	std::vector< std::vector<std::string> > grid_string;
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});
	grid_string.push_back(std::vector<std::string>{" "," "," "," "," "," "," "," "," "," "," "," "});

	for( int s = 0 ; s < solution.size() ; ++s )
		if( solution[s] > -1 )
		{
			int row = index_to_row(solution[s], 12);
			int col = index_to_column(solution[s], 12);

			for( int r = row ; r < row + buildings[s].get_height() ; ++r )
				for( int c = col ; c < col + buildings[s].get_width() ; ++c )
					grid_string[r][c] = building_to_char( buildings[s].get_name() );
		}

	grid_string[7][3] = std::string("s");
	grid_string[2][11] = std::string("t");
		
	std::cout << "---";
	for( int c = 0 ; c < 11 ; ++c )
		std::cout << "--";

	for( int r = 0 ; r < 8 ; ++r )
	{
		std::cout << "\n|";
		for( int c = 0 ; c < 12 ; ++c )
			std::cout << grid_string[r][c] << "|";
		std::cout << "\n";
		
	std::cout << "---";
	for( int c = 0 ; c < 11 ; ++c )
		std::cout << "--";
	}
	
	std::cout << "\n";	
}
