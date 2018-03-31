#include "number.h"
#include <stdio.h>
#include <stdlib.h>

/*static*/ const sudoku::number::value sudoku::number::_numbers[] = 
{sudoku::number::num_0, sudoku::number::num_1,
 sudoku::number::num_2, sudoku::number::num_3,
 sudoku::number::num_4, sudoku::number::num_5,
 sudoku::number::num_6, sudoku::number::num_7,
 sudoku::number::num_8, sudoku::number::num_9};

uint16_t sudoku::number::_line[9] = {0};
uint16_t sudoku::number::_column[9] = {0};
uint16_t sudoku::number::_square[9] = {0};

sudoku::number::number(uint16_t line, uint16_t column, uint16_t val)
   : _my_line(line)
   , _my_column(column)
   , _my_val(0)
{
	this->_my_square = (abs((line%3)-line)/3)*3+(abs((column%3)-column)/3);
	set_val(val);
	//if(val!=0)valid=true;
}

uint16_t sudoku::number::get_val(){
	return _my_val;
}

bool sudoku::number::validation(unsigned int val){
	return (_valid = (val)?(!((_line[_my_line] | _column[_my_column] | _square[_my_square]) & _numbers[val])):false);
}

uint16_t sudoku::number::get_possibles()
{
	uint16_t possibles;

	possibles = (_line[_my_line] | _column[_my_column] | _square[_my_square]);
	if(!_valid) possibles &= ~_numbers[_my_val];

	return 0x1FF & ~(possibles);
}

bool sudoku::number::set_val(uint16_t val)
{
	_line[_my_line] &= ~_numbers[_my_val];
	_column[_my_column] &= ~_numbers[_my_val];
	_square[_my_square] &= ~_numbers[_my_val];

	validation(val);

	_line[_my_line] |= _numbers[val];
	_column[_my_column] |= _numbers[val];
	_square[_my_square] |= _numbers[val];

	_my_val = val;
	return _valid;
}

bool sudoku::number::get_valid(){
	return _valid;
}
