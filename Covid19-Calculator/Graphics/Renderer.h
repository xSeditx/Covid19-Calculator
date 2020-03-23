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

    inline SDL_Renderer *g_Context() { return Context; }
    inline Scene& g_Scene() { return *Level; }

    inline static void set(Renderer *_context) { MainContext = _context; }
    inline static Renderer &get() { return *MainContext; }
    inline static bool Initialize(SDL_Window *_win, uint32_t _flags)
    {
        if (MainContext == nullptr) { MainContext = new Renderer(_win, _flags); }
    }

    Scene *Level;
private:
    uint32_t Flags;
    SDL_Renderer *Context;
    static Renderer *MainContext;
};



