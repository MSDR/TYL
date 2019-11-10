#include "Operators.h"

namespace operator_grids {
	Grid plusGrid(){
		Grid plus(5, 5);
		plus.data_ = { {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {1, 1, 1, 1, 1}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0} };
		return plus;
	}

	Grid minusGrid(){
		Grid minus(5, 5);
		minus.data_ = { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} };
		return minus;
	}

	Grid flipGrid() {
		Grid flip(5, 5);
		flip.data_ = { {0, 1, 1, 2, 0}, {1, 1, 1, 1, 2}, {1, 2, 0, 1, 2}, {1, 2, 2, 2, 2}, {0, 1, 2, 2, 0} };
		return flip;
	}

	Grid bucketGrid() {
		Grid bucket(5, 5);
		bucket.data_ = { {0, 0, 0, 1, 0}, {0, 0, 0, 0, 1}, {0, 1, 1, 0, 1}, {0, 1, 1, 1, 1}, {0, 0, 1, 1, 0} };
		return bucket;
	}

	bool isUnary(char opr) { 
		return (opr == 'f' || opr == 'b'); 
	}
}