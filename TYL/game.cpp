//Sprite class code courtesy of LimeOats: https://limeoats.com/, modified and commented by Mason Sklar [2019]

#include <algorithm>?
#include "SDL_include/SDL.h"

#include "game.h"
#include <iostream>
namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 75;
}

namespace input_keys {
	SDL_Scancode key_name = SDL_SCANCODE_0;
	SDL_Scancode quit_key = SDL_SCANCODE_ESCAPE;
}

Game::Game(const Game &other) {
	copyFromBackup(other);
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	gameLoop();
}

Game::~Game() {

}

void Game::copyFromBackup(const Game &other) {
	//currentSelection_ = other.currentSelection_;
	grids_ = other.grids_;
	operators_ = other.operators_;
	solution_ = other.solution_;
	palette_ = other.palette_;
	currentOperator_ = other.currentOperator_;
	currentTiles_ = other.currentTiles_;
}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	bool gameUpdated = true;

	currentSelection_ = 1;
	currentTiles_ = std::make_pair(-1, -1);
	currentOperator_ = -1;

	//sprite_ = Sprite(); 
	/* Use this constructor instead: Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, 
									int width, int height, float posX, float posY); */
	solution_ = Grid(4, 4);
	solution_.data_ = { {0, 1, 2, 0}, {0, 0, 0, 0}, {1, 0, 1, 0}, {1, 0, 0, 0}};
	operators_ = { '+', '-', 'f', '+'};

	std::vector<std::vector<int>> a = { {1, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} };
	std::vector<std::vector<int>> b = { {0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 0, 0, 1} };
	std::vector<std::vector<int>> c = { {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0} };
	std::vector<std::vector<int>> d = { {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0} };
	grids_.push_back(Grid(a));
	grids_.push_back(Grid(b));
	grids_.push_back(Grid(c));
	grids_.push_back(Grid(d));


	palette_.first.r = 255;
	palette_.first.g = 255;
	palette_.first.b = 255;
	palette_.first.a = 255;


	palette_.second.r = 45;
	palette_.second.g = 124;
	palette_.second.b = 249;
	palette_.second.a = 255;

	char currentOperator = ' ';
	std::pair<int, int> currentGrids = std::make_pair(-1, -1);

	int LAST_UPDATE_TIME = SDL_GetTicks();
	//Start game loop
	while (true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
					gameUpdated = true;
				}
				if (input.wasKeyPressed(input_keys::quit_key)) {
					return;
				} 
				if (input.wasKeyPressed(SDL_SCANCODE_LEFT)) {
					if (currentSelection_ > 0) {
						for(int i = 1; i <= 2; ++i) {
							if(currentSelection_ == grids_.size() && grids_.size()%2==0) //Left bound
								currentSelection_--;
							else if(currentSelection_ == grids_.size()-1 && grids_.size()%2==1) //Left bound
								currentSelection_++;
							else if (currentSelection_ == 1)
								currentSelection_++;
							else if(currentSelection_%2==1)
								currentSelection_ -= 2;
							else 
								currentSelection_ += 2;

							if (currentSelection_-1 != currentGrids.first && currentSelection_-1 != currentGrids.second)
								break;
						}
					} else {
						for(int i = 1; i <= 2; ++i) {
							if(currentSelection_*-1 == grids_.size() && grids_.size()%2==0) //Left bound
								currentSelection_++;
							else if(currentSelection_*-1 == grids_.size()-1 && grids_.size()%2==1) //Left bound
								currentSelection_--;
							else if (currentSelection_ == -1)
								currentSelection_--;
							else if((currentSelection_*-1)%2==1)
								currentSelection_ += 2;
							else 
								currentSelection_ -= 2;

							if ((currentSelection_-1)*-1 != currentOperator_)
								break;
						}
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_RIGHT)) {
					if(currentSelection_ > 0){
						for(int i = 1; i <= 2; ++i){
							if(currentSelection_ == grids_.size() && grids_.size()%2==1) //Right bound
								currentSelection_--;
							else if(currentSelection_ == grids_.size()-1 && grids_.size()%2==0) //Right bound
								currentSelection_++;
							else if (currentSelection_ == 2) 
								currentSelection_--;
							else if(currentSelection_%2==1)
								currentSelection_ += 2;
							else 
								currentSelection_ -= 2;

							if (currentSelection_-1 != currentGrids.first && currentSelection_-1 != currentGrids.second)
								break;
						}
					} else {
						for(int i = 1; i <= 2; ++i){
							if(currentSelection_*-1 == grids_.size() && grids_.size()%2==1) //Right bound
								currentSelection_++;
							else if(currentSelection_*-1 == grids_.size()-1 && grids_.size()%2==0) //Right bound
								currentSelection_--;
							else if (currentSelection_ == -2)
								currentSelection_++;
							else if((currentSelection_*-1)%2==1)
								currentSelection_ -= 2;
							else 
								currentSelection_ += 2;

							if ((currentSelection_-1)*-1 != currentOperator_)
								break;
						}
					}

				}
				if (input.wasKeyPressed(SDL_SCANCODE_UP) || input.wasKeyPressed(SDL_SCANCODE_DOWN)) {
					if(currentSelection_ > 0) {
						if (currentSelection_ > operators_.size()) {
							currentSelection_ = operators_.size();
						}
						currentSelection_ *= -1;
					} else {
						if (currentSelection_*-1 > grids_.size()) {
							currentSelection_ = grids_.size();
						}
						currentSelection_ *= -1;
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_RETURN) || input.wasKeyPressed(SDL_SCANCODE_SPACE)) {
					std::cout << "return";
					backups_.push_back(Game(*this));

					if (currentSelection_ > 0) {
						if (currentTiles_.first == -1) {
							currentTiles_.first = currentSelection_-1;
						} else if (currentTiles_.second == -1) {
							currentTiles_.second = currentSelection_-1;							
						}
					} else {
						currentOperator_ = (currentSelection_*-1) - 1;
					}

					if (currentOperator_ != -1 && currentTiles_.first != -1 && currentTiles_.second != -1) {
						useOperator(operators_[currentOperator_], currentTiles_);
						currentOperator_ = -1;
						currentTiles_ = std::make_pair(-1, -1);
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_BACKSPACE) && backups_.size() > 0) { //Restore from backup -- idk if it really works lol
					copyFromBackup(backups_.back());
					backups_.pop_back();
				}
				if (input.wasKeyPressed(SDL_SCANCODE_S)) {
					globals::SPRITE_SCALE = 20;
				}
				if (input.wasKeyPressed(SDL_SCANCODE_A)) {
					globals::SPRITE_SCALE = 7;
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
		if(gameUpdated) {
			draw(graphics);
			gameUpdated = false;
		}
	}//end game loop
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	//sprite_.draw(graphics, sprite_.getX(), sprite_.getY());

	//These lines set the background to be blacks
	SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
	SDL_RenderFillRect(graphics.getRenderer(), NULL);
	
	drawGrids(graphics);

	graphics.flip();
}

void Game::drawGrids(Graphics &graphics) {

	SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);
	SDL_Rect r;

	//Draw bar above current selection
	{
		r.x = 960-22*globals::SPRITE_SCALE;
		r.y = 540-globals::SPRITE_SCALE;
		r.w = 44*globals::SPRITE_SCALE;
		r.h = 2*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.w = globals::SPRITE_SCALE;
		r.h = 3*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.x = 960+22*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);
	}

	//Draw bar below current selection
	{
		r.x = 960 - 16*globals::SPRITE_SCALE;
		r.y = 540 + 16*globals::SPRITE_SCALE;
		r.w = 33*globals::SPRITE_SCALE;
		r.h = 2*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.y -= globals::SPRITE_SCALE;
		r.w = globals::SPRITE_SCALE;
		r.h = globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.x += 32*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);
	}
	 
	//Draw solution
	drawTile(graphics, 960-solution_.width_*.5*globals::SPRITE_SCALE - (solution_.width_%2==1 ? 2 : 1.5)*globals::SPRITE_SCALE, 540+20*globals::SPRITE_SCALE, solution_);

	int maxX = 960;

	//Copy only available operators to be drawn
	std::vector<char> oprs;
	for(int o = 0; o < operators_.size(); ++o) {
		if(o!= currentOperator_){
			oprs.push_back(operators_[o]);
		}
	}

	//Draw available operators
	for(int o = 0; o < oprs.size(); ++o) {
		int x = 960 - 12*(o>=1 ? o-(o/2) : 0)*globals::SPRITE_SCALE*(o%2==1 ? 1 : -1) - 5*globals::SPRITE_SCALE + 
			(oprs.size()%2==0 ? 6 : 0)*globals::SPRITE_SCALE;
		if (x < maxX) maxX = x;
		int y = 540 - 11*globals::SPRITE_SCALE;

		if(o==(currentSelection_*-1)-1)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);

		drawOperator(graphics, x, y, oprs[o]);
	}

	//Copy only available tiles to be drawn
	std::vector<Grid> tiles;
	for(int o = 0; o < grids_.size(); ++o){
		if(o!= currentTiles_.first && o != currentTiles_.second){
			tiles.push_back(grids_[o]);
		}
	}

	//Draw available tiles
	for(int o = 0; o < tiles.size(); ++o){
		int x = 960 - (o>=1 ? (7+tiles[o].width_)*(o-(o/2)) : 0)*globals::SPRITE_SCALE*(o%2==1 ? 1 : -1) +
			(tiles.size()%2==0 ? 2 : -(tiles[o].width_*.5 + 2 - (tiles[o].width_%2==0 ? .5 : .5)))*globals::SPRITE_SCALE;
		if (x < maxX) maxX = x;
		int y = 540 - (21+tiles[o].height_)*globals::SPRITE_SCALE;

		if(o==currentSelection_-1)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);

		drawTile(graphics, x, y, tiles[o]);
		
	}

	//Draw currentOperator_
	if(currentOperator_ != -1)
		drawOperator(graphics, 960-5*globals::SPRITE_SCALE, 540+5*globals::SPRITE_SCALE, operators_[currentOperator_]);

	//Draw currentTiles_
	{
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		if (currentTiles_.first != -1) {
			drawTile(graphics, 960-(10+grids_[currentTiles_.first].width_)*globals::SPRITE_SCALE,
						540+5*globals::SPRITE_SCALE - (grids_[currentTiles_.first].height_%2==0 ? .5 : 1)*globals::SPRITE_SCALE, 
						grids_[currentTiles_.first]);
		}

		if (currentTiles_.second != -1) {
			drawTile(graphics, 960+7*globals::SPRITE_SCALE, 
						540+5*globals::SPRITE_SCALE - (grids_[currentTiles_.second].height_%2==0 ? .5 : 1)*globals::SPRITE_SCALE, 
						grids_[currentTiles_.second]);
		}
	}



	//Draw bar btw tiles and operators 
	{
		SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);
		r.x = maxX - 3*globals::SPRITE_SCALE;
		r.y = 540 - 15*globals::SPRITE_SCALE;
		r.w = (960-maxX)*2 + 6*globals::SPRITE_SCALE;
		r.h = globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.y -= globals::SPRITE_SCALE;
		r.w = globals::SPRITE_SCALE;
		r.h = 3*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.x = 960+(960-maxX) + 3*globals::SPRITE_SCALE;;
		SDL_RenderFillRect(graphics.getRenderer(), &r);
	}
}

//x, y should be the coords of the upper-left corner of the horizontal white box in the operator's outline
void Game::drawOperator(Graphics &graphics, int x, int y, char opr) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = 11*globals::SPRITE_SCALE;
	r.h = 7*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x += 2*globals::SPRITE_SCALE;
	r.y -= 1*globals::SPRITE_SCALE;
	r.w = 7*globals::SPRITE_SCALE;
	r.h = 9*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);

	r.x += globals::SPRITE_SCALE;
	r.y += globals::SPRITE_SCALE;
	r.w = 5*globals::SPRITE_SCALE;
	r.h = 7*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x -= globals::SPRITE_SCALE;
	r.y += globals::SPRITE_SCALE;
	r.w = 7*globals::SPRITE_SCALE;
	r.h = 5*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x += globals::SPRITE_SCALE;
	Grid oGrid(5, 5);
	if (opr == '+') {
		oGrid = operator_grids::plusGrid();
	} else if (opr == '-') {
		oGrid = operator_grids::minusGrid();
	} else if (opr == 'f') {
		oGrid = operator_grids::flipGrid();
	} else if (opr == 'b') {
		oGrid = operator_grids::bucketGrid();
	}

	drawGrid(graphics, oGrid, r.x, r.y);
}

void Game::drawGrid(Graphics &graphics, const Grid &grid, const int x, const int y) {
	for (int i = 0; i < grid.width_; ++i) {
		for (int j = 0; j < grid.height_; ++j) {
			int pixel = grid.data_[j][i];
			if (pixel == 0)
				continue;

			if (pixel == 1)
				SDL_SetRenderDrawColor(graphics.getRenderer(), palette_.first.r, palette_.first.g, palette_.first.b, palette_.first.a);
			if (pixel == 2)
				SDL_SetRenderDrawColor(graphics.getRenderer(), palette_.second.r, palette_.second.g, palette_.second.b, palette_.second.a);

			SDL_Rect gridPoint;
			gridPoint.x = x + (i*globals::SPRITE_SCALE);
			gridPoint.y = y + (j*globals::SPRITE_SCALE);
			gridPoint.w = globals::SPRITE_SCALE;
			gridPoint.h = globals::SPRITE_SCALE;

			SDL_RenderFillRect(graphics.getRenderer(), &gridPoint);
		}
	}
}

void Game::drawTile(Graphics & graphics, int x, int y, const Grid & grid) {
	SDL_Rect r;
	//Left side
	r.x = x;
	r.y = y;
	r.w = globals::SPRITE_SCALE;
	r.h = globals::SPRITE_SCALE*(grid.height_+4);
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	//Right side
	r.x += (grid.width_+3)*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	//Top side
	r.x = x;
	r.y = y;
	r.w = globals::SPRITE_SCALE*(grid.width_+4);
	r.h = globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	//Bottom side
	r.y += (grid.height_+3)*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	drawGrid(graphics, grid, x+2*globals::SPRITE_SCALE, y+2*globals::SPRITE_SCALE);
}

void Game::update(float elapsedTime) {
	//sprite_.update(elapsedTime);

}

void Game::useOperator(char opr, std::pair<int, int> &gridIndices) {
	std::cout << "useoperator " << opr << std::endl;

	if (opr == '+' && std::find(operators_.begin(), operators_.end(), '+') != operators_.end()){
		std::cout << "plus\n";
		//Set up iterators
		std::vector<Grid>::iterator first = grids_.begin();
		std::vector<Grid>::iterator second = grids_.begin();
		first = first + gridIndices.first;
		second = second + gridIndices.second;

		//Create the new Grid
		Grid* hold = new Grid((*first).width_, (*first).height_);

		//Perform the operation
		add(*first, *second, hold);

		//Manage grids_ and operators_
		grids_.push_back(*hold);
		operators_.erase(std::find(operators_.begin(), operators_.end(), '+'));
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != gridIndices.first && i != gridIndices.second) tGrids.push_back(grids_[i]);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();
	}
	if (opr == '-'&& std::find(operators_.begin(), operators_.end(), '-') != operators_.end()){
		//Set up iterators
		std::vector<Grid>::iterator first = grids_.begin();
		std::vector<Grid>::iterator second = grids_.begin();
		first = first + gridIndices.first;
		second = second + gridIndices.second;

		//Create the new Grid
		Grid* hold = new Grid((*first).width_, (*first).height_);

		//Perform the operation
		subtract(*first, *second, hold);

		//Manage grids_ and operators_
		grids_.push_back(*hold);
		operators_.erase(std::find(operators_.begin(), operators_.end(), '-'));
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != gridIndices.first && i != gridIndices.second) tGrids.push_back(grids_[i]);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();
	}
	
	
}

//Merges grids a and b into out
void Game::add(const Grid& a, const Grid& b, Grid* out) {
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			(*out).data_[i][j] = a.data_[i][j] + b.data_[i][j];
			if((*out).data_[i][j] > 2) (*out).data_[i][j] = 2;
		}
	}
}

//Subtracts grid a from b (absolutely, i.e. 1-2 = 1) into out
void Game::subtract(const Grid &a, const Grid &b, Grid* out){
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			int sum = (a.data_[i][j] - b.data_[i][j]);
			if(sum < 0) sum *= -1;
			(*out).data_[i][j] = sum;
		}
	}
}

//Flips any 1s to 2s, or 2s to 1s, in a
void Game::flip(Grid &a){
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			if(a.data_[i][j] == 1) a.data_[i][j] = 2;
			else if(a.data_[i][j] == 2) a.data_[i][j] = 1;
		}
	} 
}

//Buckets any 0s to 1s, in a
void Game::bucket(Grid &a){
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			if(a.data_[i][j] == 0) a.data_[i][j] = 1;
		}
	} 
}

