#include "game.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 75;
}

namespace input_keys {
	SDL_Scancode key_name = SDL_SCANCODE_0;
	SDL_Scancode quit_key = SDL_SCANCODE_ESCAPE;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	level_ = Level("menu");

	bool gameUpdated = true;

	//sprite_ = Sprite(); 
	/* Use this constructor instead: Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, 
									int width, int height, float posX, float posY); */

	
	//Reset these with every level change
	char currentOperator = ' ';
	std::pair<int, int> currentGrids = std::make_pair(-1, -1);

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//Start game loop
	while (true) {
		input.beginNewFrame();
		
		//SDL_WaitEvent cuts CPU usage from 17% to 0.1% !
		//SDL_PollEvent(&event) && 
		if (SDL_WaitEvent(&event) != 0) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
					gameUpdated = true;
				}
				if (input.wasKeyPressed(SDL_SCANCODE_Q)) {
					return;
				} 

				if (input.wasKeyPressed(SDL_SCANCODE_LEFT)) {
					level_.inputLeft();
				}
				if (input.wasKeyPressed(SDL_SCANCODE_RIGHT)) {
					level_.inputRight();
				}
				if (input.wasKeyPressed(SDL_SCANCODE_UP)) {
					level_.inputUp(); 
				}
				if (input.wasKeyPressed(SDL_SCANCODE_DOWN)) {
					level_.inputDown();
				}
				if (input.wasKeyPressed(SDL_SCANCODE_RETURN) || input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
					level_.inputReturn();
					if(level_.getName()=="menu"){
						if (level_.getCurrentSelection() == 1) {
							level_ = Level("jamesPuzzle.txt");
						} else if (level_.getCurrentSelection() == 2) {
							level_ = Level("options");
						} else if (level_.getCurrentSelection() == 3) {
							return;
						}
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_BACKSPACE)) { //Restore from backup
					level_.undo();
				}
				if (input.wasKeyPressed(SDL_SCANCODE_EQUALS)) {
					globals::SPRITE_SCALE++;
				}
				if (input.wasKeyPressed(SDL_SCANCODE_MINUS)) {
					globals::SPRITE_SCALE--;
				}
				if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
					level_ = Level("menu");
				}
				
			} else if (event.type = SDL_KEYUP) {
				input.keyUpEvent(event);
			} else if (event.type == SDL_QUIT) {
				return;
			}
		}
	
		const int CURRENT_TIME_MILLIS = SDL_GetTicks();
		int ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;
		update(std::min(ELAPSED_TIME_MILLIS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MILLIS;

		//This makes the game only redraw itself when neccessary -- a HUGE performance increase
		if(gameUpdated) {
			draw(graphics);
			gameUpdated = false;
		}
	}//end game loop
}

//Primary draw function across game
void Game::draw(Graphics &graphics) {
	graphics.clear();
	//sprite_.draw(graphics, sprite_.getX(), sprite_.getY());

	//These lines set the background to be blacks
	SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(graphics.getRenderer(), NULL);
	
	level_.drawLevel(graphics);

	graphics.flip();
}

void Game::update(float elapsedTime) {
	//sprite_.update(elapsedTime);
}