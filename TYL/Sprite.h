//Sprite class code adapted from LimeOats: https://limeoats.com/

#ifndef SPRITE_H_
#define SPRITE_H_

#include "SDL_include/SDL.h"
#include <string>

#include "Graphics.h"
#include "Rectangle.h"

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
	virtual ~Sprite();

	//Updates the Sprite and its members
	virtual void update(float elapsedTime);

	//Draws the Sprite onto the screen
	void draw(Graphics &graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const float getWidth() const;
	const float getHeight() const;
	const float getX() const;
	const float getY() const;

	//Updates the BoundingBox_ of the Sprite
	virtual void updateBoundingBox();

	//Given another Rectangle, returns the side of collision
	const sides::Side getCollisionSide(Rectangle &other) const;

	bool isGood() { return good_; }
protected:
	SDL_Rect sourceRect_;
	SDL_Texture* spriteSheet_;
	float x_;
	float y_;

	bool good_;

	Rectangle boundingBox_;
};

#endif SPRITE_H_