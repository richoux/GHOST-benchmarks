#pragma once

#include <vector>
#include <memory>

#include <ghost/variable.hpp>
#include <ghost/objective.hpp>
#include "building.hpp"

class WallinObjectives : public ghost::Minimize
{
protected:
	std::vector<Building> buildings;
	int starting_tile;
	int target_tile;
	int width;

public:
	WallinObjectives( const std::vector<ghost::Variable>& variables,
	                  const std::vector<Building>& buildings,
	                  int starting_tile,
	                  int target_tile,
	                  int width );
	double expert_postprocess( const std::vector<ghost::Variable*>& variables, double best_cost ) const override;
};


/*****************/
/* MinNumberGaps */
/*****************/
class MinNumberGaps : public WallinObjectives
{
	std::vector<std::vector<bool>> _grid;
	int _height;

public:
	MinNumberGaps( const std::vector<ghost::Variable>& variables,
	               const std::vector<std::vector<bool>>& _grid,
	               const std::vector<Building>& buildings,
	               int starting_tile,
	               int target_tile,
	               int width,
	               int height );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};

/****************/
/* MinBuildings */
/****************/
class MinBuildings : public WallinObjectives
{
public:
	MinBuildings( const std::vector<ghost::Variable>& variables,
	              const std::vector<Building>& buildings,
	              int starting_tile,
	              int target_tile,
	              int width );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};

/***************/
/* MinTreeTech */
/***************/
class MinTechTree : public WallinObjectives
{
	std::vector<Building> _buildings;

public:
	MinTechTree( const std::vector<ghost::Variable>& variables,
	             const std::vector<Building>& buildings,
	             int starting_tile,
	             int target_tile,
	             int width );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
