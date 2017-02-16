#include "matrix.h"
#include <iostream>

sudoku::matrix::matrix()
{
  int line, column;
  _cursor=0;
  for(line=0; line<9; line++)
    for(column=0; column<9; column++)
      _mtx[line][column] = new number(line,column);
}

sudoku::matrix::matrix(unsigned int mtx[9][9])
{
  int line, column;
  _cursor=0;
  for(line=0; line<9; line++)
    for(column=0; column<9; column++)
      this->_mtx[line][column] = new number(line, column, mtx[line][column]);
}

sudoku::matrix::~matrix()
{
  unsigned int line, column;
  for(line=0; line<9; line++)
    for(column=0; column<9; column++)
      delete this->_mtx[line][column];
}

void sudoku::matrix::print()
{
  int line, column;
  for(line=0; line<9; line++){
    if(line==0 || line==3 || line==6)
      std::cout << "|-------+-------+-------|\n";
    for(column=0; column<9; column++){
      if(column==0)
        std::cout << "|";
      if(column==2 || column==5)
        std::cout << " " << _mtx[line][column]->get_val() << " |";
      else
        std::cout << " " << _mtx[line][column]->get_val();
      if(column==8)
        std::cout << " |";
    }
    if(line==8)
      std::cout << "\n|-------+-------+-------|\n";
      
    std::cout << "\n";
  }
  
}

bool sudoku::matrix::add_number(unsigned int line, unsigned int colunm, unsigned int value){
  return _mtx[line][colunm]->set_val(value);
}

bool sudoku::matrix::solve(unsigned int idx)
{
  unsigned int num;
  unsigned int possibles;
  number **linear = _mtx[0];

  if(idx==-1){
    std::cout << "this is the end\n";
    return true;
  }

  if(idx==-2)
    idx=get_next(0);

  possibles=linear[idx]->get_possibles();
  std::cout << "possibles:0x" << std::hex << possibles << std::endl;

  for(num=1;num<=9;num++){
    if(possibles&number::_numbers[num])
      std::cout << "tryng adding " << num << " in idx " << idx << " ...\n";
    if((possibles&number::_numbers[num])&&linear[idx]->set_val(num)&&(solve(get_next(idx))))
      return true;
  }

  linear[idx]->set_val(0);
  std::cout << "failed idx:" << idx << std::endl;
  return false;
}

unsigned int sudoku::matrix::get_next(const unsigned int cursor)
{
  unsigned int idx, ret=-1, minor=1000, count, possibles;
  number **linear = _mtx[0];

  idx=(cursor==80)?0:cursor+1;
  for(;idx!=cursor;idx++){
    if(!linear[idx]->get_valid()){
      possibles = linear[idx]->get_possibles();
      count=0;
      for(unsigned int x=1;x<=9;x++){
        if(possibles&number::_numbers[x]){
          count++;
        }
      }
      if(minor>count){
        minor=count;
        ret=idx;
      }
    }
    if(idx==80) idx=-1;
  }
  std::cout << "\n\nidx:" << idx << ", count:" << count << std::endl;
  return ret;
}

int main(int argc, char* argv[])
{
//  int imtx[9][9] = {{1,7,2,3,5,6,8,0,9},
//                    {4,8,6,1,9,2,5,0,0},
//                    {5,9,3,8,4,7,2,1,0},
//                    {3,1,4,0,2,0,7,6,8},
//                    {0,5,9,7,3,8,0,0,1},
//                    {7,0,8,0,6,1,9,0,0},
//                    {2,0,1,0,8,9,0,7,4},
//                    {0,0,5,6,7,0,1,0,2},
//                    {8,3,0,2,0,0,0,0,5}};

 sudoku::matrix* pmtx;
  unsigned int imtx[9][9] = {{0,0,0,0,0,5,4,0,3},
                    {0,7,4,6,1,3,0,5,0},
                    {0,5,0,0,0,0,1,0,0},
                    {0,0,0,0,0,8,0,0,5},
                    {0,0,0,2,4,1,0,0,0},
                    {4,0,0,7,0,0,0,0,0},
                    {0,0,9,0,0,0,0,8,0},
                    {0,4,0,5,2,7,9,1,0},
                    {2,0,5,8,0,0,0,0,0}};

  pmtx = new sudoku::matrix(imtx);

  pmtx->print();

  if(pmtx->solve()){
    std::cout << "Sucesso!!!!\n";
    pmtx->print();
  }
  else{
    std::cout << "Naum deu.\n";
  }
  return 0;
}
