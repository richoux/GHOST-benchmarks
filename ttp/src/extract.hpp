#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void extract_data_from_file( const std::string& filename,
                             int& number_variables,
                             std::vector< std::vector<double> >& matrix_distances );

void extract_solution_from_file( const std::string& filename,
                                 int& cost,
                                 std::vector<int>& solution );
