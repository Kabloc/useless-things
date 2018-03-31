#include "matrix.h"
#include <iostream>

sudoku::matrix::matrix()
{
	int line, column;
	for(line=0; line<9; line++)
		for(column=0; column<9; column++)
			_mtx[line][column] = std::shared_ptr<number>(new number(line, column));
}

sudoku::matrix::matrix(unsigned int mtx[9][9])
{
	int line, column;
	for(line=0; line<9; line++)
		for(column=0; column<9; column++)
			this->_mtx[line][column] = std::shared_ptr<number>(new number(line, column, mtx[line][column]));
}

sudoku::matrix::~matrix()
{ }

void sudoku::matrix::print()
{
	int idx = 0;
	for (const auto line : _mtx) {
		if (!(idx%27)) std::cout << "|-------+-------+-------|" << std::endl;
		for (const auto column : line) {
			std::cout << (!(idx%9)?"| ":" ") << column->get_val() << (((idx%3)==2)?" |":"");
			idx++;
		}
		std::cout << std::endl;
	}
	std::cout << "|-------+-------+-------|" << std::endl;
}

bool sudoku::matrix::add_number(unsigned int line, unsigned int colunm, unsigned int value){
	return _mtx[line][colunm]->set_val(value);
}

bool sudoku::matrix::solve(unsigned int idx)
{
	uint16_t num;
	uint16_t possibles;

	if(idx==-1) {
		std::cout << "this is the end\n";
		return true;
	}

	if(idx==-2)
		idx=get_next(0);

	int l = idx/9, c = idx-(l*9);
	possibles=_mtx[l][c]->get_possibles();
	std::cout << "possibles:0x" << std::hex << possibles << std::endl;

	for(num=1;num<=9;num++) {
		if(possibles&number::_numbers[num])
			std::cout << "tryng adding " << num << " in idx " << idx << " ...\n";
		if((possibles & number::_numbers[num]) && _mtx[l][c]->set_val(num) && (solve(get_next(idx))))
			return true;
	}

	_mtx[l][c]->set_val(0);
	std::cout << "failed idx:" << idx << std::endl;
	return false;
}

unsigned int sudoku::matrix::get_next(const unsigned int cursor)
{
	unsigned int idx, ret=-1, minor=1000, count=0, possibles;

	idx=(cursor==80)?0:cursor+1;
	for(;idx!=cursor;idx++) {
		int l = idx/9, c = idx-(l*9);
		if(!_mtx[l][c]->get_valid()) {
			possibles = _mtx[l][c]->get_possibles();
			count=0;
			for(unsigned int x=1;x<=9;x++)
				if(possibles&number::_numbers[x]) count++;
			if(minor>count) {
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

