#include "level.h"


bool Level::loadLevel() {
	currentSelection_ = 1;
	currentTiles_ = std::make_pair(-1, -1);
	currentOperator_ = -1;


	//Level 1 - to be loaded in a class from a file
	{
		solution_ = Grid(4, 4);
		solution_.data_ = { {2, 0, 0, 2}, {0, 2, 2, 0}, {0, 2, 2, 0}, {2, 0, 0, 2}};
		operators_ = { '+', 'f', '-', '+'};

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
	}
	return true;
}

//Draws tiles, operators, and everything else needed to see the game
//If name == "menu", draws the menu 
void Level::drawLevel(Graphics &graphics) {
	if (name_ == "menu") {
		drawMenu(graphics);
		return;
	}

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
		} else {
			oprs.push_back('_');
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
		tiles.push_back(grids_[o]);
		if(o== currentTiles_.first || o == currentTiles_.second)
			tiles[o].data_[0][0] = -1;
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
	if(currentOperator_ != -1) {
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		drawOperator(graphics, 960-5*globals::SPRITE_SCALE, 540+5*globals::SPRITE_SCALE, operators_[currentOperator_]);
	}

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


void Level::drawMenu(Graphics &graphics) {
	SDL_Rect r;

	SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);

	//Draw bar above quit
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

	//Draw bar below quit
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

	//Draw bar btw puzzle select and options
	{
		r.x = 960 - 29.5*globals::SPRITE_SCALE;
		r.y = 540 - 15*globals::SPRITE_SCALE;
		r.w = (29*globals::SPRITE_SCALE)*2;
		r.h = globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.y -= globals::SPRITE_SCALE;
		r.w = globals::SPRITE_SCALE;
		r.h = 3*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.x = 960+28.5*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);
	}

	//Draw PUZZLE SELECT
	{
		if(1==currentSelection_)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);
		Grid puzzleSelect(49, 4);
		puzzleSelect.data_ = 
		{
			{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
		{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
		{ 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0 }
		};
		drawTile(graphics, 960-26.5*globals::SPRITE_SCALE, 540 - 27*globals::SPRITE_SCALE, puzzleSelect);
	}

	//Draw OPTIONS
	{
		if(2==currentSelection_)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);
		Grid options(27, 4);
		options.data_ = 
		{
			{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0 },
		{ 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 },
		{ 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 }
		};
		drawTile(graphics, 960-15.5*globals::SPRITE_SCALE, 540 - 12*globals::SPRITE_SCALE, options);
	}

	//Draw QUIT
	{
		if(3==currentSelection_)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);
		Grid quit(15, 4);
		quit.data_ = 
		{          
			{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0 }
		};
		drawTile(graphics, 960- 9.5*globals::SPRITE_SCALE, 540 + 5*globals::SPRITE_SCALE, quit);
	}
}

//Draws an operator and the operator art inside of it
//x, y: upper-left corner of the horizontal white box in the operator's outline
//if opr=='_', draws an outline instead of the operator
void Level::drawOperator(Graphics &graphics, int x, int y, char opr) {
	if (opr == '_') {
		drawOperatorOutline(graphics, x, y);
		return;
	}

	SDL_Rect r;

	//Draw operator's border 
	{
		r.x = x;
		r.y = y;
		r.w = 11*globals::SPRITE_SCALE - (operator_grids::isUnary(opr) ? 1 : 0)*globals::SPRITE_SCALE; //If a unary operator (one tile), show only one 'connector'
		r.h = 7*globals::SPRITE_SCALE;
		SDL_RenderFillRect(graphics.getRenderer(), &r);

		r.x += 2*globals::SPRITE_SCALE;
		r.y -= 1*globals::SPRITE_SCALE;
		r.w = 7 * globals::SPRITE_SCALE;
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
	}

	//Draw the operator's art
	{
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
}

//Draws a placeoutGrider operator outline -- for use when an operator is entered
void Level::drawOperatorOutline(Graphics & graphics, int x, int y) {
	SDL_Rect r;

	r.x = x;
	r.y = y;
	r.w = 11 * globals::SPRITE_SCALE;
	r.h = 7 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x += 4 * globals::SPRITE_SCALE;
	r.y -= globals::SPRITE_SCALE;
	r.w = 3 * globals::SPRITE_SCALE;
	r.h = 9 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);

	r.x -= 2 * globals::SPRITE_SCALE;
	r.y += globals::SPRITE_SCALE;
	r.w = 7 * globals::SPRITE_SCALE;
	r.h = 7 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x -= globals::SPRITE_SCALE;
	r.y += globals::SPRITE_SCALE;
	r.w += 2 * globals::SPRITE_SCALE;
	r.h -= 2 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x -= globals::SPRITE_SCALE;
	r.y += globals::SPRITE_SCALE;
	r.w += 2 * globals::SPRITE_SCALE;
	r.h -= 2 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);
}

//Draws a tile and the grid art inside of it
//x: upper-left corner of the tile's border
//y: upper-left corner of the tile's border
//if grid.data_[0][0] == -1, draws a tile outline instead of a tile
void Level::drawTile(Graphics & graphics, int x, int y, const Grid &grid) {
	if (grid.data_[0][0] == -1) {
		drawTileOutline(graphics, x, y, (grid.width_+4)*globals::SPRITE_SCALE, (grid.height_+4)*globals::SPRITE_SCALE);
		return;
	}
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

//Draws a placeoutGrider tile outline -- for use when a tile is entered
void Level::drawTileOutline(Graphics & graphics, int x, int y, int w, int h) {
	SDL_Rect r;

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);

	r.x += 2 * globals::SPRITE_SCALE;
	r.w -= 4 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x = x;
	r.y += 2 * globals::SPRITE_SCALE;
	r.w = w;
	r.h -= 4 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	r.x = x + globals::SPRITE_SCALE;
	r.y = y + globals::SPRITE_SCALE;
	r.w = w - 2 * globals::SPRITE_SCALE;
	r.h = h - 2 * globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);
}

//Draws the grid art inside a tile
void Level::drawGrid(Graphics &graphics, const Grid &grid, const int x, const int y) {
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



//Uses whatever operators and tiles are currently entered
void Level::tryOperator() {
	if (currentOperator_ == -1) return;

	char opr = operators_[currentOperator_];
	std::pair<int, int> gridIndices = currentTiles_;

	std::cout << "useoperator " << opr << std::endl;

	//Plus (+) operator
	if (gridIndices.first != -1 && gridIndices.second != -1 &&
		opr == '+' && std::find(operators_.begin(), operators_.end(), '+') != operators_.end()){
		std::cout << "plus\n";
		//Set up iterators
		std::vector<Grid>::iterator first = grids_.begin();
		std::vector<Grid>::iterator second = grids_.begin();
		first = first + gridIndices.first;
		second = second + gridIndices.second;

		//Create the new Grid
		Grid* outGrid = new Grid((*first).width_, (*first).height_);

		//Perform the operation
		add(*first, *second, outGrid);

		//Manage grids_ and operators_
		grids_.push_back(*outGrid);
		operators_.erase(std::find(operators_.begin(), operators_.end(), '+'));
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != gridIndices.first && i != gridIndices.second) tGrids.push_back(grids_[i]);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);
	}

	//Minus (-) operator
	if (gridIndices.first != -1 && gridIndices.second != -1 &&
		opr == '-' && std::find(operators_.begin(), operators_.end(), '-') != operators_.end()){

		Grid first = grids_[gridIndices.first];
		Grid second = grids_[gridIndices.second];

		//Create the new Grid
		Grid* outGrid = new Grid(first.width_, first.height_);

		//Perform the operation
		subtract(first, second, outGrid);

		//Manage grids_ and operators_
		grids_.push_back(*outGrid);
		operators_.erase(std::find(operators_.begin(), operators_.end(), '-'));
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != gridIndices.first && i != gridIndices.second) tGrids.push_back(grids_[i]);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);
	}

	//=================PLACE UNARY OPERATORS BELOW THIS LINE=====================================================================================================

	//Pushes a selected gridIndex to the front of the pair
	if (gridIndices.first == -1) {
		gridIndices.first = gridIndices.second;
		gridIndices.second = -1;
	}

	//Flip (f) operator
	if (gridIndices.first != -1 &&
		opr == 'f' && std::find(operators_.begin(), operators_.end(), 'f') != operators_.end()) {
		Grid first = grids_[gridIndices.first];

		//Create the new Grid
		Grid* outGrid = new Grid(first.width_, first.height_);

		flip(first, outGrid);

		grids_.push_back(*outGrid);
		operators_.erase(std::find(operators_.begin(), operators_.end(), 'f'));
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != gridIndices.first) tGrids.push_back(grids_[i]);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);
	}

	//Bucket (b) operator
	if (gridIndices.first != -1 &&
		opr == 'b' && std::find(operators_.begin(), operators_.end(), 'b') != operators_.end()) {
		Grid first = grids_[gridIndices.first];

		//Create the new Grid
		Grid* outGrid = new Grid(first.width_, first.height_);

		bucket(first, outGrid);

		grids_.push_back(*outGrid);
		operators_.erase(std::find(operators_.begin(), operators_.end(), 'b'));
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != gridIndices.first) tGrids.push_back(grids_[i]);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);
	}

	//TO-DO: flip and bucket

}

void Level::inputUp() {
	if(name_=="menu"){
		currentSelection_--;
		if (currentSelection_ == 0) currentSelection_ = 3;
	} else {
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
}

void Level::inputDown() {
	if(name_ == "menu"){
		currentSelection_++;
		if (currentSelection_ == 4) currentSelection_ = 1;
	} else {
		inputUp(); //Functionally identical during a game since it swaps btw two values
	}
}

void Level::inputLeft() {
	if (currentSelection_ > 0) {
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

	} else {
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
	}
}

void Level::inputRight() {
	if(currentSelection_ > 0){
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

	} else {
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
	}
}

void Level::inputReturn() {
	if(name_ == "menu"){
		
	} else {
		backup();

		//Manage currentSelection
		if (currentSelection_ > 0) {
			if (currentSelection_ - 1 == currentTiles_.first) {
				currentTiles_.first = -1;
			} else if (currentSelection_ - 1 == currentTiles_.second) {
				currentTiles_.second = -1;
			} else if (currentTiles_.first == -1) {
				currentTiles_.first = currentSelection_-1;
			} else if (currentTiles_.second == -1) {
				currentTiles_.second = currentSelection_-1;							
			}
		} else {
			if ((currentSelection_*-1) - 1 == currentOperator_)
				currentOperator_ = -1;
			else
				currentOperator_ = (currentSelection_*-1) - 1;
		}

		tryOperator();
		if (puzzleSolved()) {
			name_ = "menu";
		}
	}
}

//Merges grids a and b into out
void Level::add(const Grid& a, const Grid& b, Grid* out) {
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			(*out).data_[i][j] = a.data_[i][j] + b.data_[i][j];
			if((*out).data_[i][j] > 2) (*out).data_[i][j] = 2;
		}
	}
}

//Subtracts grid a from b absolutely (i.e. 1-2 = 1) into out
void Level::subtract(const Grid &a, const Grid &b, Grid* out){
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			int sum = (a.data_[i][j] - b.data_[i][j]);
			if(sum < 0) sum *= -1;
			(*out).data_[i][j] = sum;
		}
	}
}

//Flips any 1s to 2s, or 2s to 1s, into out
void Level::flip(Grid &a, Grid* out){
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			if(a.data_[i][j] == 1) (*out).data_[i][j] = 2;
			else if(a.data_[i][j] == 2) (*out).data_[i][j] = 1;
		}
	} 
}

//Buckets any 0s to 1s, in a
void Level::bucket(Grid &a, Grid *out){
	for(int i = 0; i < a.data_.size(); ++i){
		for(int j = 0; j < a.data_.size(); ++j){
			(*out).data_[i][j] = a.data_[i][j];
			if(a.data_[i][j] == 0) (*out).data_[i][j] = 1;
		}
	} 
}

//Loads the most previous backup
void Level::undo() {
	if (backups_.size() == 0) return;

	Level other = backups_.back();
	//currentSelection_ = other.currentSelection_;
	grids_ = other.grids_;
	operators_ = other.operators_;
	solution_ = other.solution_;
	palette_ = other.palette_;
	currentOperator_ = other.currentOperator_;
	currentTiles_ = other.currentTiles_;

	backups_.pop_back();
}