#include <algorithm>
#include <vector>
#include <deque>

//#include <iostream>

#include "wallinConstraint.hpp"
#include "convert.hpp"

/**************/
/* NoOverlaps */
/**************/  
NoOverlaps::NoOverlaps( const std::vector<ghost::Variable>& variables,
                        int width,
                        int height,
                        const std::vector<Building>& buildings )
	: Constraint( variables ),
	  _width(width),
	  _height(height),
	  _buildings(buildings),
	  _line(std::vector<int>(_width*_height))
{}

double NoOverlaps::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	double error = 0.;

	std::fill( _line.begin(), _line.end(), true ); // true = free tile
	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() > -1 )
		{		
			//std::cout << "index=" << variables[i]->get_value()  << "\n";
			int start_r = index_to_row( variables[i]->get_value(), _width );
			int start_c = index_to_column( variables[i]->get_value(), _width );
			//std::cout << "start_r=" << start_r << "\n";
			//std::cout << "start_c=" << start_c << "\n";
			
			for( int r = start_r ; r < start_r + _buildings[i].get_height() && r < _height ; ++r ) // don't check outside the width x height frame, that is not the purpose of this constraint
				for( int c = start_c ; c < start_c + _buildings[i].get_width() && c < _width ; ++c )
				{
					//std::cout << "height=" << _height << ", width=" << _width << "\n";
					//std::cout << "coord_to_index(r,c,_width) => " << "coord_to_index("<<r<<","<<c<<","<<_width<<") = "<< coord_to_index(r,c,_width) << "\n";
					if( _line[coord_to_index(r,c,_width)] )
						_line[coord_to_index(r,c,_width)] = false;
					else
						++error;
				}
		}

	return error;
}

/*************/
/* Buildable */
/*************/  
Buildable::Buildable( const std::vector<ghost::Variable>& variables,
                      const std::vector<std::vector<bool>>& grid,
                      int width,
                      int height,
                      const std::vector<Building>& buildings )
	: Constraint( variables ),
	  _grid(grid),
	  _width(width),
	  _height(height),
	  _buildings(buildings)
{}

double Buildable::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	double error = 0.;

	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() > -1 )
		{
			int start_r = index_to_row( variables[i]->get_value(), _width );
			int start_c = index_to_column( variables[i]->get_value(), _width );
			
			for( int r = start_r ; r < start_r + _buildings[i].get_height() ; ++r )
				for( int c = start_c ; c < start_c + _buildings[i].get_width() ; ++c )
					if( r >= _height || c >= _width || !_grid[r][c] )
						++error;
		}

	return error;
}

/**********/
/* NoHoles */
/**********/  
NoHoles::NoHoles(const std::vector<ghost::Variable>& variables,
                 int width,
                 int height,
                 const std::vector<Building>& buildings )
	: Constraint( variables ),
	  _width(width),
	  _height(height),
	  _buildings(buildings),
	  _line(std::vector<int>(_width*_height))
{}

double NoHoles::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	int connected_components = 0;
	
	std::fill( _line.begin(), _line.end(), -1 );
	_queue_all.clear();
	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() > -1 )
		{
			int start_r = index_to_row( variables[i]->get_value(), _width );
			int start_c = index_to_column( variables[i]->get_value(), _width );
			
			for( int r = start_r ; r < start_r + _buildings[i].get_height() && r < _height ; ++r ) // don't check outside the width x height frame, that is not the purpose of this constraint
				for( int c = start_c ; c < start_c + _buildings[i].get_width() && c < _width ; ++c )
				{
					_line[coord_to_index(r,c,_width)] = 0;
					_queue_all.push_front( coord_to_index(r,c,_width) );
				}
		}

	_queue_cc.clear();
	int current_tile;
	
	while( !_queue_all.empty() )
	{
		do
		{
			current_tile = _queue_all.back();
			_queue_all.pop_back();
		}
		while( _line[ current_tile ] != 0 && !_queue_all.empty() );

		if( _queue_all.empty() )
			break;
		
		_queue_cc.push_front( current_tile );
		++connected_components;

		do
		{
			current_tile = _queue_cc.back();
			_queue_cc.pop_back();
			_line[ current_tile ] = connected_components;
				
			int row = index_to_row( current_tile, _width );
			int column = index_to_column( current_tile, _width );

			if( row - 1 >= 0 )
			{
				if( column - 1 >= 0 && _line[ coord_to_index( row-1, column-1, _width ) ] == 0 )
				{
					_queue_cc.push_front( coord_to_index( row-1, column-1, _width ) );
					_line[ coord_to_index( row-1, column-1, _width ) ] = -2; //in process
				}

				if( _line[ coord_to_index( row-1, column, _width ) ] == 0 )
				{
					_queue_cc.push_front( coord_to_index( row-1, column, _width ) );
					_line[ coord_to_index( row-1, column, _width ) ] = -2; //in process
				}

				if( column + 1 < _width && _line[ coord_to_index( row-1, column+1, _width ) ] == 0 )
				{
					_queue_cc.push_front( coord_to_index( row-1, column+1, _width ) );
					_line[ coord_to_index( row-1, column+1, _width ) ] = -2; //in process
				}
			}

			if( column - 1 >= 0 && _line[ coord_to_index( row, column-1, _width ) ] == 0 )
			{
				_queue_cc.push_front( coord_to_index( row, column-1, _width ) );
				_line[ coord_to_index( row, column-1, _width ) ] = -2; //in process
			}
			
			if( column + 1 < _width && _line[ coord_to_index( row, column+1, _width ) ] == 0 )
			{
				_queue_cc.push_front( coord_to_index( row, column+1, _width ) );
				_line[ coord_to_index( row, column+1, _width ) ] = -2; //in process
			}
			
			if( row + 1 < _height )
			{
				if( column - 1 >= 0 && _line[ coord_to_index( row+1, column-1, _width ) ] == 0 )
				{
					_queue_cc.push_front( coord_to_index( row+1, column-1, _width ) );
					_line[ coord_to_index( row+1, column-1, _width ) ] = -2; //in process
				}

				if( _line[ coord_to_index( row+1, column, _width ) ] == 0 )
				{
					_queue_cc.push_front( coord_to_index( row+1, column, _width ) );
					_line[ coord_to_index( row+1, column, _width ) ] = -2; //in process
				}

				if( column + 1 < _width && _line[ coord_to_index( row+1, column+1, _width ) ] == 0 )
				{
					_queue_cc.push_front( coord_to_index( row+1, column+1, _width ) );
					_line[ coord_to_index( row+1, column+1, _width ) ] = -2; //in process
				}
			}
		}
		while( !_queue_cc.empty() );
	}
	
	return static_cast<double>(connected_components - 1);
}

  
/***********************/
/* StartingTargetTiles */
/***********************/  
StartingTargetTiles::StartingTargetTiles(const std::vector<ghost::Variable>& variables,
                                         int width,
                                         int height,
                                         int starting_tile,
                                         int target_tile,
                                         const std::vector<Building>& buildings )
	: Constraint( variables ),
	  _width(width),
	  _height(height),
	  _starting_tile(starting_tile),
	  _target_tile(target_tile),
	  _buildings(buildings)
{}

double StartingTargetTiles::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	double error = 0.;
	bool starting_tile_covered = false;
	bool target_tile_covered = false;

	int starting_tile_r = index_to_row( _starting_tile, _width );
	int starting_tile_c = index_to_column( _starting_tile, _width );

	int target_tile_r = index_to_row( _target_tile, _width );
	int target_tile_c = index_to_column( _target_tile, _width );

	for( int i = 0 ; i < variables.size() ; ++i )
		if( variables[i]->get_value() > -1 )
		{
			int start_r = index_to_row( variables[i]->get_value(), _width );
			int start_c = index_to_column( variables[i]->get_value(), _width );
			int end_r = start_r + _buildings[i].get_height() - 1;
			int end_c = start_c + _buildings[i].get_width() - 1;
			
			if( !starting_tile_covered
			    && start_r <= starting_tile_r && starting_tile_r <= end_r
			    && start_c <= starting_tile_c && starting_tile_c <= end_c	)
				starting_tile_covered = true;
			
			if( !target_tile_covered
			    && start_r <= target_tile_r && target_tile_r <= end_r
			    && start_c <= target_tile_c && target_tile_c <= end_c	)
				target_tile_covered = true;
		}
	
	if( !starting_tile_covered )
		++error;
	
	if( !target_tile_covered )
		++error;

	return error;
}
