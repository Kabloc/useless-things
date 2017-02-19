#include "number.h"

namespace sudoku
{
class matrix
{
private:
	number* _mtx[9][9];
	unsigned int _cursor;

public:
	matrix();
	matrix(unsigned int[9][9]);
	~matrix();

	void print();
	bool add_number(unsigned int, unsigned int, unsigned int);
	bool solve(unsigned int=-2);
	unsigned int get_next(const unsigned int=0);
};
}
