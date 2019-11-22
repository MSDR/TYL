#ifndef LEVELPREVIEW_H_
#define LEVELPREVIEW_H_

#include "Globals.h"
#include "graphics.h"

#include <fstream>
#include <iostream>
#include <SDL.h>
#include <string>
#include <sstream>

//A stripped-back version of Level, containing only the ability to read and draw the solution to a puzzle
class LevelPreview {
public:
	LevelPreview(const std::string &filepath) : filepath_(filepath) { good = loadPreview(); }

	bool loadPreview();

	bool isGood() { return good; }
	int getWidth() { return preview_.width_; }
	int getHeight() { return preview_.height_; }

	std::string getFilepath() { return filepath_; }

	void drawPreview(Graphics &graphics, int x, int y);

private:
	void drawGrid(Graphics &graphics, int x, int y);

	Grid preview_;
	std::pair<SDL_Color, SDL_Color> palette_;

	std::string filepath_;

	bool good;
};

#endif