#ifndef SUDOKU_NUMBER_H
#define SUDOKU_NUMBER_H
#include <stdint.h>

namespace sudoku
{
  class number
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

    static const value _numbers[10];

  private:
    static uint16_t _line[9], _column[9], _square[9];
    int _my_val, _my_line, _my_column, _my_square;
    bool _valid;

    bool validation(unsigned int);

  public:
    number(unsigned int, unsigned int, unsigned int=0);
    unsigned int get_possibles();
    unsigned int get_val();
    bool set_val(unsigned int);
    bool get_valid();
  };
}
#endif  //SUDOKU_NUMBER_H
