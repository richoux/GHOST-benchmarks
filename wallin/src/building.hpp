/*
 * GHOST (General meta-Heuristic Optimization Solving Tool) is a C++ library 
 * designed to help developers to model and implement optimization problem 
 * solving. 
 * GHOST is a meta-heuristic solver aiming to solve any kind of combinatorial 
 * and optimization real-time problems represented by a CSP/COP. 
 * GHOST has been first developped to help making AI for the RTS game
 * StarCraft: Brood war, but can be used for any kind of applications where 
 * solving combinatorial and optimization problems within some tenth of 
 * milliseconds is needed. It is a generalization of the Wall-in project.
 * Please visit https://github.com/richoux/GHOST for further information.
 * 
 * Copyright (C) 2014-2016 Florian Richoux
 *
 * This file is part of GHOST.
 * GHOST is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as published 
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * GHOST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with GHOST. If not, see http://www.gnu.org/licenses/.
 */


#pragma once

#include "../../src/variable.hpp"
#include "races.hpp"

using namespace std;

namespace ghost
{
  class Building : public Variable
  {
  public:
    Building();
    Building(int, int, int, int, int, int, Race, int, string, string, int = -1);
    Building(const Building&);
    Building& operator=(Building);

    
    inline int getLength()	const	{ return length; }
    inline int getHeight()	const	{ return height; }
    inline int getSurface()	const	{ return height * length; }

    inline int getGapTop()	const	{ return gapTop; }
    inline int getGapRight()	const	{ return gapRight; }
    inline int getGapBottom()	const	{ return gapBottom; }
    inline int getGapLeft()	const	{ return gapLeft; }

    inline bool	isSelected()	const	{ return value != -1; }

    inline Race getRace()	const	{ return race; }
    inline string getRaceString()	const	
    { 
      switch( race ) 
      {
      case Terran: return "Terran";
      case Protoss: return "Protoss";
      case Zerg: return "Zerg";
      default: return "Unknown";
      }
    }
    
    inline int getTreedepth()	const	{ return treedepth; }
    
    friend ostream& operator<<( ostream&, const Building& );

  private:
    void swap(Building&);

    
    int length;
    int height;

    int gapTop;
    int gapRight;
    int gapBottom;
    int gapLeft;

    Race race;
    int treedepth;
  };
}
