#pragma once

#include <vector>
#include <memory>

#include <ghost/variable>
#include <ghost/objective>
#include "building.hpp"

/**********/
/* GapObj */
/**********/
class GapObj : public ghost::Objective
{
public:
	GapObj();

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
	
private:
	double v_cost( vector< Building > *vecVariables, WallinDomain *domain ) const;
	int v_heuristicVariable( const vector< int > &vecId, const vector< Building > *vecVariables, WallinDomain *domain );
	void v_setHelper( const Building &b, const vector< Building > *vecVariables, const WallinDomain *domain );
	int gapSize( const Building &b, const vector< Building > *vecVariables, const WallinDomain *domain ) const;
};

/***************/
/* BuildingObj */
/***************/
class BuildingObj : public ghost::Objective
{
public:
	BuildingObj();

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;

private:
	double v_cost( vector< Building > *vecVariables, WallinDomain *domain ) const;
	int v_heuristicVariable( const vector< int > &vecId, const vector< Building > *vecVariables, WallinDomain *domain );
	double v_postprocessOptimization( vector< Building > *vecVariables, WallinDomain *domain, double &bestCost, double opt_timeout );
};

/***************/
/* TreeTechObj */
/***************/
class TechTreeObj : public ghost::Objective
{
public:
	TechTreeObj();

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;

private:
	double v_cost( vector< Building > *vecVariables, WallinDomain *domain ) const;
	int v_heuristicVariable( const vector< int > &vecId, const vector< Building > *vecVariables, WallinDomain *domain );
};
