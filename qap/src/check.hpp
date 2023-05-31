#pragma once

#include <vector>
#include <iostream>
#include <string>

bool check_solution( const std::vector<int>& solution );
double check_cost( const std::vector<int>& solution,
                   int number_variables,
                   const std::vector<std::vector<int>>& distances,
                   const std::vector<std::vector<int>>& flows	);
