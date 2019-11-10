#ifndef  GLOBALS_H_
#define GLOBALS_H_

#include <vector>

namespace globals {
	static float SPRITE_SCALE = 14;

	const int SCREEN_WIDTH = 1920/SPRITE_SCALE;
	const int SCREEN_HEIGHT = 1080/SPRITE_SCALE;
}


namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2 {
	int x, y;
	Vector2() : x(0), y(0) {}
	Vector2(int x, int y) : x(x), y(y) {}

	Vector2 zero() {
		return Vector2(0, 0);
	}
};

struct Grid {
	int width_, height_;
	std::vector<std::vector<int> > data_;

	Grid() : 
		width_(0),
		height_(0)
	{}

	Grid(const Grid &o) {
		width_ = o.width_;
		height_ = o.height_;
		data_ = o.data_;
	}

	Grid(int width, int height) :
		width_(width),
		height_(height) 
	{
		for (size_t i = 0; i < height_; ++i) {
			data_.push_back(std::vector<int>(width_, 0));
		}
	}

	Grid(const std::vector<std::vector<int> >& data) :
		width_(data[0].size()),
		height_(data.size()),
		data_(data)
	{}

	bool sameDimensions(const Grid &a) {
		return (width_ == a.width_ && height_ == a.height_);
	}
};




#endif GLOBALS_H_

