#ifndef SUDOKU_NUMBER_H
#define SUDOKU_NUMBER_H

namespace Sudoku
{
  class Number 
  {
  public:
    enum value{
      num_0 = 0x0,
      num_1 = 0x1,
      num_2 = 0x2,
      num_3 = 0x4,
      num_4 = 0x8,
      num_5 = 0x10,
      num_6 = 0x20,
      num_7 = 0x40,
      num_8 = 0x80,
      num_9 = 0x100
    };

    static const value numbers[10];

  private:
    static int vct_line[9], vct_column[9], vct_square[9];
    int val, line, column, square;
    bool valid;

    bool validation(int);

  public:
    Number(int, int, int=0);
    int getPossibles();
    int getVal();
    bool setVal(int);
    bool getValid();
  };
}
#endif  //SUDOKU_NUMBER_H
