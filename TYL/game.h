#ifndef GAME_H_
#define GAME_H_

#include "Graphics.h"
#include "Input.h"
#include "Operators.h"
#include "Sprite.h"


#include <algorithm>
#include <list>
#include <map>
#include <utility>

class Game {
public:
	Game();
	Game(const Game &other);
	~Game();

	void copyFromBackup(const Game &other);

	void gameLoop();

	//Draws all Sprites
	void draw(Graphics &graphics);

	void drawGame(Graphics &graphics);
	void drawOperator(Graphics &graphics, int x, int y, char opr);
	void drawOperatorOutline(Graphics &graphics, int x, int y);
	void drawTile(Graphics &graphics, int x, int y, const Grid &grid);
	void drawTileOutline(Graphics &graphics, int x, int y, int w, int h);
	void drawGrid(Graphics &graphics, const Grid &grid, const int x, const int y);

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