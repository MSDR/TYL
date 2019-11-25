#include "level.h"


bool Level::loadLevel(int inRow, int inCol) {
	currentSelection_ = 1;
	currentTiles_ = std::make_pair(-1, -1);
	currentOperator_ = -1;

	if (name_ == "menu") {
		palette_.first.r = 255;
		palette_.first.g = 255;
		palette_.first.b = 255;
		palette_.first.a = 255;

		palette_.second.r = 45;
		palette_.second.g = 124;
		palette_.second.b = 249;
		palette_.second.a = 255;

		globals::SPRITE_SCALE = 14;

		return true;
	}

	if (name_ == "select") {
		currentTiles_ = std::make_pair(inRow, inCol);
	
		int row = 0;
		//while(true) isn't the best practice, 
		//but the loop breaks when it runs out of files to read so it shouldn't be an issue
		while (true) {
			int col = 0;
			while (true) {
				std::string filepath = "Levels/" + std::to_string(row); 
				filepath += "_" + std::to_string(col);
				filepath += ".txt";
				LevelPreview p(filepath);
				if (!p.isGood()) {
					if (col == 0)
						return true;
					else
						break; //and go to the next row
				}
				previews_[row].push_back(p);
				col++;
			}
			row++;
		}
		return false;
	}

	//Open and validate inputReader
	std::ifstream inputReader("Levels/"+name_); 
	if (!inputReader.good()) {
		return false;
	}

	std::string line = "";
	std::string word = "";

	//Read in operators
	std::getline(inputReader, line);

	//lineReader used to access individual words in a line
	std::istringstream lineReader(line);

	while (lineReader >> word) {
		char opr = word[0];
		std::cout << " " << opr;
		if (opr == '+') {        //PLUS
			operators_.push_back(new PlusOperator());
		} else if (opr == '-') { //MINUS
			operators_.push_back(new MinusOperator());
		} else if (opr == 'd') { //DUPLICATE
			operators_.push_back(new DuplicateOperator());
		} else if (opr == 'm') { //MIRROR
			if (word.size() != 2) continue;
			operators_.push_back(new MirrorOperator(word[1] == 'v'));
		} else if (opr == 'v') { //VORTEX
			if (word.size() != 3) continue;
			int c1 = word[1]-'0';
			int c2 = word[2]-'0';
			operators_.push_back(new VortexOperator(c1, c2));
		} else if (opr == 'b') { //BUCKET
			if (word.size() != 3) continue;
			int c1 = word[1]-'0';
			int c2 = word[2]-'0';
			operators_.push_back(new BucketOperator(c1, c2));
		}
	}

	std::getline(inputReader, line);

	//Read in palette
	std::getline(inputReader, line);
	lineReader = std::istringstream(line);
	lineReader >> word;
	palette_.first.r = std::stoi(word);
	lineReader >> word;
	palette_.first.g = std::stoi(word);
	lineReader >> word;
	palette_.first.b = std::stoi(word);
	lineReader >> word;
	palette_.first.a = std::stoi(word);

	lineReader.clear();

	std::getline(inputReader, line);
	lineReader.str(line);
	lineReader >> word;
	palette_.second.r = std::stoi(word);
	lineReader >> word;
	palette_.second.g = std::stoi(word);
	lineReader >> word;
	palette_.second.b = std::stoi(word);
	lineReader >> word;
	palette_.second.a = std::stoi(word);

	std::getline(inputReader, line);

	std::vector<std::vector<int> > data;
	int linePos = 0;
	int maxDim = 0;
	//Read data from input text line-by-line
	while (std::getline(inputReader, line)) {
		if (line[0] == '>') {
			linePos = 0;
			if (solution_.data_.empty())
				solution_ = Grid(data);
			else {
				grids_.push_back(Grid(data));
			}

			if (data.size() > maxDim)
				maxDim = data.size();
			if (data[0].size()/3 > maxDim)
				maxDim = data[0].size()/3;

			data.clear();
			continue;
		}
		data.push_back(std::vector<int>());
		for (size_t i = 0; i < line.size(); ++i) {
			data[linePos].push_back((int)(line[i])-48);
		}
		linePos++;
	}

	globals::SPRITE_SCALE = std::max(20 - (int)std::pow(maxDim, .8), 4);

	//Level 1 - to be loaded in a class from a file
	/*{
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
	}*/
	return true;
}

//Draws tiles, operators, and everything else needed to see the game
//If name == "menu", draws the menu 
void Level::drawLevel(Graphics &graphics) {
	if (name_ == "menu") {
		drawMenu(graphics);
		return;
	}
	if (name_ == "select") {
		drawPuzzleSelect(graphics);
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
	std::vector<Operator*> oprs;
	for(int o = 0; o < operators_.size(); ++o) {
		if(o!= currentOperator_){
			oprs.push_back(operators_[o]);
		} else {
			oprs.push_back(new Operator()); //EMPTY operator -- just an outline
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
		delete oprs[currentOperator_];
	}

	//Draw currentTiles_
	{
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		if (currentTiles_.first != -1) {
			drawTile(graphics, 960-(10+grids_[currentTiles_.first].width_)*globals::SPRITE_SCALE,
				540+5*globals::SPRITE_SCALE - (grids_[currentTiles_.first].height_%2==0 ? .5 : 1)*globals::SPRITE_SCALE, 
				grids_[currentTiles_.first]);
		}
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
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
		drawTile(graphics, 960-15.5*globals::SPRITE_SCALE, 540 - 11.5*globals::SPRITE_SCALE, options);
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

	//Draw TYL
	{
		if(4==currentSelection_)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 45, 125, 249, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);
		solution_.width_ = 9;
		solution_.height_ = 4;
		solution_.data_ = 
		{
			{ 1, 1, 1, 2, 0, 2, 1, 0, 0 },
			{ 0, 1, 0, 2, 0, 2, 1, 0, 0 },
			{ 0, 1, 0, 2, 2, 2, 1, 0, 0 },
			{ 0, 1, 0, 0, 2, 0, 1, 1, 1 }
		};
		drawTile(graphics, 960-solution_.width_*.5*globals::SPRITE_SCALE - (solution_.width_%2==1 ? 2 : 1.5)*globals::SPRITE_SCALE, 
			540+20*globals::SPRITE_SCALE, solution_);
	}
}

void Level::drawPuzzleSelect(Graphics & graphics) {
	int offsetX = 0;
	int offsetY = 0;
	//std::map<int, std::vector<LevelPreview> >::iterator itr = previews_.begin(); itr != previews_.end(); ++itr
	
	offsetY -= ((previews_[currentTiles_.first][0].getHeight())/2.0+2)*globals::SPRITE_SCALE;

	//Draw currently selected row
	for (int j = 0; j < previews_[currentTiles_.first].size(); ++j) {
		int x = 960 - (j>=1 ? (7+previews_[currentTiles_.first][j].getWidth())*(j-(j/2)) : 0)*globals::SPRITE_SCALE*(j%2==1 ? 1 : -1) +
			(previews_[currentTiles_.first].size()%2==0 ? 2 : -(previews_[currentTiles_.first][j].getWidth()*.5 + 2 - (previews_[currentTiles_.first][j].getWidth()%2==0 ? .5 : .5)))*globals::SPRITE_SCALE;
		int y = 540 + offsetY;

		if (j == currentTiles_.second)
			SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
		else
			SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);

		previews_[currentTiles_.first][j].drawPreview(graphics, x, y);
	}

	//draw above currently selected row (< currentTiles.first)
	if(currentTiles_.first-1 >= 0){
		for (int i = currentTiles_.first-1; i >= 0; --i) {
			offsetY -= (2 + previews_[i][0].getHeight() + 4)*globals::SPRITE_SCALE;
			for (int j = 0; j < previews_[i].size(); ++j) {
				int x = 960 - (j>=1 ? (7+previews_[i][j].getWidth())*(j-(j/2)) : 0)*globals::SPRITE_SCALE*(j%2==1 ? 1 : -1) +
					(previews_[i].size()%2==0 ? 2 : -(previews_[i][j].getWidth()*.5 + 2 - (previews_[i][j].getWidth()%2==0 ? .5 : .5)))*globals::SPRITE_SCALE;
				int y = 540 + offsetY;

				if (i == currentTiles_.first && j == currentTiles_.second)
					SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
				else
					SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);

				previews_[i][j].drawPreview(graphics, x, y);
			}
		}
	}

	offsetY = ((previews_[currentTiles_.first][0].getHeight())/2.0+4)*globals::SPRITE_SCALE;

	//draw below currently selected row (> currentTiles.first)
	if(currentTiles_.first+1 < previews_.size()){
		for (int i = currentTiles_.first+1; i < previews_.size(); ++i) {
			for (int j = 0; j < previews_[i].size(); ++j) {
				int x = 960 - (j>=1 ? (7+previews_[i][j].getWidth())*(j-(j/2)) : 0)*globals::SPRITE_SCALE*(j%2==1 ? 1 : -1) +
					(previews_[i].size()%2==0 ? 2 : -(previews_[i][j].getWidth()*.5 + 2 - (previews_[i][j].getWidth()%2==0 ? .5 : .5)))*globals::SPRITE_SCALE;
				int y = 540 + offsetY;

				if (i == currentTiles_.first && j == currentTiles_.second)
					SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 255, 255, 255);
				else
					SDL_SetRenderDrawColor(graphics.getRenderer(), 115, 115, 115, 255);

				previews_[i][j].drawPreview(graphics, x, y);
			}
			offsetY += (2 + previews_[i][0].getHeight() + 4)*globals::SPRITE_SCALE;
		}
	}
}

//Draws an operator and the operator art inside of it
//x, y: upper-left corner of the horizontal white box in the operator's outline
//if opr=='_', draws an outline instead of the operator
void Level::drawOperator(Graphics &graphics, int x, int y, Operator* opr) {
	if (opr->type() == EMPTY) {
		drawOperatorOutline(graphics, x, y);
		return;
	}

	SDL_Rect r;

	//Draw operator's border 
	{
		r.x = x;
		r.y = y;                          //If a unary operator (one tile), show only one 'connector'
		r.w = 11*globals::SPRITE_SCALE - (opr->type()==UNARY || opr->type()==DUPLICATORY ? 1 : 0)*globals::SPRITE_SCALE; 
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
		oGrid = opr->grid();
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
	if (currentOperator_ == -1) return; //No operator selected

	Operator* opr = operators_[currentOperator_];

	if (opr->type() == UNARY) {
		if (currentTiles_.first == -1) {
			currentTiles_.first = currentTiles_.second;
			currentTiles_.second = -1;
		}
		if (currentTiles_.first == -1) return; //No tile selected

		Grid first = grids_[currentTiles_.first];

		//Create the new Grid
		Grid outGrid = Grid(first.width_, first.height_);

		opr->operate(&outGrid, first);

		operators_.erase(operators_.begin() + currentOperator_);

		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != currentTiles_.first) 
				tGrids.push_back(grids_[i]);
			else
				tGrids.push_back(outGrid);
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);
	} else if (opr->type() == BINARY){
		if (currentTiles_.first == -1 || currentTiles_.second == -1) return; //No tile selected

		//Set up iterators
		std::vector<Grid>::iterator first = grids_.begin();
		std::vector<Grid>::iterator second = grids_.begin();
		first = first + currentTiles_.first;
		second = second + currentTiles_.second;

		//Create the new Grid
		Grid outGrid = Grid((*first).width_, (*first).height_);

		//Perform the operation
		opr->operate(&outGrid, *first, *second);

		//Manage grids_ and operators_
		operators_.erase(operators_.begin() + currentOperator_);
		std::vector<Grid> tGrids;
		for (size_t i = 0; i < grids_.size(); ++i) {
			if(i != currentTiles_.first && i != currentTiles_.second) {
				tGrids.push_back(grids_[i]);
			} else if(i == currentTiles_.first) {
				tGrids.push_back(outGrid);
			}
		}
		grids_.clear();
		grids_ = tGrids;
		tGrids.clear();

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);

		currentSelection_ = currentSelection_ > 0 ? 1 : -1;
	} else if (opr->type() == DUPLICATORY) {
		if (currentTiles_.first == -1) return;

		Grid first = grids_[currentTiles_.first];

		//Create the new Grid
		Grid outGrid = Grid(first.width_, first.height_);

		opr->operate(&outGrid, first);

		grids_.push_back(outGrid);
		operators_.erase(operators_.begin() + currentOperator_);

		currentOperator_ = -1;
		currentTiles_ = std::make_pair(-1, -1);
	}
}

void Level::inputUp() {
	if(name_ == "menu"){
		currentSelection_--;
		if (currentSelection_ == 0) currentSelection_ = 4;
	} else if (name_ == "select"){
		currentTiles_.first--;
		if (currentTiles_.first < 0) {
			currentTiles_.first = previews_.size()-1;
		}

		//Move to be horizontally within bounds
		int rowSize = previews_.find(currentTiles_.first)->second.size();
		if (rowSize == 1)
			currentTiles_.second = 0;
		else if (currentTiles_.second > rowSize-1)
			currentTiles_.second = rowSize - (currentTiles_.second%2==rowSize%2 ? 2 : 1);

	} else {
		if(currentSelection_ > 0) {
			if (currentSelection_ > operators_.size()) {
				currentSelection_ = operators_.size() - (currentSelection_%2==operators_.size()%2 ? 0 : 1);
			}
			currentSelection_ *= -1;

			if (operators_.size() == 1)
				currentSelection_ = -1;
		} else {
			currentSelection_ *= -1;
			if (currentSelection_ > grids_.size()) {
				currentSelection_ = grids_.size() - (currentSelection_%2==grids_.size()%2 ? 0 : 1);
			}

			if (grids_.size() == 1)
				currentSelection_ = 1;
		}
	}
}

void Level::inputDown() {
	if(name_ == "menu"){
		currentSelection_++;
		if (currentSelection_ == 5) currentSelection_ = 1;
	} else if (name_ == "select"){
		currentTiles_.first++;
		if (currentTiles_.first > previews_.size()-1) {
			currentTiles_.first = 0;
		}

		//Move to be horizontally within bounds
		int rowSize = previews_.find(currentTiles_.first)->second.size();
		if (rowSize == 1)
			currentTiles_.second = 0;
		else if (currentTiles_.second > rowSize-1)
			currentTiles_.second = rowSize - (currentTiles_.second%2==rowSize%2 ? 2 : 1);
	} else {
		inputUp(); //Functionally identical during a game since it swaps btw two values
	}
}

void Level::inputLeft() {
	if (name_ == "menu") {

	} else if (name_ == "select") {
		int maxX = previews_.find(currentTiles_.first)->second.size()-1;
		if (maxX == 0)
			return;
		else if (currentTiles_.second == maxX && maxX%2 == 1) //Left bound
			currentTiles_.second--;
		else if(currentTiles_.second == maxX-1 && maxX%2 == 0) //Left bound
			currentTiles_.second++;
		else if (currentTiles_.second == 0)
			currentTiles_.second++;
		else if(currentTiles_.second%2==0)
			currentTiles_.second -= 2;
		else 
			currentTiles_.second += 2;

	} else {
		if (currentSelection_ > 0) {
			if (grids_.size() == 1)
				return;
			else if(currentSelection_ == grids_.size() && grids_.size()%2==0) //Left bound
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
			if (operators_.size() == 1)
				return;
			else if(currentSelection_*-1 == operators_.size() && operators_.size()%2==0) //Left bound
				currentSelection_++;
			else if(currentSelection_*-1 == operators_.size()-1 && operators_.size()%2==1) //Left bound
				currentSelection_--;
			else if (currentSelection_ == -1)
				currentSelection_--;
			else if((currentSelection_*-1)%2==1)
				currentSelection_ += 2;
			else 
				currentSelection_ -= 2;
		}
	}
}

void Level::inputRight() {
	if (name_ == "menu") {

	} else if (name_ == "select") {
		int maxX = previews_.find(currentTiles_.first)->second.size()-1;
		if (maxX == 0)
			return;
		else if(currentTiles_.second == maxX && maxX%2 == 0) //Right bound
			currentTiles_.second--;
		else if(currentTiles_.second == maxX-1 && maxX%2 == 1) //Right bound
			currentTiles_.second++;
		else if (currentTiles_.second == 1) 
			currentTiles_.second--;
		else if(currentTiles_.second%2==0)
			currentTiles_.second += 2;
		else 
			currentTiles_.second -= 2;

	} else {
		if(currentSelection_ > 0){
			if (grids_.size() == 1)
				return;
			else if(currentSelection_ == grids_.size() && grids_.size()%2==1) //Right bound
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
			if (operators_.size() == 1)
				return;
			else if(currentSelection_*-1 == operators_.size() && operators_.size()%2==1) //Right bound
				currentSelection_++;
			else if(currentSelection_*-1 == operators_.size()-1 && operators_.size()%2==0) //Right bound
				currentSelection_--;
			else if (currentSelection_ == -2)
				currentSelection_++;
			else if((currentSelection_*-1)%2==1)
				currentSelection_ -= 2;
			else 
				currentSelection_ += 2;
		}
	}
}

void Level::inputReturn() {
	if(name_ == "menu" || name_ == "select"){
		return;
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

		if (currentSelection_ < 0 && operators_.size() == 0) {
			inputUp();
		}

		tryOperator();

		if (puzzleSolved()) {
			name_ = "menu";
			DestroyLevel();
			backups_.clear();
			loadLevel();
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
	currentOperator_ = other.currentOperator_;
	currentTiles_ = other.currentTiles_;

	backups_.pop_back();
}

std::string Level::getPuzzleFilepath() {
	if (name_ == "select") {
		return std::to_string(currentTiles_.first) + "_" 
			+ std::to_string(currentTiles_.second) + ".txt";
	} else {
		return name_;
	}
}

//Deletes dynamically allocated Operators
void Level::DestroyLevel() {
	Level other = backups_.front();
	for (std::vector<Operator*>::iterator itr = other.operators_.begin(); itr != other.operators_.end(); ++itr) {
		delete (*itr);
	}
}

