#include<string>

#include"../Common.h"
#include"Application.h"

#include"Window.h"


#define SHOW_FPS
_static InputDevices *InputDevices::Active;
_static Application *Application::ActiveApp;

///uint32_t ErrorCode = NO_ERROR_LUST;

Application::Application()
	:
	Title(""),
	ApplicationTimer(Timer())
{
    set(ActiveApp);
}
Application::~Application()
{ 
}

//=======================================================================================================
//_______________________________ STATE and FLOW HANDLING _______________________________________________
//=======================================================================================================

#include"SDL2/include/SDL_Image.h"


void Application::End()
{
    OnEnd();
}
void Application::Start()
{
    int Init = SDL_Init(SDL_INIT_EVERYTHING);
    if (Init != NULL)
    {
        Print("Error Initializing SDL:" << Init);
        exit(0x13);
    }
    IMG_Init(IMG_INIT_JPG);

    //-------------------------------------------------------------------------------------------------------------
    OnCreate();
    if (!Window::get().is_Initialized())
    {
        ApplicationWindow = new Window(640, 480, "Default Application Window: Create a User Window");
    }
	ApplicationWindow->s_Position(Vec2(20, 30));
	//InputDevices::get().Mouse.Hide();
}

void Application::Pause()
{}

unsigned int PreviousTime = 0;
unsigned int Accumulator = 0;
unsigned int UpdatePass = 0;


void Application::Update()
{ // Calls User defined Application Update function
    OnUpdate();

	ApplicationTimer.Update();
	++Accumulator;


	unsigned int CurrentTime = SDL_GetTicks();

	DeltaTime = CurrentTime - LastUpdate;
	LastUpdate = CurrentTime;

	unsigned int Difference = (CurrentTime - PreviousTime);

	//while(CurrentTime - UpdatePass < TargetFPS){}
	//if (DeltaTime < TargetFPS)
	//{// Limit Frame Rate to Desired Target
	//	unsigned int delay = TargetFPS - (CurrentTime - UpdatePass);
	//	SDL_Delay(delay * 2);
	//}

	UpdatePass = CurrentTime;

//	Print(Difference);
	if (Difference > 1000) // CurrentTime > (P + 1000))           //
	{// If a Full second passed Display the FPS and reset the Accumulator
		FramesPerSecond = Accumulator;
#ifdef SHOW_FPS
		std::cout << "FPS: " << (FramesPerSecond) << "\n";
#endif	
		PreviousTime = CurrentTime;
		Accumulator = 0;
	}

}
void Application::Render()
{ // Calls User define Application Render function
    OnRender();
    g_Window().UserInterface.Render();
    g_Window().Sync();
    g_Window().CLS(); // Do not like the duel Dereference every single frame Feels unneeded at best
}

void Application::OnEnd()    { Print(" Default "); }
void Application::OnRun()    { Print(" Default "); }
void Application::OnStart()  { Print(" Default "); }
void Application::OnPause()  { Print(" Default "); }
void Application::OnSleep()  { Print(" Default "); }
void Application::OnUpdate() { Print(" Default "); }
void Application::OnRender() { Print(" Default "); }
void Application::OnCreate() { Print(" Default "); }
void Application::OnError(uint32_t _code){ Print(" Default ");}

void Application::MakeWindow(int _width, int _height, std::string _name)
{
    ApplicationWindow = new Window(_width, _height, _name);
}
void Application::ErrorCatch(uint32_t _code)
{
///    switch (_code)
///    {
/// case 0:
//////    Print("**WARNING**");
//////    ErrorCode = NO_ERROR_LUST;
//////    break;
/// case 1:// LEVEL_ONE_ERROR:
//////    Print("***ERROR***");
//////    OnError(_code);
//////    ErrorCode = NO_ERROR_LUST;
//////    break;
/// case 2://LEVEL_TWO_ERROR:
//////    __debugbreak();
//////    ErrorCode = NO_ERROR_LUST;
//////    break;
///    }
	//const char *Err = SDL_GetError();
	//if (Err)
	//{
	//	Print("\n" << SDL_GetError());
	//}
	SDL_ClearError();
}
void Application::ClearError()
{
    ///ErrorCode = NO_ERROR_LUST;
}
uint32_t Application::g_Error()
{
    return 0;///ErrorCode;
}


void Application::Run()
{
	InputDevices::get().Keyboard.Update();
	bool Running = true;
	while (Running)
	{
		SDL_Event msg;
		while (SDL_PollEvent(&msg))
		{
			g_Window().UserInterface.Update(msg);
            Print("Msg: " << msg.type);
			switch (msg.type)
			{
			case SDL_QUIT:
				Running = false;
				break;

			case SDL_MOUSEMOTION:
				InputDevices::get().Mouse.Position = Vec2(msg.motion.x, msg.motion.y);
				InputDevices::get().Mouse.Relative = Vec2(msg.motion.xrel, msg.motion.yrel);
				//	Input.Mouse.Buttons;///Dont need this
				OnMouseMove(Vec2(msg.motion.x, msg.motion.y), Vec2(msg.motion.xrel, msg.motion.yrel), msg.motion.state);  
				break;

			case SDL_MOUSEBUTTONDOWN:
				switch (msg.button.button)
				{
				case SDL_BUTTON_LEFT:
					InputDevices::get().Mouse.Buttons.Left = true;
					break;
				case SDL_BUTTON_MIDDLE:
					InputDevices::get().Mouse.Buttons.Center = true;
					break;
				case SDL_BUTTON_RIGHT:
					InputDevices::get().Mouse.Buttons.Right = true;
					break;
				case SDL_BUTTON_X1:
					InputDevices::get().Mouse.Buttons.X1 = true;
					break;
				case SDL_BUTTON_X2:
					InputDevices::get().Mouse.Buttons.X2 = true;
					break;
				}
				OnMouseButtonDown(Vec2(msg.button.x, msg.button.y), msg.button.button, msg.button.state, msg.button.clicks); 
				break;

			case SDL_MOUSEBUTTONUP: 
		
				switch (msg.button.button)
				{
				case SDL_BUTTON_LEFT:
					InputDevices::get().Mouse.Buttons.Left = false;
					break;
				case SDL_BUTTON_MIDDLE:
					InputDevices::get().Mouse.Buttons.Center = false;
					break;
				case SDL_BUTTON_RIGHT:
					InputDevices::get().Mouse.Buttons.Right = false;
					break;
				case SDL_BUTTON_X1:
					InputDevices::get().Mouse.Buttons.X1 = false;
					break;
				case SDL_BUTTON_X2:
					InputDevices::get().Mouse.Buttons.X2 = false;
					break;
				}

				OnMouseButtonUp(Vec2(msg.button.x, msg.button.y), msg.button.button, msg.button.state); 
				break;

			case SDL_KEYDOWN:
				InputDevices::get().Keyboard.Keys[msg.key.keysym.scancode] = true;

				OnKeyDown(msg.key.keysym.sym, msg.key.keysym.scancode, msg.key.keysym.mod, msg.key.repeat);
				break;

			case SDL_KEYUP:
				InputDevices::get().Keyboard.Keys[msg.key.keysym.scancode] = false;

				OnKeyUp(msg.key.keysym.sym, msg.key.keysym.scancode, msg.key.keysym.mod); 
				break;  
			}

			
		}

		Update();
//		DEBUG_CODE(   ErrorCatch(ErrorCode);)
		Render();
	}
}

void Application::OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState)
{// SDL_MOUSEMOTION
    Print("DEFAULT_MOUSE MOVE");
}
void Application::OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks)
{// SDL_MOUSEBUTTONDOWN
    Print("DEFAULT_MOUSE BUTTON DOWN");
}
void Application::OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state)
{// SDL_MOUSEBUTTONUP
    Print("DEFAULT_MOUSE BUTTON UP");
}
void Application::OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat)
{// SDL_KEYDOWN
    Print("DEFAULT_KEY DOWN");
}
void Application::OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod)
{// SDL_KEYUP
    Print("DEFAULT_KEY UP");
}






//SDL_MOUSEMOTION
//SDL_MOUSEBUTTONDOWN
//SDL_MOUSEBUTTONUP
//SDL_MOUSEWHEEL
//
//SDL_KEYDOWN
//SDL_KEYUP
//
//SDL_AUDIODEVICEADDED
//SDL_AUDIODEVICEREMOVED
//
//SDL_CONTROLLERAXISMOTION
//
//SDL_CONTROLLERBUTTONDOWN
//SDL_CONTROLLERBUTTONUP
//
//SDL_CONTROLLERDEVICEADDED
//SDL_CONTROLLERDEVICEREMOVED
//SDL_CONTROLLERDEVICEREMAPPED
//
//SDL_DOLLARGESTURE
//SDL_DOLLARRECORD
//
//SDL_DROPFILE
//SDL_DROPTEXT
//SDL_DROPBEGIN
//SDL_DROPCOMPLETE
//
//SDL_FINGERMOTION
//SDL_FINGERDOWN
//SDL_FINGERUP
//
//SDL_KEYDOWN
//SDL_KEYUP
//
//SDL_JOYAXISMOTION
//SDL_JOYBALLMOTION
//
//SDL_JOYHATMOTION
//SDL_JOYBUTTONDOWN
//SDL_JOYBUTTONUP
//SDL_JOYDEVICEADDED
//SDL_JOYDEVICEREMOVED
//
//SDL_MOUSEMOTION
//SDL_MOUSEBUTTONDOWN
//SDL_MOUSEBUTTONUP
//SDL_MOUSEWHEEL
//
//SDL_MULTIGESTURE
//
//SDL_QUIT
//
//SDL_SYSWMEVENT
//
//SDL_TEXTEDITING
//
//SDL_TEXTINPUT
//
//SDL_USEREVENT
//
//SDL_WINDOWEVENT