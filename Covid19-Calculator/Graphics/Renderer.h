#pragma once
#include<vector>
#include"../Common.h"
#include "Window.h"

#include"Sprite.h"


class Scene;
class Camera;

class Renderer
{
public:
    Renderer(SDL_Window *_win, uint32_t _flags);

    void Clear();
    void Display();
    static SDL_Rect make_Rect(iVec2 _topleft, iVec2 _size);

    Scene *World;
    Camera *Viewport;
    SDL_Surface *Surface;
    SDL_Texture *SurfacePixels;

    /* DOD Based Design */
    std::vector<std::vector<idTag>> Groups;
    std::vector<Sprite *> Objects;

    idTag Add(Sprite *_sprite, uint8_t _layer);

    void Attach(Scene *_scene);
    void Update(float _deltaTime);

    void Render();

    /* Gets the SDL Surface for the Renderer */
    SDL_Surface* g_Surface() { return Surface; };
    /* Getes the SDL Rendering context for the Renderer */
    SDL_Renderer *g_Context() { return Context; }


    /* Sets the Main Renderer */
    static void set(Renderer *_context);
    /* Gets the Main Renderer */
    static Renderer &get();
    /* Test to see if a Renderer has been Created and set */
    static bool is_Initialize();  
    /* Creates and Assigns the main renderer */
    static bool Initialize(SDL_Window *_win, uint32_t _flags);
    /* Returns the a pointer to the Entire Scene graph for the Level */
    Scene& g_Scene() { return *Level; }

private:
    Scene *Level;
    uint32_t Flags;
    SDL_Renderer *Context;
    static Renderer *MainContext;
};


/// LIKELY IN THE NEAR FUTURE
/*


class Surface
{
    Surface(SDL_Window *_window)
        :
        Target(_window)
    {
        DrawableSurface = SDL_GetWindowSurface(Target);
        Texture = SDL_CreateTextureFromSurface(Context, SurfaceTexture);
    }

    SDL_Surface *DrawableSurface;
    SDL_Texture *SurfacePixels;
    SDL_Window* Target;

    SDL_Surface& g_Surface() { return DrawableSurface; }
    SDL_Texture& g_Texture() { return SurfacePixels}
};



*/