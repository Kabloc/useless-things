#include "matrix.h"
#include <stdio.h>

Sudoku::Matrix::Matrix()
{
  int line, column;
  cursor=0;
  for(line=0; line<9; line++)
    for(column=0; column<9; column++)
      mtx[line][column] = new Number(line,column);
}

Sudoku::Matrix::Matrix(int mtx[9][9])
{
  int line, column;
  cursor=0;
  for(line=0; line<9; line++)
    for(column=0; column<9; column++)
      this->mtx[line][column] = new Number(line, column, mtx[line][column]);
}

Sudoku::Matrix::~Matrix()
{
  int line, column;
  for(line=0; line<9; line++)
    for(column=0; column<9; column++)
      delete this->mtx[line][column];
}

void Sudoku::Matrix::printMatrix()
{
  int line, column;
  for(line=0; line<9; line++){
    if(line==0 || line==3 || line==6)
      printf("|-------+-------+-------|\n");
    for(column=0; column<9; column++){
      if(column==0)
        printf("|");
      if(column==2 || column==5)
        printf(" %d |", mtx[line][column]->getVal());
      else
        printf(" %d", mtx[line][column]->getVal());
      if(column==8)
        printf(" |");
    }
    if(line==8)
      printf("\n|-------+-------+-------|\n");
      
    printf("\n");
  }
  
}

bool Sudoku::Matrix::addNumber(int line, int colunm, int value){
  return mtx[line][colunm]->setVal(value);
}

bool Sudoku::Matrix::solve(int idx)
{
  int num;
  int possibles;
  Number **linear = mtx[0];

  if(idx==-1){
    printf("this is the end\n");
    return true;
  }

  if(idx==-2)
    idx=getNext(0);

  possibles=linear[idx]->getPossibles();
  printf("possibles:0x%x\n", possibles);

  if (possibles){
    for(num=1;num<=9;num++){
      if(possibles&Number::numbers[num])
        printf("tryng adding %d in idx %d ...\n", num, idx);
      if((possibles&Number::numbers[num])&&linear[idx]->setVal(num)&&(solve(getNext(idx))))
        return true;
    }
  }

  linear[idx]->setVal(0);
  printf("failed idx:%d\n", idx);
  return false;
}

int Sudoku::Matrix::getNext(const int cursor)
{
  int idx, ret=-1, minor=1000, count, possibles;
  Number **linear = mtx[0];

  idx=(cursor==80)?0:cursor+1;
  for(;idx!=cursor;idx++){
    if(!linear[idx]->getValid()){
      possibles = linear[idx]->getPossibles();
      count=0;
      for(int x=1;x<=9;x++){
        if(possibles&Number::numbers[x]){
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
  printf("\n\nidx:%d, count:%d\n", idx, count);
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

  Sudoku::Matrix* pmtx;
  int imtx[9][9] = {{0,0,0,0,0,5,4,0,3},
                    {0,7,4,6,1,3,0,5,0},
                    {0,5,0,0,0,0,1,0,0},
                    {0,0,0,0,0,8,0,0,5},
                    {0,0,0,2,4,1,0,0,0},
                    {4,0,0,7,0,0,0,0,0},
                    {0,0,9,0,0,0,0,8,0},
                    {0,4,0,5,2,7,9,1,0},
                    {2,0,5,8,0,0,0,0,0}};

  pmtx = new Sudoku::Matrix(imtx);

  pmtx->printMatrix();

  if(pmtx->solve()){
    printf("Sucesso!!!!\n");
    pmtx->printMatrix();
  }
  else{
    printf("Naum deu.\n");
  }
  return 0;
}

