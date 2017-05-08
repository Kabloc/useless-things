#include "number.h"
#include <array>
#include <memory>

namespace sudoku
{
class matrix
{
private:
	std::array<std::array<std::shared_ptr<number>, 9>, 9> _mtx;

public:
	matrix();
	matrix(unsigned int[9][9]);
	~matrix();

	void print();
	bool add_number(unsigned int, unsigned int, unsigned int);
	bool solve(unsigned int=-2);

private:
	unsigned int get_next(const unsigned int=0);
};
}
