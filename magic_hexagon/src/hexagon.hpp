/*
 * Author: Samuel Buchet
 * Year: 2026
 */

#pragma once

#include <vector>
#include <cstddef>

class HComb {

    public:
        HComb(int pindex) : index(pindex), east(-1), west(-1), nwest(-1), neast(-1), seast(-1), swest(-1) {}

        int index;
        int east, west;
        int nwest, neast;
        int seast, swest;

};

class Hexagon {

    public:
        
        Hexagon(size_t N);

        void printGrid();
        void printGrid(std::vector<int>& mapping);

	//void debug_check(std::vector<int>& solution);

    private:

        void buildGrid();

    public:

        size_t _N;
        size_t _nvar;
        int _range, _nvalues;
        int _min_val, _max_val;
        int _offset;

        std::vector<HComb> _grid;
        std::vector<std::vector<int>> _horiz, _diag, _revdiag;
        std::vector<std::vector<int>> _rows;



};
