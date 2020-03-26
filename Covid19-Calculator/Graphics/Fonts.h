#pragma once

#include<string>
#include"../Common.h"

/*
FONT RENDERER STARTED. NEED TO PLACE ALL THE GIVEN TEXT INTO A STD::VECTOR SO THAT i CAN ASSIGN IT ONCE
AND SPEED UP THE FONT RENDERING PROCESS.
CREATE STRING,
RENDER STRING AND
DESTROY STRING FUNCTIONS ARE NEEDED
ALSO SOME STYLE AND COLOR FUNCTIONALITY
*/


//TODO: So much shit to do here. Perhaps make a loader like I did everything else but even that has become a pain in the ass to manage 
//     even though its extremely easy.




#define OPENING_FONT 0 
// TODO: Perhaps Shadowing should just be a Trait checked before rendering with a boolean or something to prevent having to duplicate code every time
class FontRender
{
public:
	FontRender() = default;
    FontRender(const char*file, int size);

    void FreeFont(TTF_Font *font);
    void CloseFontRender();
    void s_Size(float _size);
    void s_ForgroundColor(float R, float G, float B, float A);
    void s_BackGroundColor(SDL_Color col);
    void s_ForgroundColor(SDL_Color col);

    void Write(float value, Vec2 pos);
    void Write(const char *text, Vec2 pos);
    void WriteFormated(const char *_text, Vec4 _pos_size);
    void WriteFormatedShadow(const char *_text, Vec4 _pos_size, int _depth);

    void WriteShadow(const char *text, Vec2 pos, int depth);
    void WriteShadow(float value, Vec2 pos, int depth);


    void s_Shadowed(bool _state);
    bool g_Shadowed();

    int g_Outline();

	static FontRender *Fonts;

	private:


        TTF_Font *CurrentFont;

        int CurrentFontID;
        float Size;

        SDL_Color
            ForgroundColor,
            BackgroundColor;


		bool Shadowed;
};
extern FontRender *GlobalFont_Renderer;




//SDL_Surface *surface = TTF_RenderText_Solid(CurrentFont, _text, ForgroundColor);

//if (surface == NULL)
//{
//	printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
//}
//else
//{
//	SDL_Texture *mTexture = SDL_CreateTextureFromSurface(Renderer::get().g_Context(), surface);
//	if (mTexture == NULL)
//	{
//		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
//	}
//	else
//	{
//		//Get image dimensions
//		Width = surface->w;
//		Height = surface->h;
//	}
//	SDL_FreeSurface(surface);
			//TTF_Set
			//Set rendering space and render to screen
			//SDL_Rect renderQuad = { _pos_size.x, _pos_size.y, _pos_size.z, _pos_size.w };
			//SDL_RenderCopy(Renderer::get().g_Context(), mTexture, NULL, &renderQuad);
			//SDL_DestroyTexture(mTexture);

//FontRender(const char*file)
//{
//	if (TTF_Init() < 0)
//	{
//		Print("Font Initialization Failed");
//		Print("Error: " << TTF_GetError());
//	}
//
//	CurrentFont = TTF_OpenFont(file, 30);
//
//	if (!CurrentFont)
//	{
//		Print(TTF_GetError);
//	}
//}
