#include "Operators.h"

//==================== PLUS OPERATOR ======================
Grid PlusOperator::grid() const {
	Grid plus(5, 5);
	plus.data_ = { {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {2, 2, 2, 2, 2}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0} };
	return plus;
}
#include <iostream>
void PlusOperator::operate(Grid* out, const Grid& a, const Grid& b) const {
	for(int i = 0; i < a.height_; ++i){
		for(int j = 0; j < a.width_; ++j){
			(*out).data_[i][j] = a.data_[i][j] + b.data_[i][j];
			if((*out).data_[i][j] > 2) (*out).data_[i][j] = 2;
		}
	}
	std::cout << "operating plus";
}

//==================== MINUS OPERATOR ======================
Grid MinusOperator::grid() const {
	Grid minus(5, 5);
	minus.data_ = { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {2, 2, 2, 1, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} };
	return minus;
}

void MinusOperator::operate(Grid* out, const Grid& a, const Grid& b) const {
	for(int i = 0; i < a.height_; ++i){
		for(int j = 0; j < a.width_; ++j){
			int sum = (a.data_[i][j] - b.data_[i][j]);
			if(sum < 0) sum *= -1;
			(*out).data_[i][j] = sum;
		}
	}
}

//==================== VORTEX OPERATOR ======================
Grid VortexOperator::grid() const {
	Grid vortex(5, 5);
	vortex.data_ = { {0, c1_, c1_, c2_, 0}, {c1_, c1_, c1_, c1_, c2_}, {c1_, c2_, 0, c1_, c2_}, {c1_, c2_, c2_, c2_, c2_}, {0, c1_, c2_, c2_, 0} };
	return vortex;
}

void VortexOperator::operate(Grid* out, const Grid& a) const {
	for(int i = 0; i < a.height_; ++i){
		for(int j = 0; j < a.width_; ++j){
			if(a.data_[i][j] == c1_) (*out).data_[i][j] = c2_;
			else if(a.data_[i][j] == c2_) (*out).data_[i][j] = c1_;
		}
	} 
}

//==================== BUCKET OPERATOR ======================
Grid BucketOperator::grid() const {
	Grid bucket(5, 5);
	bucket.data_ = { {c1_, c1_, c2_, c2_, c2_}, {c1_, c2_, c1_, c2_, c2_}, {c1_, c1_, c2_, c1_, c2_}, {c1_, c2_, c1_, c2_, c2_}, {c1_, c1_, c2_, c2_, c2_} };
	return bucket;
}

void BucketOperator::operate(Grid* out, const Grid& a) const {
	for(int i = 0; i < a.height_; ++i){
		for(int j = 0; j < a.width_; ++j){
			(*out).data_[i][j] = a.data_[i][j];
			if(a.data_[i][j] == c1_) (*out).data_[i][j] = c2_;
		}
	} 
}

//==================== DUPLICATE OPERATOR ======================
Grid DuplicateOperator::grid() const {
	Grid dup(5, 5);
	dup.data_ = { {0, 1, 1, 1, 0}, {0, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, { 0, 1, 0, 0, 0}, {0, 1, 1, 1, 0} };
	return dup;
}

void DuplicateOperator::operate(Grid* out, const Grid& a) const {
	out->data_ = a.data_;
}

//==================== MIRROR OPERATOR ======================
Grid MirrorOperator::grid() const {
	Grid mir(5, 5);
	if (isVertical_)
		mir.data_ = { {1, 1, 0, 1, 1}, {1, 2, 0, 2, 1}, {2, 2, 0, 2, 2}, {1, 2, 0, 2, 1}, {1, 1, 0, 1, 1} };
	else
		mir.data_ = { {1, 1, 2, 1, 1}, {1, 2, 2, 2, 1}, {0, 0, 0, 0, 0}, {1, 2, 2, 2, 1}, {1, 1, 2, 1, 1} };
	return mir;

}

void MirrorOperator::operate(Grid* out, const Grid& a) const {
	if (!a.sameDimensions(*out)) return;
	for(int i = 0; i < a.height_; ++i){
		for(int j = 0; j < a.width_; ++j){
			if(isVertical_)
				(*out).data_[a.height_-1-i][j] = a.data_[i][j];
			else
				(*out).data_[i][a.width_-1-j] = a.data_[i][j];
		}
	} 
}