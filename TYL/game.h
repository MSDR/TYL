//Sprite class code courtesy of LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2019]

#ifndef GAME_H_
#define GAME_H_

#include "Graphics.h"
#include "Input.h"
#include "Operators.h"
#include "Sprite.h"

#include <algorithm>
#include <list>
#include <map>


class Game {
public:
	Game();
	Game(const Game &other);
	~Game();

	void copyFromBackup(const Game &other);

//below should be private but having them public makes a copy constructor a lot easier lol
	void gameLoop();

	//Draws all Sprites
	void draw(Graphics &graphics);

	void drawGrids(Graphics &graphics);
	void drawGrid(Graphics &graphics, const Grid &grid, const int x, const int y);
	void drawTile(Graphics &graphics, int x, int y, const Grid &grid);
	void drawOperator(Graphics &graphics, int x, int y, char opr);

	//Updates member variables appropriately
	void update(float elapsedTime);

	void useOperator(char opr, std::pair<int, int> &gridIndices);

	void add(const Grid& a, const Grid& b, Grid* out);
	void subtract(const Grid& a, const Grid& b, Grid* out);
	void flip(Grid& a);
	void bucket(Grid& a);

	//Sprite sprite_;

	int currentSelection_ = 0;
	std::vector<Grid> grids_;
	std::vector<char> operators_;
	Grid solution_;
	std::pair<SDL_Color, SDL_Color> palette_; 

	int currentOperator_;
	std::pair<int, int> currentTiles_;

	std::list<Game> backups_;
};

#endif GAME_H_