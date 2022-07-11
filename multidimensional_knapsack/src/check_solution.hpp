#pragma once

#include <vector>
#include <iostream>

bool check_solution( const std::vector<int>& solution,
                     const std::vector<std::vector<double>>& coefficients,
                     const std::vector<int>& capacities,
                     const std::vector<double>& values,
                     int optimal);
