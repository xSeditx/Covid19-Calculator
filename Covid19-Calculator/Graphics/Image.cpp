
#include"SDL2/include/SDL_Image.h"

#include"Window.h"

#include"Image.h"
#include"Renderer.h"

Image::Image(std::string _file)
	:
	Size(Vec2(NULL))
{
    Surface = IMG_Load(_file.c_str());
    if (Surface == nullptr)
    {// If file can not load we fall back to a known default Image
        ///ErrorCode = LEVEL_ZERO_ERROR; // Zero means Warning.
        Print("Can not Load Image: " << _file);
        Surface = IMG_Load("Resources/Default.bmp");
        Size = Vec2
        (// Sets the Size to that of the Image file
            Surface->w,
            Surface->h
        );
    }
	Size = Vec2
	(// Sets the Size to that of the Image file
		Surface->w,
		Surface->h
	);
}
Texture::Texture(Image *_image)
{// Creates an SDL_Texture from an Image
    Pixels = SDL_CreateTextureFromSurface(Renderer::get().g_Context(), _image->Surface);
    Size = _image->Size;
}


void Texture::Render(Vec2 _pos, Vec2 _size)
{
	SDL_Rect dest = Renderer::make_Rect
	(
		_pos,//Vec2((AdjustedPos.x + Offset.x), (AdjustedPos.y + Offset.y)),
		Vec2(_size.x, _size.y)
	);
	SDL_Rect SourceRect = { 0,0, (int)Size.x,(int)Size.y };
	SDL_Point center = { (int)(Size.x * 0.5f) , (int)(Size.y * 0.5f) };///{ 0, 0};/// Deal with this Later
	SDL_RenderCopyEx
	(// Render Sprite in position and Orientation as defined by its state and that of the Camera
		Renderer::get().g_Context(),
		Pixels,
		&SourceRect,
		&dest,
		NULL,// Fix angle rotation later
		&center,
		SDL_FLIP_NONE// Deal with flip later
	);
}


void Texture::Render(Vec2 _pos) 
{
//	Vec2 AdjustedPos = (Vec2)Position - (Vec2)Size * .5f;  // Find the Center of the Sprite
//	Vec2 Offset = (iVec2)Camera::get().g_Center(); // Adjust the Render Position According to the Camera Position sent in              

	SDL_Rect dest = Renderer::make_Rect
	(
		_pos,//Vec2((AdjustedPos.x + Offset.x), (AdjustedPos.y + Offset.y)),
		Vec2(Size.x, Size.y)
	);
	SDL_Rect SourceRect = {0,0,(int)Size.x,(int)Size.y};
	SDL_Point center = { (int)(Size.x * 0.5f) , (int)(Size.y * 0.5f) };///{ 0, 0};/// Deal with this Later
	SDL_RenderCopyEx
	(// Render Sprite in position and Orientation as defined by its state and that of the Camera
		Renderer::get().g_Context(),
		Pixels,
		&SourceRect,
		&dest,
		NULL,// Fix angle rotation later
		&center,
		SDL_FLIP_NONE// Deal with flip later
	);
}
void Texture::Render(int _x, int _y) 
{
	Render(Vec2(_x, _y));/// I know but don't feel like fixing it, this is easier
}