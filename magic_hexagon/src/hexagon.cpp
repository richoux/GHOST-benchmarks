/*
 * Author: Samuel Buchet
 * Year: 2026
 */

#include "hexagon.hpp"

#include <iostream>
#include <cmath>

Hexagon::Hexagon(size_t N): _N(N) {

    _nvar = 3*_N*_N - 3*_N + 1;

    // std::cout << "n var: " << _nvar << std::endl;

    _range = (_nvar-1)/2;

    // _range=_range*2;

    _nvalues = _range*2+1;
    
    _min_val = -_range; 
    _max_val = _range;
    _offset = -_min_val;

    buildGrid();

}

void Hexagon::buildGrid() {

    std::vector<int> prev_row;

    for(size_t col_ind = 0; col_ind < _N; col_ind ++) {
        _grid.emplace_back(HComb(_grid.size()));
        if(col_ind > 0) {
            _grid.back().west = _grid[_grid.size()-2].index;
            _grid[col_ind-1].east = _grid.back().index;
        }
        prev_row.push_back(_grid.size()-1);
    }

    for(size_t row_ind = 1; row_ind < _N; row_ind ++) {
        std::vector<int> temp_row;
        for(size_t col_ind = 0; col_ind < _N+row_ind; col_ind ++) {
            _grid.emplace_back(HComb(_grid.size()));
            if(col_ind > 0) {
                _grid.back().west = _grid[_grid.size()-2].index;
                _grid[_grid.size()-2].east = _grid.back().index;
                _grid.back().nwest = _grid[prev_row[col_ind-1]].index;
                _grid[prev_row[col_ind-1]].seast = _grid.back().index; 
            }
            if(col_ind < _N+row_ind-1) {
                _grid.back().neast = _grid[prev_row[col_ind]].index;
                _grid[prev_row[col_ind]].swest = _grid.back().index; 
            }
            temp_row.push_back(_grid.back().index);
        }
        prev_row = temp_row;
    }

    for(size_t row_ind = 0; row_ind < _N-1; row_ind ++) {
        std::vector<int> temp_row;
        for(size_t col_ind = 0; col_ind < 2*_N-2-row_ind; col_ind ++) {
            _grid.emplace_back(HComb(_grid.size()));
            _grid.back().nwest = _grid[prev_row[col_ind]].index;
            _grid[prev_row[col_ind]].seast = _grid.back().index;
            if(col_ind > 0) {
                _grid.back().west = _grid[_grid.size()-2].index;
                _grid[_grid.size()-2].east = _grid.back().index;
            }
            _grid.back().neast = _grid[prev_row[col_ind+1]].index;
            _grid[prev_row[col_ind+1]].swest = _grid.back().index;
            temp_row.push_back(_grid.back().index);
        }
        prev_row = temp_row;
    }

    for(size_t cell_ind = 0; cell_ind < _grid.size(); cell_ind ++) {
        if(_grid[cell_ind].west < 0) {
            _horiz.push_back(std::vector<int>());
            int ind = cell_ind;
            while(ind >= 0) {
                _horiz.back().push_back(ind);
                ind = _grid[ind].east;
            }
        }
        if(_grid[cell_ind].nwest < 0) {
            _diag.push_back(std::vector<int>());
            int ind = cell_ind;
            while(ind >= 0) {
                _diag.back().push_back(ind);
                ind = _grid[ind].seast;
            }
        }
        if(_grid[cell_ind].neast < 0) {
            _revdiag.push_back(std::vector<int>());
            int ind = cell_ind;
            while(ind >= 0) {
                _revdiag.back().push_back(ind);
                ind = _grid[ind].swest;
            }
        }
    }

    for(auto& row: _horiz) {
        _rows.emplace_back(row);
    }
    for(auto& row: _diag) {
        _rows.emplace_back(row);
    }
    for(auto& row: _revdiag) {
        _rows.emplace_back(row);
    }

    return;

    std::cout << "horizontal rows:  " << std::endl;
    for(auto row: _horiz) {
        for(auto elt: row) {
            std::cout << elt << ", "; 
        }
        std::cout << std::endl;
    }
    
    std::cout << "diagonal rows:  " << std::endl;
    for(auto diag: _diag) {
        for(auto elt: diag) {
            std::cout << elt << ", "; 
        }
        std::cout << std::endl;
    }

    std::cout << "rev diagonals rows:  " << std::endl;
    for(auto diag: _revdiag) {
        for(auto elt: diag) {
            std::cout << elt << ", "; 
        }
        std::cout << std::endl;
    }

}


void Hexagon::printGrid() {
    size_t cell_ind = 0;
    for(size_t row_ind = 0; row_ind < _N; row_ind ++) {
        for(size_t i = 0; i < _N-row_ind-1; i++) {
            std::cout << "   ";
        }
        for(size_t col_ind = 0; col_ind < _N+row_ind; col_ind ++) {
            std::cout << "    " << _grid[cell_ind].index;
            if(_grid[cell_ind].index < 10) {
                std::cout << " ";
            }
            cell_ind ++;
        }
        std::cout << std::endl << std::endl;
    }
    for(size_t row_ind = 0; row_ind < _N-1; row_ind ++) {
        for(size_t i = 0; i < row_ind+1; i++) {
            std::cout << "   ";
        }
        for(size_t col_ind = 0; col_ind < _N*2-2-row_ind; col_ind ++) {
            std::cout << "    " << _grid[cell_ind].index;
            if(_grid[cell_ind].index < 10) {
                std::cout << " ";
            }
            cell_ind += 1;
        }
        std::cout << std::endl << std::endl;
    }
}

void Hexagon::printGrid(std::vector<int>& mapping) {
    size_t cell_ind = 0;
    for(size_t row_ind = 0; row_ind < _N; row_ind ++) {
        for(size_t i = 0; i < _N-row_ind-1; i++) {
            std::cout << "   ";
        }
        for(size_t col_ind = 0; col_ind < _N+row_ind; col_ind ++) {
            std::cout << "    " << mapping[_grid[cell_ind].index];
            if(abs(mapping[_grid[cell_ind].index]) < 10) {
                std::cout << " ";
            }
            cell_ind ++;
        }
        std::cout << std::endl << std::endl;
    }
    for(size_t row_ind = 0; row_ind < _N-1; row_ind ++) {
        for(size_t i = 0; i < row_ind+1; i++) {
            std::cout << "   ";
        }
        for(size_t col_ind = 0; col_ind < _N*2-2-row_ind; col_ind ++) {
            std::cout << "    " << mapping[_grid[cell_ind].index];
            if(abs(mapping[_grid[cell_ind].index]) < 10) {
                std::cout << " ";
            }
            cell_ind += 1;
        }
        std::cout << std::endl << std::endl;
    }
}

// void Hexagon::debug_check(std::vector<int>& solution) {

//     std::cout << "solution: " << std::endl;
//     for(int& value: solution) {
//         std::cout << value << ", ";
//     }
//     std::cout << std::endl;

//     int sum = 0;
//     double mean;

//     std::vector<int> row_sum(_rows.size(), 0);
//     for(size_t row_ind = 0; row_ind < _rows.size(); row_ind ++) {
//         for(size_t var_ind: _rows[row_ind]) {
//             row_sum[row_ind] += solution[var_ind];
//         }
//         sum += row_sum[row_ind];
//     }

//     mean = static_cast<double>(sum)/row_sum.size();

//     // std::cout << "mean sum on each row: " << mean << std::endl;

//     std::cout << "sum on each rows: " << std::endl;
//     for(size_t row_ind = 0; row_ind < _rows.size(); row_ind ++) {
//         std::cout << row_ind << ": " << row_sum[row_ind] << std::endl;
//     }

//     double sum_error = 0.;
//     for(size_t row_ind = 0; row_ind < _rows.size(); row_ind ++) {
//         sum_error += fabs(row_sum[row_ind]-mean);
//     }
//     std::cout << "recomputed sum error: " << sum_error << std::endl;

//     int alldiff_error = 0;
//     std::vector<int> all_values(_nvalues, 0);
//     for(auto value: solution) {
//         if(all_values[_offset+value] >= 1) {
//             alldiff_error ++;
//         }
//         all_values[_offset+value] ++;
//     }
//     std::cout << "recomputed alldiff error: " << alldiff_error << std::endl;

//     printGrid(solution);

// }
