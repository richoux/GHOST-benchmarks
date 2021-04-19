#pragma once

#include <vector>
#include <memory>

#include "../../src/objective.hpp"
#include "building.hpp"
#include "wallinDomain.hpp"

using namespace std;

/*******************/
/* WallinObjective */
/*******************/
class WallinObjective : public Objective<Building, WallinDomain>
{
public:
	WallinObjective( const string & );

protected:
	virtual void v_setHelper( const Building &b, const vector< Building > *vecVariables, const WallinDomain *domain );

	virtual double v_postprocessSatisfaction( vector< Building > *vecVariables,
	                                          WallinDomain *domain,
	                                          double &bestCost,
	                                          vector< Building > &bestSolution,
	                                          double sat_timeout) const;
    
	virtual double v_postprocessOptimization( vector< Building > *vecBuildings,
	                                          WallinDomain *domain,
	                                          double &bestCost,
	                                          double opt_timeout);

    
	static int sizeWall;
};
  
/**********/
/* GapObj */
/**********/
class GapObj : public WallinObjective
{
public:
	GapObj();

private:
	double v_cost( vector< Building > *vecVariables, WallinDomain *domain ) const;
	int v_heuristicVariable( const vector< int > &vecId, const vector< Building > *vecVariables, WallinDomain *domain );
	void v_setHelper( const Building &b, const vector< Building > *vecVariables, const WallinDomain *domain );
	int gapSize( const Building &b, const vector< Building > *vecVariables, const WallinDomain *domain ) const;
};

/***************/
/* BuildingObj */
/***************/
class BuildingObj : public WallinObjective
{
public:
	BuildingObj();

private:
	double v_cost( vector< Building > *vecVariables, WallinDomain *domain ) const;
	int v_heuristicVariable( const vector< int > &vecId, const vector< Building > *vecVariables, WallinDomain *domain );
	double v_postprocessOptimization( vector< Building > *vecVariables, WallinDomain *domain, double &bestCost, double opt_timeout );
};

/***************/
/* TreeTechObj */
/***************/
class TechTreeObj : public WallinObjective
{
public:
	TechTreeObj();

private:
	double v_cost( vector< Building > *vecVariables, WallinDomain *domain ) const;
	int v_heuristicVariable( const vector< int > &vecId, const vector< Building > *vecVariables, WallinDomain *domain );
};
