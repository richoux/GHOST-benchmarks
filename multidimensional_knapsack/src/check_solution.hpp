#pragma once

#include <vector>
#include <iostream>

bool check_solution( const std::vector<int>& solution,
                     const std::vector<std::vector<int>>& coefficients,
                     const std::vector<int>& capacities,
                     const std::vector<int>& values,
                     int optimal);
