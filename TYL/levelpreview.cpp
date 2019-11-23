#include "levelpreview.h"

//Returns false if invalid level file
bool LevelPreview::loadPreview() {
	//std::cout << "(loading filepath " << filepath_ << " )";
	std::ifstream inputReader;
	inputReader.open(filepath_);
	if (!inputReader.good())
		return false;

	std::string line = "";
	std::string word = "";

	//lineReader used to access individual words in a line
	std::istringstream lineReader(line);

	//Advance to palette
	std::getline(inputReader, line);
	std::getline(inputReader, line);

	//Read in palette
	{
		std::getline(inputReader, line);
		lineReader.str(line);
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
	}

	std::getline(inputReader, line);

	std::vector<std::vector<int> > data;
	int linePos = 0;

	while (std::getline(inputReader, line)) {
		if (line[0] == '>') {
			preview_ = Grid(data);
			return true;
		}
		data.push_back(std::vector<int>());
		for (size_t i = 0; i < line.size(); ++i) {
			data[linePos].push_back((int)(line[i])-48);
		}
		linePos++;
	}

	return false;
}


//Draws a tile and preview_'s art inside of it
//x: upper-left corner of the tile's border
//y: upper-left corner of the tile's border
//if preview_.data_[0][0] == -1, draws a tile outline instead of a tile
void LevelPreview::drawPreview(Graphics & graphics, int x, int y) {
	//std::cout << "\n drawing " << filepath_ << " at x: " << x << ", y: " << y;
	SDL_Rect r;
	//Left side
	r.x = x;
	r.y = y;
	r.w = globals::SPRITE_SCALE;
	r.h = globals::SPRITE_SCALE*(preview_.height_+4);
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	//Right side
	r.x += (preview_.width_+3)*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	//Top side
	r.x = x;
	r.y = y;
	r.w = globals::SPRITE_SCALE*(preview_.width_+4);
	r.h = globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	//Bottom side
	r.y += (preview_.height_+3)*globals::SPRITE_SCALE;
	SDL_RenderFillRect(graphics.getRenderer(), &r);

	drawGrid(graphics, x+2*globals::SPRITE_SCALE, y+2*globals::SPRITE_SCALE);

}

void LevelPreview::drawGrid(Graphics & graphics, int x, int y) {
	for (int i = 0; i < preview_.width_; ++i) {
		for (int j = 0; j < preview_.height_; ++j) {
			int pixel = preview_.data_[j][i];
			if (pixel == 0)
				continue;

			if (pixel == 1)
				SDL_SetRenderDrawColor(graphics.getRenderer(), palette_.first.r, palette_.first.g, palette_.first.b, palette_.first.a);
			if (pixel == 2)
				SDL_SetRenderDrawColor(graphics.getRenderer(), palette_.second.r, palette_.second.g, palette_.second.b, palette_.second.a);

			SDL_Rect preview_Point;
			preview_Point.x = x + (i*globals::SPRITE_SCALE);
			preview_Point.y = y + (j*globals::SPRITE_SCALE);
			preview_Point.w = globals::SPRITE_SCALE;
			preview_Point.h = globals::SPRITE_SCALE;

			SDL_RenderFillRect(graphics.getRenderer(), &preview_Point);
		}
	}
}
