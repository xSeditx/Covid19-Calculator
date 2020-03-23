
#include"Renderer.h"
#include"Window.h"

Window *Window::MainWindow;

Window::Window(int _w, int _h, std::string _name)
    :
    x(NULL),
    y(NULL),
    width(_w),
    height(_h),
    name(_name),
	Surface(nullptr),
	Initialized(true)
{
    Handle = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _w, _h, SDL_WINDOW_RESIZABLE );//  SDL_WINDOW_BORDERLESS
    Context = new Renderer(Handle, SDL_RENDERER_ACCELERATED); /// Not sure bout this one yet | SDL_RENDERER_TARGETTEXTURE);
    MainWindow = this;
}
void Window::Sync()
{ 
    Context->Display();
}
void Window::CLS()
{
    Context->Clear(); 
}
Window Window::g_Window()
{
    return *MainWindow;
}

void Window::s_Position(Vec2 _position)
{
	SDL_SetWindowPosition(Handle, _position.x, _position.y);
}