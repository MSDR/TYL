#ifndef LEVELPREVIEW_H_
#define LEVELPREVIEW_H_

#include "Globals.h"
#include "level.h"

//A stripped-back version of Level, containing only the ability to read and draw the solution to a puzzle
class LevelPreview {
public:
	LevelPreview() {};
	LevelPreview(const std::string &filepath) : filepath_(filepath) {}

	bool loadPreview();

	std::string getFilepath() { return filepath_; }

	void drawPreview(Graphics &graphics, int x, int y);

private:
	void drawGrid(Graphics &graphics, int x, int y);

	Grid preview_;
	std::pair<SDL_Color, SDL_Color> palette_;

	std::string filepath_;
};

#endif