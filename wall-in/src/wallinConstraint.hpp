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
	double required_error( const std::vector<Variable*>& variables ) const override;

public:
	Overlap( const vector< Building >*, const WallinDomain* );
};

  
/*************/
/* Buildable */
/*************/  
class Buildable : public ghost::Constraint
{
	double required_error( const std::vector<Variable*>& variables ) const override;

public:
	Buildable( const vector< Building >*, const WallinDomain* );
};

  
/**********/
/* NoHoles */
/**********/  
class NoHoles : public ghost::Constraint
{
	double required_error( const std::vector<Variable*>& variables ) const override;

public:
	NoHoles( const vector< Building >*, const WallinDomain* );

	double postprocess_simulateCost( Building&, const int, vector<double>& );
};

  
/***********************/
/* StartingTargetTiles */
/***********************/  
class StartingTargetTiles : public ghost::Constraint
{
	double required_error( const std::vector<Variable*>& variables ) const override;

public:
	StartingTargetTiles( const vector< Building >*, const WallinDomain* );
};
