#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void extract_data_from_file( const std::string& filename,
                             int& number_variables,
                             std::vector< std::vector<int> >& matrix_distances,
                             std::vector< std::vector<int> >& matrix_flows );

void extract_solution_from_file( const std::string& filename,
                                 int& cost,
                                 std::vector<int>& solution );
