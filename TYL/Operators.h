#ifndef OPERATORS_H_
#define OPERATORS_H_

#include "Globals.h"
#include <iostream>

enum OperatorTypes {
	EMPTY = -1,
	BINARY = 0,
	UNARY = 1,
	DUPLICATORY = 2
};

class Operator {
public:
	virtual Grid grid() const { std::cout << "empty grid"; return Grid(); }
	virtual int type() const { return EMPTY; }

	virtual void operate(Grid* out, const Grid &a, const Grid &b) const { std::cout << "operating empty"; };
	virtual void operate(Grid* out, const Grid &a) const { std::cout << "operating empty"; };
};

class PlusOperator : public Operator {
public: 
	PlusOperator() {};

	Grid grid() const;
	int type() const { return BINARY; }

	void operate(Grid* out, const Grid &a, const Grid &b) const;
};

class MinusOperator : public Operator {
public: 
	MinusOperator() {};

	Grid grid() const;
	int type() const { return BINARY; }

	void operate(Grid* out, const Grid &a, const Grid &b) const;
};

class VortexOperator : public Operator {
public: 
	VortexOperator(int c1, int c2) : c1_(c1), c2_(c2) {};

	Grid grid() const;
	int type() const { return UNARY; }

	void operate(Grid* out, const Grid &a) const;

private:
	int c1_, c2_;
};

class BucketOperator : public Operator {
public: 
	//c1: the color to be bucketed
	//c2: the color to fill c1 with
	BucketOperator(int c1, int c2) : c1_(c1), c2_(c2) {};

	Grid grid() const;
	int type() const { return UNARY; }

	void operate(Grid* out, const Grid &a) const;

private:
	int c1_, c2_;
};


class DuplicateOperator : public Operator {
public: 
	DuplicateOperator() {};

	Grid grid() const;
	int type() const { return DUPLICATORY; }

	void operate(Grid* out, const Grid &a) const;
};

class MirrorOperator : public Operator {
public:
	MirrorOperator(bool isVertical) : isVertical_(isVertical) {}

	Grid grid() const;
	int type() const { return UNARY; }

	void operate(Grid* out, const Grid&a) const;
private:
	//If false, it's a horizontal mirror
	//If true, a vertical mirror
	bool isVertical_;
};

#endif
