#pragma once

#define NUMBER_OF_KEYS    1024
struct Window;

#include"../Common.h"
#include"Time.h"
#include"Window.h"
#include"GUI.h"

class  EventSystem;
struct InputDevices
{

	//NO_COPY_OR_ASSIGNMENT(InputDevices);
	struct Mouse_state
	{
		struct
		{
			bool Left;
			bool Right;
			bool Center;
			bool X1;
			bool X2;
		}Buttons;

		Vec2 Position;
		Vec2 Relative;

		void Show() { SDL_ShowCursor(SDL_ENABLE); }
		void Hide() { SDL_ShowCursor(SDL_DISABLE); }
		void isVisible() { SDL_ShowCursor(SDL_QUERY); }
	}Mouse;

	struct Keyboard_state
	{
		Keyboard_state()
		{
			Keys = (uint8_t*)SDL_GetKeyboardState(NULL); //new uint8_t[NUMBER_OF_KEYS];
		}
		~Keyboard_state()
		{
			Print("Calling Keyboard_state Destructor");
			//delete(Keys);
		}

		void Update()
		{
			//Keycount = (uint8_t*)SDL_GetKeyboardState(NUMBER_OF_KEYS);
			Print(*Keys);
		}

		uint8_t *Keys;
	private:
		uint32_t Keycount;
	}Keyboard;

	static InputDevices& get() 
	{ 
		static InputDevices *_initialized = new InputDevices();
		return *_initialized;
	}
private:
	InputDevices() { }
static	InputDevices *Active;
};
//const Uint8* SDL_GetKeyboardState(int* numkeys)

class Application
{
public:
    Application();
    ~Application();

    void Run();
    void End();
    void Start();
    void Pause();
    void Update();
    void Render();
    void ErrorCatch(uint32_t _code);
    void MakeWindow(int _width, int _height, std::string _name);

    static uint32_t g_Error(); 
    static void ClearError();

protected:

    virtual void OnRun(); /// Differs from start in that it can resume execution; Possibly change to Resume later or something
    virtual void OnEnd(); 
    virtual void OnStart();
    virtual void OnPause();
    virtual void OnSleep();/// Add this tomorrow
    virtual void OnCreate();
    virtual void OnUpdate();
    virtual void OnRender();

	virtual void OnMouseMove(Vec2 _position, Vec2 _relative, uint32_t _buttonState);   // SDL_MOUSEMOTION/
	virtual void OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks); // SDL_MOUSEBUTTONDOWN
	virtual void OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state); //SDL_MOUSEBUTTONUP
	virtual void OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat); //SDL_KEYDOWN
	virtual void OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod);   //SDL_KEYUP

    virtual void OnError(uint32_t _code); 

	GUI UserInterface;

	float g_Delta() { return DeltaTime / 1000.0f; }
	std::string Title;
	Window ApplicationWindow;
	Timer ApplicationTimer;

	unsigned int LastUpdate = 0.0;
	float DeltaTime = 0.0f;
	float FramesPerSecond;
	float TargetFPS = 16.66f;
	void s_Framerate(float _fps) { TargetFPS = 1000.0f / _fps; }
};
