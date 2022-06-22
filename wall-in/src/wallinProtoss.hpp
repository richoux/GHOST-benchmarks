#pragma once

#include <vector>
#include <memory>

#include "../../src/constraint.hpp"
#include "building.hpp"
#include "wallinDomain.hpp"

using namespace std;

std::shared_ptr<Building> c;
std::shared_ptr<Building> f;
std::shared_ptr<Building> g1;
std::shared_ptr<Building> g2;
std::shared_ptr<Building> p1;
std::shared_ptr<Building> p2;
std::shared_ptr<Building> y1;
std::shared_ptr<Building> y2;
std::shared_ptr<Building> y3;
std::shared_ptr<Building> y4;
std::shared_ptr<Building> s;

shared_ptr<Constraint> overlap;
shared_ptr<Constraint> buildable;
shared_ptr<Constraint> noGaps;
shared_ptr<Constraint> specialTiles;
shared_ptr<Constraint> pylons;

std::vector<std::shared_ptr<Building> > makeProtossBuildings()
{
	c = std::make_shared<CyberneticCore>( );
	f = std::make_shared<Forge>( );
	g1 = std::make_shared<Gateway>( );
	g2 = std::make_shared<Gateway>( );
	p1 = std::make_shared<PhotonCannon>( );
	p2 = std::make_shared<PhotonCannon>( );
	y1 = std::make_shared<Pylon>( );
	y2 = std::make_shared<Pylon>( );
	y3 = std::make_shared<Pylon>( );
	y4 = std::make_shared<Pylon>( );
	s = std::make_shared<ShieldBattery>( );

	std::vector<std::shared_ptr<Building> > vec {c, f, g1, g2, p1, p2, y1, y2, y3, y4, s};
	return vec;
}

vector::set< Constraint* > makeProtossConstraints( const std::vector<std::shared_ptr<Building> >& vec, const WallinDomain& domain )
{
	overlap	 = make_shared<Overlap>( vec, domain );
	buildable	 = make_shared<Buildable>( vec, domain );
	noHoles	 = make_shared<NoHoles>( vec, domain );
	specialTiles = make_shared<StartingTargetTiles>( vec, domain );
	pylons	 = make_shared<Pylons>( vec, domain );
    
	vector< shared_ptr<Constraint> > vecConstraints {overlap, buildable, noHoles, specialTiles, pylons};
	return vecConstraints;
}
