#include "number.h"

namespace Sudoku
{
  class Matrix
  {
  private:
    Number* mtx[9][9];
    int cursor;

  public:
    Matrix();
    Matrix(int[9][9]);
    ~Matrix();

    void printMatrix();
    bool addNumber(int, int, int);
    bool solve(int=-2);
    int getNext(const int=0);
  };
}
