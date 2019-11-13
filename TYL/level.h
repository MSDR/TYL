#ifndef LEVEL_H_
#define LEVEL_H_

#include "Globals.h"
#include "graphics.h"
#include "Operators.h"

#include "SDL_include/SDL.h"

#include <iostream>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

class Level {
public:
	Level() : name_("menu") { loadLevel(); }
	Level(std::string name) : name_(name) { loadLevel(); }
	Level(const Level &level) : 
		grids_(level.grids_),
		operators_(level.operators_), 
		currentOperator_(level.currentOperator_), 
		currentTiles_(level.currentTiles_) {}

	bool loadLevel();

	void drawLevel(Graphics &graphics);

	void tryOperator();

	void inputUp();
	void inputDown();
	void inputLeft();
	void inputRight();
	void inputReturn();

	void undo();
	void backup() { backups_.push_back(Level(*this)); }

	bool puzzleSolved() { return (grids_.size() == 1 && grids_[0].data_ == solution_.data_); }

	int getCurrentSelection() { return currentSelection_; }
	std::string getName() { return name_; }

private:
	void drawMenu(Graphics &graphics);

	void drawOperator(Graphics &graphics, int x, int y, char opr);
	void drawOperatorOutline(Graphics &graphics, int x, int y);
	void drawTile(Graphics &graphics, int x, int y, const Grid &grid);
	void drawTileOutline(Graphics &graphics, int x, int y, int w, int h);
	void drawGrid(Graphics &graphics, const Grid &grid, const int x, const int y);

	void add(const Grid& a, const Grid& b, Grid* out);
	void subtract(const Grid& a, const Grid& b, Grid* out);
	void flip(Grid& a, Grid *out);
	void bucket(Grid& a, Grid *out);
	void duplicate(Grid& a, Grid *out);

	std::string name_;

	int currentSelection_ = 0;
	std::vector<Grid> grids_;
	std::vector<char> operators_;
	Grid solution_;
	std::pair<SDL_Color, SDL_Color> palette_; 

	int currentOperator_;
	std::pair<int, int> currentTiles_;

	std::list<Level> backups_;
};
#endif 