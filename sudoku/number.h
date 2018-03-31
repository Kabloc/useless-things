#ifndef SUDOKU_NUMBER_H
#define SUDOKU_NUMBER_H
#include <stdint.h>

namespace sudoku
{
class number
{
	friend class matrix;
public:
	enum value{
		num_0 = 0x0,  num_1 = 0x1,
		num_2 = 0x2,  num_3 = 0x4,
		num_4 = 0x8,  num_5 = 0x10,
		num_6 = 0x20, num_7 = 0x40,
		num_8 = 0x80, num_9 = 0x100
	};

	static const value _numbers[10];

private:
	static uint16_t _line[9], _column[9], _square[9];
	uint16_t _my_val, _my_line, _my_column, _my_square;
	bool _valid;

	bool validation(unsigned int);

//public:
	number(uint16_t, uint16_t, uint16_t=0);
	uint16_t get_possibles();
	uint16_t get_val();
	bool set_val(uint16_t);
	bool get_valid();
};
}
#endif  //SUDOKU_NUMBER_H
