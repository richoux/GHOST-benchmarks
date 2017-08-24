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
#include <map>
#include <memory>
#include <cmath>
#include <chrono>
#include <ctime>
#include <limits>
#include <algorithm>
#include <functional>
#include <cassert>
#include <typeinfo>

#include "variable.hpp"
#include "constraint.hpp"
#include "domain.hpp"
#include "misc/random.hpp"
#include "objective.hpp"

using namespace std;

namespace ghost
{
  //! Solver is the class coding the solver itself.
  /*! 
   * You just need to instanciate one Solver object.
   *
   * The Solver class is a template class, waiting for both the type
   * of variable, the type of domain and the type of constraint. Thus,
   * you must instanciate a solver by specifying the class of your
   * variable objects, the class of your domain object and the class
   * of your constraint objects, like for instance Solver<Variable,
   * Domain, Constraint> or Solver<MyCustomVariable, MyCustomDomain,
   * MyCustomConstraint>, if MyCustomVariable inherits from the
   * ghost::Variable class, MyCustomDomain inherits from the
   * ghost::Domain class and MyCustomConstraint inherits from the
   * ghost::Constraint class.
   *
   * Solver's constructor also need a shared pointer of an Objective
   * object (nullptr by default). The reason why Objective is not a
   * template parameter of Solver but a pointer is to allow a dynamic
   * modification of the objective function.
   *
   * \sa Variable, Domain, Constraint, Objective
   */
  class Solver
  {
    vector< shared_ptr< Variable > >	_vecVariables;	//!< Vector of (shared pointers of) variable of the CSP/COP.
    vector< shared_ptr< Constraint > >	_vecConstraints; //!< The vector of (shared pointers of) constraints of the CSP/COP.
    shared_ptr< Objective >		_objective;	//!< The shared pointer of the objective function.

    vector<int>	_weakTabuList;		//!< The weak tabu list, frozing used variables for tabuTime iterations. 
    Random	_randomVar;		//!< The random generator used by the solver.
    double	_bestSatCost;		//!< The satisfaction cost of the best solution.
    double	_bestOptCost;		//!< The optimization cost of the best solution.
    bool	_isOptimization;	//!< A boolean to know if it is a satisfaction or optimization run.
    bool	_permutationProblem;	//!< A boolean to know if it is a permutation problem or not.

    /////////////////////////
    // Bonne idée de mettre mutable ici ? On peut s'en débarasser ?
    mutable map< shared_ptr< Variable >, vector< shared_ptr< Constraint > > > _mapVarCtr;	//!< Map to know in which constraints are each variable.
    // map<Variable, vector< pair< shared_ptr< Constraint >, vector< Variable* >::iterator> >
    // _mapVarCtr;	//!< Map to know in which constraints are each variable.

    //! Decreasing values in tabuList
    //! \param freeVariables A boolean set to true if there is at least one free variable, ie, untabu.
    //! \sa _weakTabuList
    void decay_weak_tabu_list( bool& freeVariables );

    //! Compute and return the vector containing worst variables,
    //! ie, variables with the highest variable cost.
    //! \return A vector of worst variables
    vector< shared_ptr< Variable > > compute_worst_variables( bool freeVariables, const vector<double>& costVariables ) const;

    //! Compute the cost of each constraints
    //! \param costConstraints The vector to be filled by this function.
    //! \return The sum of constraints costs, ie, the global cost of the current configuration.
    double compute_constraints_costs( vector<double>& costConstraints ) const;

    //! Compute the variable cost of each variables
    //! \param costConstraints The vector containing the cost of each constraint.
    //! \param costVariables The vector of the cost of each variable, filled by this function.
    //! \param costNonTabuVariables The vector of the cost of non-tabu variables only, filled by this function.
    void compute_variables_costs( const vector<double>& costConstraints,
				  vector<double>& costVariables,
				  vector<double>& costNonTabuVariables ) const;

    // Compute incrementally the now global cost IF we change the value of 'variable' by 'value' with a local move.
    double simulate_local_move_cost( shared_ptr< Variable > variable,
				     double value,
				     vector<double>& costConstraints,
				     double currentSatCost ) const;

    // Compute incrementally the now global cost IF we swap values of 'variable' with another variable.
    double simulate_permutation_cost( shared_ptr< Variable > worstVariable,
				      shared_ptr< Variable > otherVariable,
				      vector<double>& costConstraints,
				      double currentSatCost ) const;

    //! Function to make a local move, ie, to assign a given
    //! value to a given variable
    void local_move( shared_ptr< Variable > variable,
		     vector<double>& costConstraints,
		     vector<double>& costVariables,
		     vector<double>& costNonTabuVariables,
		     double& currentSatCost );

    //! Function to make a permutation move, ie, to assign a given
    //! variable to a new position
    void permutation_move( shared_ptr< Variable > variable,
			   vector<double>& costConstraints,
			   vector<double>& costVariables,
			   vector<double>& costNonTabuVariables,
			   double& currentSatCost );


  public:
    //! Solver's regular constructor
    /*!
     * \param vecVariables A pointer to the vector of variable objects of the CSP/COP.
     * \param vecConstraints A constant reference to the vector of shared pointers of Constraint
     * \param obj A reference to the shared pointer of an Objective object. Default value is nullptr.
     * \param permutationProblem A boolean indicating if we work on a permutation problem. False by default.
     */
    Solver( const vector< shared_ptr< Variable > >& vecVariables, 
	    const vector< shared_ptr< Constraint > >& vecConstraints,
	    shared_ptr< Objective > obj,
	    bool permutationProblem = false );

    //! Second Solver's constructor
    /*!
     * The solver is calling Solver(vecVariables, vecConstraints, nullptr, permutationProblem)
     *
     * \param vecVariables A pointer to the vector of variable objects of the CSP/COP.
     * \param vecConstraints A constant reference to the vector of shared pointers of Constraint
     * \param permutationProblem A boolean indicating if we work on a permutation problem. False by default.
     */
    Solver( const vector< shared_ptr< Variable > >& vecVariables, 
	    const vector< shared_ptr< Constraint > >& vecConstraints,
	    bool permutationProblem = false );

    //! Solver's main function, to solve the given CSP/COP.
    /*!
     * \param finalCost The double of the sum of constraints cost for satisfaction problems, or the value of the objective function for optimization problems. For satisfaction problems, a cost of zero means a solution has been found.
     * \param finalSolution The configuration of the best solution found, ie, the vector of assignements of each variable.
     * \param sat_timeout The satisfaction timeout in milliseconds.
     * \param opt_timeout The optimization timeout in milliseconds (optionnal, equals to 10 times sat_timeout is not set).
     * \return True iff a solution has been found.
     */
    bool solve( double& finalCost, vector<int>& finalSolution, double sat_timeout, double opt_timeout = 0. );
  };
}
