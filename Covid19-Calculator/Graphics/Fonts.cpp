#include"Fonts.h"
#include"Renderer.h"

FontRender *GlobalFont_Renderer;


FontRender::FontRender(const char*file, int size = 30)
{
    if (TTF_Init() < 0)
    {
        Print("Font Initialization Failed");
        Print("Error: " << TTF_GetError());
    }

    CurrentFont = TTF_OpenFont(file, size);

    if (!CurrentFont)
    {
        Print(TTF_GetError);
    }
}


void  FontRender::FreeFont(TTF_Font *font)
{
    TTF_CloseFont(font);
}
void  FontRender::CloseFontRender()
{
    TTF_Quit();
}
void  FontRender::s_Size(float _size)
{
    __debugbreak();
    Size = _size;
}
void  FontRender::s_ForgroundColor(float R, float G, float B, float A)
{
    ForgroundColor.r = R;
    ForgroundColor.g = G;
    ForgroundColor.b = B;
    ForgroundColor.a = A;
}
void FontRender::s_BackGroundColor(SDL_Color col) { BackgroundColor = col; }
void  FontRender::s_ForgroundColor(SDL_Color col) { ForgroundColor = col; }

void  FontRender::Write(float value, Vec2 pos)
{
    std::string Str = std::to_string((int)value);
    const char* S = Str.c_str();

    Write(S, pos);
}

void  FontRender::Write(const char *text, Vec2 pos)
{
    float
        Width = 0,
        Height = 0;

    SDL_Surface *surface = TTF_RenderText_Solid(CurrentFont, text, ForgroundColor);

    if (surface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        SDL_Texture *mTexture = SDL_CreateTextureFromSurface(Renderer::get().g_Context(), surface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            Width = surface->w;
            Height = surface->h;
        }
        SDL_FreeSurface(surface);
        //TTF_Set
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { (int)pos.x, (int)pos.y,  (int)Width,  (int)Height };
        SDL_RenderCopy(Renderer::get().g_Context(), mTexture, NULL, &renderQuad);

        SDL_DestroyTexture(mTexture);
    }
}

void  FontRender::WriteFormated(const char *_text, Vec4 _pos_size)
{
    float
        Width = 0,
        Height = 0;

    SDL_Surface *surface = TTF_RenderText_Solid(CurrentFont, _text, ForgroundColor);

    if (surface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        SDL_Texture *mTexture = SDL_CreateTextureFromSurface(Renderer::get().g_Context(), surface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            Width = surface->w;
            Height = surface->h;
        }
        SDL_FreeSurface(surface);
        //TTF_Set
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { (int)_pos_size.x,  (int)_pos_size.y,  (int)_pos_size.z,  (int)_pos_size.w };
        SDL_RenderCopy(Renderer::get().g_Context(), mTexture, NULL, &renderQuad);

        SDL_DestroyTexture(mTexture);
    }
}
void  FontRender::WriteFormatedShadow(const char *_text, Vec4 _pos_size, int _depth)
{
    float
        Width = 0,
        Height = 0;
    SDL_Color savedcol = ForgroundColor;

    s_ForgroundColor(80, 80, 80, 180);
    WriteFormated(_text, Vec4(_pos_size.x + _depth, _pos_size.y + _depth, _pos_size.z, _pos_size.w));

    s_ForgroundColor(savedcol.r, savedcol.g, savedcol.b, savedcol.a);
    WriteFormated(_text, Vec4(_pos_size.x, _pos_size.y, _pos_size.z, _pos_size.w));
}

void  FontRender::WriteShadow(const char *text, Vec2 pos, int depth)
{
    SDL_Color savedcol = ForgroundColor;

    s_ForgroundColor(80, 80, 80, 180);
    Write(text, Vec2(pos.x + depth, pos.y + depth));

    s_ForgroundColor(savedcol.r, savedcol.g, savedcol.b, savedcol.a);
    Write(text, pos);
}
void  FontRender::WriteShadow(float value, Vec2 pos, int depth)
{
    SDL_Color savedcol = ForgroundColor;

    s_ForgroundColor(75, 75, 75, 255);
    std::string Str = std::to_string((int)value);
    const char* S = Str.c_str();
    Write(S, Vec2(pos.x + depth, pos.y + depth));

    s_ForgroundColor(savedcol.r, savedcol.g, savedcol.b, savedcol.a);
    Str = std::to_string((int)value);
    S = Str.c_str();
    Write(S, pos);
}


void  FontRender::s_Shadowed(bool _state) { Shadowed = _state; }
bool  FontRender::g_Shadowed() { return Shadowed; }

int  FontRender::g_Outline() { return TTF_GetFontOutline(CurrentFont); }



