#include "number.h"
#include <stdio.h>

/*static*/ const Sudoku::Number::value Sudoku::Number::numbers[] = {Sudoku::Number::num_0, Sudoku::Number::num_1,
                                                                    Sudoku::Number::num_2, Sudoku::Number::num_3, 
                                                                    Sudoku::Number::num_4, Sudoku::Number::num_5, 
                                                                    Sudoku::Number::num_6, Sudoku::Number::num_7, 
                                                                    Sudoku::Number::num_8, Sudoku::Number::num_9};

int Sudoku::Number::vct_line[9] = {0};
int Sudoku::Number::vct_column[9] = {0};
int Sudoku::Number::vct_square[9] = {0};

Sudoku::Number::Number(int line, int column, int val)
{
  if(line>=0&&line<=2){
    if(column>=0&&column<=2)
      this->square = 0;
    else if(column>=3&&column<=5)
      this->square = 1;
    else if(column>=6&&column<=8)
      this->square = 2;
    else
      this->square = -1;
  }
  else if(line>=3&&line<=5){
    if(column>=0&&column<=2)
      this->square = 3;
    else if(column>=3&&column<=5)
      this->square = 4;
    else if(column>=6&&column<=8)
      this->square = 5;
    else
      this->square = -1;
  }
  else if(line>=6&&line<=8){
    if(column>=0&&column<=2)
      this->square = 6;
    else if(column>=3&&column<=5)
      this->square = 7;
    else if(column>=6&&column<=8)
      this->square = 8;
    else
      this->square = -1;
  }
  else
    this->square = -1;

  this->line = line;
  this->column = column;
  this->val=0;
  setVal(val); 
  if(val!=0)valid=true;
}

int Sudoku::Number::getVal(){
  return val;
}

bool Sudoku::Number::validation(int val){
  return (valid = (val)?(!((vct_line[line] | vct_column[column] | vct_square[square]) & numbers[val])):false);
}

int Sudoku::Number::getPossibles()
{
  int possibles;

  possibles = (vct_line[line] | vct_column[column] | vct_square[square]);
  if(!valid) possibles &= ~numbers[val];

  return 0x1FF & ~(possibles);
}

bool Sudoku::Number::setVal(int val)
{

  vct_line[line] &= ~numbers[this->val];
  vct_column[column] &= ~numbers[this->val];
  vct_square[square] &= ~numbers[this->val];

  validation(val);

  vct_line[line] |= numbers[val];
  vct_column[column] |= numbers[val];
  vct_square[square] |= numbers[val];

  this->val = val;
  return valid;
}

bool Sudoku::Number::getValid(){
  return valid;
}
