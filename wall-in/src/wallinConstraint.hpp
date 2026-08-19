#pragma once

#include <vector>

#include <ghost/constraint>
#include <ghost/variable.hpp>
#include "building.hpp"

/***********/
/* Overlap */
/***********/  
class Overlap : public ghost::Constraint
{
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
	
public:
	Overlap( const std::vector<ghost::Variable*>& variables, std::vector<int> line );
};

  
/*************/
/* Buildable */
/*************/  
class Buildable : public ghost::Constraint
{
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
	
public:
	Buildable( const std::vector<ghost::Variable*>& variables, std::vector<int> line );
};

  
/**********/
/* NoHoles */
/**********/  
class NoHoles : public ghost::Constraint
{
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;

public:
	NoHoles( const std::vector<ghost::Variable*>& variables, std::vector<int> line );

	//double postprocess_simulateCost( Building&, const int, vector<double>& );
};

  
/***********************/
/* StartingTargetTiles */
/***********************/  
class StartingTargetTiles : public ghost::Constraint
{
	double required_error( const std::vector<ghost::Variable*>& variables ) const override;

public:
	StartingTargetTiles( const std::vector<ghost::Variable*>& variables, std::vector<int> line, int starting_tile, int target_tile );
};
