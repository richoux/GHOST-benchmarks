/*
 * GHOST (General meta-Heuristic Optimization Solving Tool) is a C++ library 
 * designed to help developers to model and implement optimization problem 
 * solving. It contains a meta-heuristic solver aiming to solve any kind of 
 * combinatorial and optimization real-time problems represented by a CSP/COP. 
 *
 * GHOST has been first developped to help making AI for the RTS game
 * StarCraft: Brood war, but can be used for any kind of applications where 
 * solving combinatorial and optimization problems within some tenth of 
 * milliseconds is needed. It is a generalization of the Wall-in project.
 * Please visit https://github.com/richoux/GHOST for further information.
 * 
 * Copyright (C) 2014-2017 Florian Richoux
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

#include <vector>
#include <iostream>
#include <iterator>

#include "misc/random.hpp"

namespace ghost
{
  //! Domain is the class encoding the domain of your CSP/COP.
  /*! 
   * Domain is the class implementing variables' domains, ie, the set of possible values a variable can take.
   * In GHOST, such values must be integers.
   * 
   * A domain contains:
   * 1. the container of current possible values of the variable it belongs to, 
   * 2. the container of initial values (if one wants to reset the domain, since values in the current domain may change),
   * 3. an integer representing values outside the domain scope
   * 4. finally, a pseudo-random number generator.
   */
  class Domain
  {
    std::vector< int >	_currentDomain;	//!< Vector of integers containing the current values of the domain.
    int			_outsideScope;	//!< Value representing all values outside the scope of the domain
    Random		_random;	//!< A random generator used by the function randomValue. 
        
  public:
    //! Domain constructor.
    /*!
     * Basic constructor taking the outside-the-scope value (-1 by default).
     */
    Domain( int outsideScope = -1 );
    
    //! First Domain constructor.
    /*!
     * Constructor taking a vector of integer values the outside-the-scope value (-1 by default), to 
     * initialize both the initial and current possible variable values. The outside-the-scope value
     * must not belong to this list, or an exception is raised (throw 0).
     */
    Domain( const std::vector< int >& domain, int outsideScope = -1 );

    //! Second Domain constructor.
    /*!
     * Constructor taking the domain size N and a starting value x, and creating a domain
     * with all values in [x, x + N]. The outside-the-scope value is set to x-1.
     */
    Domain( int size, int startValue );

    //! Inline function to know if the domain has been initialized.
    /*!
     * Used to know if the Domain object is just an empty shell or a properly 
     * initialized domain. In some cases, it can be convenient to instanciate 
     * a domain object first and to fill it up with values latter.
     * \return True if and only if the domain has been initialized (i.e., the current domain is not empty).
     */
    inline bool is_initialized() const
    {
      return !_currentDomain.empty();
    }

    //! Inline function returning a random value from the domain.
    inline int random_value() const
    {
      return _currentDomain[ _random.get_random_number( _currentDomain.size() ) ];
    }

    //! Inline function to get the size of the current domain.
    /*!
     * Get the number of values currently contained by the domain.
     * \return A size_t corresponding to the size of _currentDomain.
     */
    inline size_t get_size() const
    {
      return _currentDomain.size();
    }

    //! Inline function returning the outside-scope value.
    /*!
     * Returns the value _outsideScope.
     */ 
    inline int get_outside_scope() const
    {
      return _outsideScope;
    }

    //! Get the value at the given index
    /*!
     * \param index is the index of the desired value.
     * \return The value at the given index if this one is in the range of the domain, otherwise the outside-the-scope value.
     */    
    int get_value( int index ) const;

    //! Get the index of a given value.
    /*!
     * \return If the given value is in the domain, it returns its index, and -1 otherwise.
     */ 
    int index_of( int value ) const;

    //! friend override of operator<<
    /*!
     * Prints on the standard output the current domain size and content.
     */ 
    friend std::ostream& operator<<( std::ostream& os, const Domain& domain )
    {
      os << "Size: " <<  domain.get_size() << "\nCurrent domain: ";
      std::copy( std::begin( domain._currentDomain ), std::end( domain._currentDomain ), std::ostream_iterator<int>( os, " " ) );
      return os << "\n";
    }
  };
}
