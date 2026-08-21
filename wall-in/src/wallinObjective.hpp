#pragma once

#include <vector>
#include <memory>

#include <ghost/variable.hpp>
#include <ghost/objective.hpp>
#include "building.hpp"

/*****************/
/* MinNumberGaps */
/*****************/
class MinNumberGaps : public ghost::Minimize
{
	std::vector<std::vector<bool>> _grid;
	int _width;
	int _height;
	std::vector<Building> _buildings;

public:
	MinNumberGaps( const std::vector<ghost::Variable>& variables,
	               const std::vector<std::vector<bool>>& _grid,
	               int width,
	               int height,
	               const std::vector<Building>& buildings );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};

/****************/
/* MinBuildings */
/****************/
class MinBuildings : public ghost::Minimize
{
public:
	MinBuildings( const std::vector<ghost::Variable>& variables );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};

/***************/
/* MinTreeTech */
/***************/
class MinTechTree : public ghost::Minimize
{
	std::vector<Building> _buildings;

public:
	MinTechTree( const std::vector<ghost::Variable>& variables,
	             const std::vector<Building>& buildings );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
