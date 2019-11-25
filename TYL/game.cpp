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
	SDL_ShowCursor(SDL_DISABLE);
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
	creditsPage_ = false;

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//Start game loop
	while (true) {
		input.beginNewFrame();
		
		//Using SDL_WaitEvent over SDL_PollEvent cuts CPU usage from 17% to 0.1% !
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
							level_ = Level("select");
						} else if (level_.getCurrentSelection() == 3) {
							return;
						} else if (level_.getCurrentSelection() == 4) {
							creditsPage_ = true;
							level_ = Level(" ");
						}
					} else if(level_.getName()=="select"){
						level_ = Level(level_.getPuzzleFilepath());
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
					if (creditsPage_) {
						creditsPage_ = false;
						level_ = Level("menu");
					}
					std::string levelName = level_.getName();
					if (levelName == "select") {
						level_ = Level("menu");
					} else if (levelName == "menu") {
						continue;
					} else {
						int row = 0;
						int col = 0;
						int midIndex = 0;
						for (int i = 0; i < levelName.size(); ++i) {
							if (levelName[i] == '_') {
								row = std::stoi(levelName.substr(0, i));
								midIndex = i;
							}
							if (levelName[i] == '.') {
								col = std::stoi(levelName.substr(midIndex+1, i-midIndex+1));
								break;
							}
						}
						level_ = Level("select", row, col);
					}
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

		//The game only needs to draw on an input event. 
		//Only drawing when necessary drastically increases efficiency.
		if(gameUpdated) {
			draw(graphics);
			gameUpdated = false;
		}
	}//end game loop
}

//Primary draw function across game
void Game::draw(Graphics &graphics) {
	if (creditsPage_) {
		SDL_Texture* t = IMG_LoadTexture(graphics.getRenderer(), "./Images/RGSCreditPage.bmp");
		if (t == NULL) std::cerr << "ERROR: Credit page could not be loaded.";
		SDL_RenderCopy(graphics.getRenderer(), t, NULL, NULL);
		SDL_RenderPresent(graphics.getRenderer());
		SDL_DestroyTexture(t);
	} else {
		graphics.clear();
		//These lines set the background to be blacks
		SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
		SDL_RenderFillRect(graphics.getRenderer(), NULL);
		level_.drawLevel(graphics);
		graphics.flip();
	}
}

void Game::update(float elapsedTime) {
	//Function is currently unused.
}