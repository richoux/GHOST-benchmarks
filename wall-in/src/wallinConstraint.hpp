#pragma once

#include <vector>

#include <ghost/constraint>
#include <ghost/variable.hpp>
#include "building.hpp"

/**************/
/* NoOverlaps */
/**************/  
class NoOverlaps : public ghost::Constraint
{
	int _width;
	int _height;
	std::vector<Building> _buildings;
	// grid where buildings are placed. -1 for unbuildable, 0 for free, n and -(n+1) for n buildings on the given (unbuildable if negative) tile
	std::vector<int> _line;

public:
	NoOverlaps( const std::vector<ghost::Variable*>& variables,
	            int width,
	            int height,
	            const std::vector<Building>& buildings );
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
};

  
/*************/
/* Buildable */
/*************/  
class Buildable : public ghost::Constraint
{
	std::vector<std::vector<bool>> _grid;
	int _width;
	int _height;
	std::vector<Building> _buildings;
	
public:
	Buildable( const std::vector<ghost::Variable*>& variables,
	           const std::vector<std::vector<bool>>& _grid,
	           int width,
	           int height,
	           const std::vector<Building>& buildings );
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
};

  
/**********/
/* NoHoles */
/**********/  
class NoHoles : public ghost::Constraint
{
	int _width;
	int _height;
	std::vector<Building> _buildings;
	std::vector<int> _line;
	std::deque<int> _queue_all;
	std::deque<int> _queue_cc;

public:
	NoHoles( const std::vector<ghost::Variable*>& variables,
	         int width,
	         int height,
	         const std::vector<Building>& buildings );
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
};

  
/***********************/
/* StartingTargetTiles */
/***********************/  
class StartingTargetTiles : public ghost::Constraint
{
	int _starting_tile;
	int _target_tile;
	std::vector<Building> _buildings;

public:
	StartingTargetTiles( const std::vector<ghost::Variable*>& variables,
	                     int starting_tile,
	                     int target_tile,
	                     const std::vector<Building>& buildings );
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
};
