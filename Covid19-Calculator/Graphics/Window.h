#pragma once
#include<string>

#include"../Common.h"
#include"GUI.h"

using Event = SDL_Event;
class Renderer;


struct Window
{
    Window(const Window&) = delete;
    Window& operator =(const Window&) = delete;
    
    Window() = default;
    Window(int _w, int _h, std::string _name);

    uint32_t Height() { return height; }
    uint32_t Width() { return width; }
    std::string Title() { return name; }
    SDL_Window& g_Handle() { return *Handle; }
    SDL_Surface& g_Surface() { return  *Surface; }
    Renderer& g_Context() { return *Context; }

    Vec2 Position() { return { x,y }; }
    Vec2 size() { return { width, height }; }

    void Sync();
    void CLS();

	void s_Position(Vec2 _position);

    inline static Window& get()
    {
        return *mainWindow;
    }
    static void set(Window* _window)
    {
        mainWindow = _window;
    }

    bool is_Initialized() {    return Initialized;    }


    GUI UserInterface;
private:

    bool Initialized{ false };

    SDL_Window *Handle;
    Renderer *Context;

    int x, y, width, height;
    std::string name;
    SDL_Surface *Surface;

    static Window *mainWindow;
}; 











/* =======================================================================================================================
                                                        TRASH
   =======================================================================================================================







//#pragma comment(lib, "SDL2-2.0.12/lib/x64/SDL2_Image.lib")
//#pragma comment(lib, "SDL2-2.0.12/lib/x64/SDL2_ttf.lib")
//#pragma comment(lib, "SDL2-2.0.12/lib/x64/SDL2_mixer.lib")

   ======================================================================================================================= */