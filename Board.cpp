#include "Board.h"

// constructor
Board::Board(const matrix& inputBoard) {
    m = inputBoard;
    nCol = m.at(0).size();
}
void Board::mergeStack(int i, int j) {
    // only push nonzero values
    if(m.at(i).at(j) != 0) {
        // check 2048 for shiftUp
        if(this->shiftOp.compare(shiftUpOp) == 0) {
            // while(this->size() <)
        }
    }
}

void Board::shiftUp() {
    
}