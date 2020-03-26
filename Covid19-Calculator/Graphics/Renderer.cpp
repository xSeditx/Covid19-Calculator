#include"Renderer.h"
#include"Window.h"

#include"Image.h"
#include"Sprite.h"
#include"Scene.h"

#include"Camera.h"


Renderer *Renderer::MainContext;

SDL_Rect Renderer::make_Rect(iVec2 _topleft, iVec2 _bottomright)
{
    return 
    {
        _topleft.x,
        _topleft.y,
        _bottomright.x,
        _bottomright.y
    };
}

Renderer::Renderer(SDL_Window *_win, uint32_t _flags)
    :
    Flags(_flags),
	World(nullptr),
	Viewport(nullptr)
{
    Context = SDL_CreateRenderer(_win, -1, _flags);
    Surface = SDL_GetWindowSurface(_win);
    SurfacePixels = SDL_CreateTextureFromSurface(Context,Surface);
    SDL_SetRenderDrawColor(Context, DEFAULT_DRAW_COLOR);
	SDL_SetRenderDrawBlendMode(Context, SDL_BLENDMODE_BLEND);
	set(this);
}
void Renderer::Clear()
{
    SDL_RenderClear(Context);
}
void Renderer::Display()
{
    SDL_RenderPresent(Context);
}

void Renderer::Update(float _deltaTime)
{
    for (auto& L : Groups)
    {
        for (auto &O : L)
        {
            Objects[O]->Update(_deltaTime);
        }
    }
}
void Renderer::Render()
{
    for(auto& L: Groups)
    { 
        for (auto &O : L)
        {
			Objects[O]->Render();
        }
    }
}
void Renderer::Attach(Scene *_scene)
{
    World = std::move(_scene);
}
idTag Renderer::Add(Sprite *_sprite, uint8_t _layer)
{
    if (Groups.size() < ((size_t)_layer + 1))
    {
		size_t count = ((size_t)_layer + 1) - Groups.size();
		for_loop (i, count)
		{
			Groups.push_back(std::vector<idTag>());
		}
    }
    idTag results = Objects.size();
    Groups[_layer].push_back(results);// Objects.size() is the _Sprites new idTag as now Groups[_layer].back() value points to the index of the Sprite
    Objects.push_back(_sprite);
    return results;
}



/* Sets the Main Renderer */
_static void Renderer::set(Renderer *_context) { MainContext = _context; }
/* Gets the Main Renderer */
_static Renderer &Renderer::get() { return *MainContext; }
/* Test to see if a Renderer has been Created and set */
_static bool Renderer::is_Initialize()
{
    return MainContext != nullptr;
}
/* Creates and Assigns the main renderer*/
_static bool Renderer::Initialize(SDL_Window *_win, uint32_t _flags)
{
    if (!MainContext)
    { 
        MainContext = new Renderer(_win, _flags);
        return true;
    }
    return false;
}

