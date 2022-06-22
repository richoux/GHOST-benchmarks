#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <map>

#include "../../src/constraint.hpp"
#include "../../src/objective.hpp"
#include "building.hpp"
#include "wallinDomain.hpp"

using namespace std;

class WallinConstraint : public Constraint<Building, WallinDomain>
{
public:
	WallinConstraint( const vector< Building >*, const WallinDomain* );

protected:
	virtual vector<double> v_simulateCost( Building &oldBuilding,
	                                       const vector<int> &newPosition,
	                                       vector< vector<double> > &vecVarSimCosts,
	                                       shared_ptr< Objective< Building, WallinDomain > > objective )
	{
		vector<double> simCosts( domain->getSize(), -1. );
		int backup = oldBuilding.getValue();
		int previousPos = 0;

		if( objective )
			objective->resetHelper();
      
		for( const auto &pos : newPosition )
		{
			if( pos >= 1 && pos == previousPos + 1 )
			{
				domain->quickShift( oldBuilding );
			}
			else
			{ 
				domain->clear( oldBuilding );
				oldBuilding.setValue( pos );
				domain->add( oldBuilding );
			}

			simCosts[pos+1] = cost( vecVarSimCosts[pos+1] );

			if( objective )
				objective->setHelper( oldBuilding, variables, domain );

			previousPos = pos;
		}

		domain->clear( oldBuilding );
		oldBuilding.setValue( backup );
		domain->add( oldBuilding );

		return simCosts;
	}

	bool isWall() const;
};  


/***********/
/* Overlap */
/***********/  
class Overlap : public WallinConstraint
{
public:
	Overlap( const vector< Building >*, const WallinDomain* );

private:
	double v_cost( vector<double>& ) const;
	vector<double> v_simulateCost( Building&,
	                               const vector<int>&,
	                               vector< vector<double> >&,
	                               shared_ptr< Objective< Building, WallinDomain > > );
};

  
/*************/
/* Buildable */
/*************/  
class Buildable : public WallinConstraint
{
public:
	Buildable( const vector< Building >*, const WallinDomain* );

private:
	double v_cost( vector<double>& ) const;
	vector<double> v_simulateCost( Building&,
	                               const vector<int>&,
	                               vector< vector<double> >&,
	                               shared_ptr< Objective< Building, WallinDomain > > );
};

  
/**********/
/* NoHoles */
/**********/  
class NoHoles : public WallinConstraint
{
public:
	NoHoles( const vector< Building >*, const WallinDomain* );

	double postprocess_simulateCost( Building&, const int, vector<double>& );
    
private:
	double v_cost( vector<double>& ) const;
};

  
/***********************/
/* StartingTargetTiles */
/***********************/  
class StartingTargetTiles : public WallinConstraint
{
public:
	StartingTargetTiles( const vector< Building >*, const WallinDomain* );

private:
	double v_cost( vector<double>& ) const;

	map<int, Building*> mapBuildings;
};
