#pragma once
#include<string>

#include"../Common.h"

struct Image
{
    Image(std::string _file);  

    Vec2 Size;
    SDL_Surface *Surface;
};
struct Texture
{
    Texture(Image *_image);
    Vec2 Size;
    SDL_Texture *Pixels;

	void Render(Vec2 _pos);
	void Render(int _x, int _y);

	void Render(Vec2 _pos, Vec2 _size);
};