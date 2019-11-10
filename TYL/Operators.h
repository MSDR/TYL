#ifndef OPERATORS_H_
#define OPERATORS_H_

#include "Globals.h"

namespace operator_grids {
	Grid plusGrid();

	Grid minusGrid();

	Grid flipGrid();

	Grid bucketGrid();

	bool isUnary(char opr);
};

#endif
