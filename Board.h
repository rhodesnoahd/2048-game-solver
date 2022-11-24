#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

typedef std::vector<int> row;
typedef std::vector<row> matrix;
// shift operation codes
std::string shiftUpOp    = "1";
std::string shiftRightOp = "2";
std::string shiftDownOp  = "3";
std::string shiftLeftOp  = "4";

class Board {
    private:
       matrix m;
       int nCol; // number of columns, rows (square board)
       std::string shiftOp;
       // helpers...
       void mergeStack(int i, int j);
    public:
        Board(const matrix&);
       ~Board();
        void shiftUp();
        void shiftRight();
        void shiftDown();
        void shiftLeft();
        bool isSolved();
        bool equalityCheck(const Board& a, const Board& b);
};

#endif