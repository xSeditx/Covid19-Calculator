#pragma once
#include<string>
//#include"SDL_defines.h"
#include"../Common.h"

using Event = SDL_Event;
class Renderer;


struct Window
{
	Window() = default;
    Window(int _w, int _h, std::string _name);

    int x, y, width, height;
    std::string name;

    Renderer *Context;
    SDL_Window *Handle;
    SDL_Surface *Surface;

    void Sync();
    void CLS();
    static Window g_Window();

	bool Initialized = false;

	void s_Position(Vec2 _position);
private:
    static Window *MainWindow;
}; 











/* =======================================================================================================================
                                                        TRASH
   =======================================================================================================================







//#pragma comment(lib, "SDL2-2.0.12/lib/x64/SDL2_Image.lib")
//#pragma comment(lib, "SDL2-2.0.12/lib/x64/SDL2_ttf.lib")
//#pragma comment(lib, "SDL2-2.0.12/lib/x64/SDL2_mixer.lib")

   ======================================================================================================================= */