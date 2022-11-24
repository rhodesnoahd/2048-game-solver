#include <iostream>
#include "ArgumentManager.h"
#include <fstream>
#include <vector>
#include <stack>
#include <math.h>

#include <chrono>

// using namespace std; // bad practice

typedef vector<int> Row;
typedef vector<Row> Matrix;

struct node {
    string shift; // 1, 2, 3, or 4
    Matrix m;
    bool solved;
    // bool different; // false if not identical to root matrix
};

// void checkDifferent(node& n_, const Matrix& m_, int size_)
// {
//     for(int i=0; i<size_; i++)
//     {
//         for(int j=0; j<size_; j++)
//         {
//             if(n_.m.at(i).at(j) != m_.at(i).at(j))
//             {
//                 n_.different = true;
//             }
//             if(n_.different) {return;}
//         }
//     }
// }
   
// helpers
void mergeStack(int i_, int j_, int size_, bool& addedToStack_, const Matrix& matrix_, stack<int>& s_, node& n_)
{
    if(matrix_.at(i_).at(j_) != 0) // only push nonzero values from matrix_, by default
    {
        // check 2048 UP
        if(n_.shift == "1" && matrix_.at(i_).at(j_) == 2048)
        {
            while(s_.size() < i_) {s_.push(0);}
            s_.push(2048);
            addedToStack_ = true;
            return;
        }
        // check 2048 RIGHT
        if(n_.shift == "2" && matrix_.at(i_).at(j_) == 2048)
        {
            while(s_.size() < size_-1-j_) {s_.push(0);}
            s_.push(2048);
            addedToStack_ = true;
            return;
        }
        // check 2048 DOWN
        if(n_.shift == "3" && matrix_.at(i_).at(j_) == 2048)
        {
            while(s_.size() < size_-1-i_) {s_.push(0);}
            s_.push(2048);
            addedToStack_ = true;
            return;
        }
        // check 2048 LEFT
        if(n_.shift == "4" && matrix_.at(i_).at(j_) == 2048)
        {
            while(s_.size() < j_) {s_.push(0);}
            s_.push(2048);
            addedToStack_ = true;
            return;
        }
        // check after merge, !2048
        if(addedToStack_ && s_.top() == 0 && matrix_.at(i_).at(j_) != 2048)
        {
            s_.pop(); // pop 0
            s_.push(matrix_.at(i_).at(j_));
            return;
        }
        // check merge, don't merge 2048s -> different
        if(addedToStack_ && matrix_.at(i_).at(j_) == s_.top() && matrix_.at(i_).at(j_) != 2048)
        {
            s_.pop(); // remove top
            s_.push(2* matrix_.at(i_).at(j_)); // push 2*value
            // n_.merged = true;
            // n_.different = true; // update different
            s_.push(0); // push a zero
        }
        else
        {
            s_.push(matrix_.at(i_).at(j_));
            addedToStack_ = true;
        }
    }
}
void fill0s(stack<int>& s_, int size_)
{
    while(s_.size() != size_) {s_.push(0);}
}
void popStackIntoRow(stack<int>& s_, Row& tRow_, int& nonZCount_)
{
    while(!s_.empty())
    {
        tRow_.push_back(s_.top());
        if(s_.top() != 0 && s_.top() != 2048) {nonZCount_++;}
        s_.pop();
    }
}
stack<int> reverseStack(stack<int>& s_)
{
    stack<int> tS;
    while(!s_.empty())
    {
        tS.push(s_.top());
        s_.pop();
    }
    return tS;
}
Matrix rotate90Counterclockwise(Matrix& matrix_, int size_)
{
    Matrix rMatrix;
    for(int j=size_-1; j>=0; j--)
    {
        Row tRow;
        for(int i=0; i<size_; i++)
        {
            tRow.push_back(matrix_.at(i).at(j));
        }
        rMatrix.push_back(tRow);
    }
    return rMatrix;
}

// shifts
node shiftUp(const Matrix& matrix_, int size_)
{
    // create node
    node n;
    // fill node
    n.shift = "1";
    // n.merged = false;
    // n.different = false;
    Matrix nMatrix;
    // vector<vector<int>> nMatrix(size_);
    int nonZCount = 0;  // to check solved
    // iterate thru columns l->r
    for(int j=0; j<size_; j++)
    {
        stack<int> s;
        bool addedToStack = false;
        Row tRow;
        // iterate thru rows top->bottom
        for(int i=0; i<size_; i++)
        {
            mergeStack(i,j,size_,addedToStack,matrix_,s,n);
        }
        fill0s(s, size_);
        popStackIntoRow(s, tRow, nonZCount);
        // push tRow into nMatrix
        nMatrix.push_back(tRow);        
    }
    // fill node
    n.m = rotate90Counterclockwise(nMatrix, size_);
    // if(!n.different) {checkDifferent(n, matrix_, size_);}
    n.solved = nonZCount == 1;
    return n;
}
node shiftRight(const Matrix& matrix_, int size_)
{
    // create node
    node n;
    // fill node
    n.shift = "2";
    // n.merged = false;
    // n.different = false;
    Matrix nMatrix;
    int nonZCount = 0;
    // iterate thru rows l->r
    for(int i=0; i<size_; i++)
    {
        stack<int> s;
        bool addedToStack = false;
        Row tRow;
        // iterate thru columns l<-r
        for(int j=size_-1; j>=0; j--)
        {
            mergeStack(i,j,size_,addedToStack,matrix_,s,n);
        }
        fill0s(s, size_);
        // pop stack into tRow
        popStackIntoRow(s, tRow, nonZCount);
        // push tRow into nMatrix
        nMatrix.push_back(tRow);
    }
    // fill node
    n.m = nMatrix;
    // if(!n.different) {checkDifferent(n, matrix_, size_);}
    n.solved = nonZCount == 1;
    return n;
}
node shiftDown(const Matrix& matrix_, int size_)
{
    // create node
    node n;
    // fill node
    n.shift = "3";
    // n.merged = false;
    // n.different = false;
    Matrix nMatrix;
    int nonZCount = 0;  // to check solved
    // iterate thru columns l->r
    for(int j=0; j<size_; j++)
    {
        stack<int> s;
        bool addedToStack = false;
        Row tRow;
        // iterate thru rows bottom->top
        for(int i=size_-1; i>=0; i--)
        {
            mergeStack(i,j,size_,addedToStack,matrix_,s,n);
        }
        fill0s(s, size_);
        // pop stack into tRow BACKWARDS
        stack<int> tS = reverseStack(s);
        popStackIntoRow(tS, tRow, nonZCount);
        // push tRow into nMatrix
        nMatrix.push_back(tRow); 
    }
    // fill node
    n.m = rotate90Counterclockwise(nMatrix, size_);
    // if(!n.different) {checkDifferent(n, matrix_, size_);}
    n.solved = nonZCount == 1;
    return n;
}
node shiftLeft(const Matrix& matrix_, int size_)
{
    // create node
    node n;
    // fill node
    n.shift = "4";
    // n.merged = false;
    // n.different = false;
    Matrix nMatrix;
    int nonZCount = 0;
    // iterate thru rows l->r
    for(int i=0; i<size_; i++)
    {
        stack<int> s;
        bool addedToStack = false;
        Row tRow;
        // iterate thru columns l->r
        for(int j=0; j<size_; j++)
        {
            mergeStack(i,j,size_,addedToStack,matrix_,s,n);
        }
        fill0s(s, size_);
        // pop stack into tRow BACKWARDS
        stack<int> tS = reverseStack(s);
        popStackIntoRow(tS, tRow, nonZCount);
        // push tRow into nMatrix
        nMatrix.push_back(tRow);
    }
    // fill node
    n.m = nMatrix;
    // if(!n.different) {checkDifferent(n, matrix_, size_);}
    n.solved = nonZCount == 1;
    return n;
}

string findShortestPath(const Matrix& matrix_, int size_)
{
    vector<node> pathTree;
    node root;
    root.shift = "";
    root.m = matrix_;
    root.solved = false;
    pathTree.push_back(root);

    string rPath;

    int pos = 0; // position in tree
    while(!pathTree.at(pos).solved)
    {
        // add shift up node
        node suNode = shiftUp(pathTree.at(pos/4).m, size_);
        pathTree.push_back(suNode);
        pos++;
        if(pathTree.at(pos).solved)
        {
            while(pos != 0)
            {
                rPath += pathTree.at(pos).shift;
                pos = (pos-1) / 4;
            }
            break;
        }
        // add shift right node
        node srNode = shiftRight(pathTree.at(pos/4).m, size_);
        pathTree.push_back(srNode);
        pos++;
        if(pathTree.at(pos).solved)
        {
            while(pos != 0)
            {
                rPath += pathTree.at(pos).shift;
                pos = (pos-1) / 4;
            }
            break;
        }
        // add shift down node
        node sdNode = shiftDown(pathTree.at(pos/4).m, size_);
        pathTree.push_back(sdNode);
        pos++;
        if(pathTree.at(pos).solved)
        {
            while(pos != 0)
            {
                rPath += pathTree.at(pos).shift;
                pos = (pos-1) / 4;
            }
            break;
        }
        // add shift left node
        node slNode = shiftLeft(pathTree.at(pos/4).m, size_);
        pathTree.push_back(slNode);
        pos++;
        if(pathTree.at(pos).solved)
        {
            while(pos != 0)
            {
                rPath += pathTree.at(pos).shift;
                pos = (pos-1) / 4;
            }
            break;
        }
        // HOW DO I SPEED THIS UP USING DIFFERENT??? (see struct)
        // NEED TO AVOID THE SAME MATRICIES BEING CREATED BY PRUNING
    }
    stack<char> s;
    string path = "";
    // reverse path
    for(auto &ch : rPath)
    {
        s.push(ch);
    }
    while(!s.empty())
    {
        path.push_back(s.top());
        s.pop();
    }
    return path;
};

// bottom-up dynamic programming
string solve2048(int level, Matrix& board, string path, ofstream& _ofs) {
    // node 
}


auto start = chrono::steady_clock::now();
int main(int argc, char* argv[])
{
    ArgumentManager am(argc, argv);
    string inputfilename = am.get("input");
    string outputfilename = am.get("output");
    ifstream ifs(inputfilename.c_str());
    ofstream ofs_default(outputfilename.c_str());
    ofs_default << "Impossible" << endl;
    ofs_default.close();
    
    // get size of matrix
    int size = 0;
    ifs >> size;

    // build matrix
    Matrix matrix;
    int nonZeroCount = 0;
    int sum = 0;
    for(int i=0; i<size; i++)
    {
        int value;
        Row tRow;
        for(int j=0; j<size; j++)
        {
            ifs >> value;
            if(value != 0 && value != 2048) {nonZeroCount++; sum += value;}
            tRow.push_back(value);
        }
        matrix.push_back(tRow);
    }
    
    // check return 0
    if(nonZeroCount == 1) 
    {
        ofstream ofs0(outputfilename.c_str()); 
        ofs0 << "0";
        ofs0.close();
        return 0;
    }

    // Check Impossible
    double exponent = log(sum) / log(2); // 2^exponent = sum
    int exp = exponent / 1; // is exponent an integer? 
    int sumMod2048 = sum%2048; // 2^sumMod2048_exponent = sumMod2048
    double sumMod2048_exponent = log(sumMod2048) / log(2);
    int sumMod2048_exp = sumMod2048_exponent / 1; // is sumMod2048_exponent an integer?
    
    /* If 2 numbers combine to form 2048 (1 or more times) the sum of the nonzero
    and non 2048 numbers of the matrix may not be a power of 2 since we discount 
    2048 when determining if the matrix is solved. So, we perform the power of 2
    check on sum AND sum%2048 to account for such cases. */

    if(exponent != exp && sumMod2048_exponent != sumMod2048_exp) 
    {
        return 0;
    }
 
    // Find shortest path
    string solution = findShortestPath(matrix, size);
    ofstream ofs(outputfilename.c_str());
    ofs << solution;

    cout<<"\n";
    auto end = chrono::steady_clock::now();
    auto diff = end - start;
    cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;
    return 0;
}