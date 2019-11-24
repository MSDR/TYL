#ifndef GAME_H_
#define GAME_H_

#include "Graphics.h"
#include "Input.h"
#include "level.h"
#include "Operators.h"
#include "Sprite.h"

#include "SDL_include/SDL.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <utility>

class Game {
public:
	Game();
	~Game();

	void gameLoop();

	//Draws all Sprites
	void draw(Graphics &graphics);

	//Updates member variables appropriately
	void update(float elapsedTime);

	//Sprite sprite_;

	Level level_;

	bool creditsPage_;
};

#endif GAME_H_