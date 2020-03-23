#pragma once
#include"../Common.h"
#include"Gameobject.h"

#include"Fonts.h"



struct Texture;


/*
 When rendering a control it's position is renderered relative to parent with alignment determining which part of the parent it is relative to.
================================================================================================================================================
    Center = X,Y of parent
    Left = X Position of parent
    Right = Position of Parent + Width of parent
    Top = Y Position of Parent
    Bottom = Y Position + Height of Parent
*/



enum Alignment
{
	Align_Top = BIT(0),
	Align_Left = BIT(1),
	Align_Right = BIT(2),
	Align_Center = BIT(3),
	Align_Bottom = BIT(4)
};
enum boarder_type
{
	LeftWindowBoarder = BIT(0),
	TopWindowBoarder = BIT(1),
	BottomWindowBoarder = BIT(2),
	RightWindowBoarder = BIT(3)
};
enum WidgetStyle
{
	Borderless = BIT(0),
	Resizable = BIT(1),
	Anchored = BIT(2)
};


class Widget_base
{
protected:
	//Widget_base() = default;
public:
	Alignment Align;
	struct _area
	{
		Vec2 Position;// Determined by Alignment
		Vec2 Size;	
		Vec2 RelativePosition;

		operator SDL_Rect() 
		{
			return
			{
				(int)Position.x,
				(int)Position.y,
				(int)Size.x,
				(int)Size.y
			};
		}
		bool isOver(int _x, int _y);
	}Area;

	Texture& g_Skin();
	Widget_base& g_Parent();

	void s_Skin(Texture *_skin);
	void s_Parent(Widget_base *_parent);


	Vec2 Topleft();
	Vec2 Bottomleft();
	Vec2 Topright();
	Vec2 Bottomright();

	float g_Width();
	float g_Height();

	void make_Child(Widget_base *_child);

	virtual void OnEnd();
	virtual void OnCreate();
	virtual void OnUpdate(SDL_Event &_msg);
	virtual void OnRender();
	virtual void OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState);   // SDL_MOUSEMOTION/
	virtual void OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks); // SDL_MOUSEBUTTONDOWN
	virtual void OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state); //SDL_MOUSEBUTTONUP
	virtual void OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat); //SDL_KEYDOWN
	virtual void OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod);   //SDL_KEYUP
	virtual void Update(SDL_Event &_msg);
	virtual void Render();

	bool Dragable = false;
	bool isChild() { return (Parent != nullptr); }
	bool isMouseOver();
	bool isClicked() { return Clicked; }
	void Select();// Sets clicked
	void Unselect();// Resets clicked

	std::vector<Widget_base *> Children;
protected:
	Widget_base(Vec2 _pos, Vec2 _size);

	uint32_t Style;// WidgetStyles ORed together

	Widget_base *Parent;
	Texture *Skin;
	bool Clicked = false;


	virtual void End();
	virtual void Create();

	virtual void MouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState);   // SDL_MOUSEMOTION/
	virtual void MouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks); // SDL_MOUSEBUTTONDOWN
	virtual void MouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state); //SDL_MOUSEBUTTONUP
	virtual void KeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat); //SDL_KEYDOWN
	virtual void KeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod);   //SDL_KEYUP

	static FontRender Fonts;
	static SDL_Renderer *Context;

};
class Window_Widget
	:
	public Widget_base
{
	//Window_Widget() = default;

public:
	Window_Widget(Window_Widget *_other);
	Window_Widget(Vec2 _pos, Vec2 _size);
	void Render() override;

	void OnUpdate(SDL_Event &_msg) override;

	void s_Text(std::string _text);
	std::string Text;


	SDL_Rect WindowTop;
	SDL_Rect Sides;
	SDL_Rect TopLeft, TopRight, BottomLeft, BottomRight;
	SDL_Rect CloseX, CloseXPressed, MinButton, MinButtonPressed, Menu;
	SDL_Rect TopBoarder, LeftBoarder, RightBoarder, BottomBoarder;
	SDL_Rect BottomLeftCorner, BottomRightCorner, TopLeftCorner, TopRightCorner;
	bool Dragging = false;
};

class Button
	:
	public Widget_base
{
public:
	Button(Vec2 _position, Vec2 _size);
	void Render() override;

	std::string Text;
	void s_Text(std::string _text);
};
class MenuButton
	:
	public Button
{
public:
	MenuButton(Vec2 _position, Vec2 _size);
	void Render() override;

	SDL_Rect Menu = { 32,0,24,24 };
};
class MinmizeButton
	:
	public Button
{
public:
	MinmizeButton(Vec2 _position, Vec2 _size);
	void Render() override;
	//void Update(SDL_Event& _msg) override;
	SDL_Rect MinButton = { 144, 0, 24, 24 };
	SDL_Rect MinButtonPressed = { 170,0,24,24 };
};
class CloseXButton
	:
	public Button
{
public:
	CloseXButton(Vec2 _position, Vec2 _size);
	void Render() override;
	SDL_Rect CloseX = { 92,0,24,24 };
	SDL_Rect CloseXPressed = { 118, 0, 24, 24 };
};
class TextInputBox
	:
	public Widget_base
{
	std::vector<char> TextInput;
};
class TextBox
	:
	public Widget_base
{

};
class GUIFrame
	:
	public Widget_base
{
};
class HScrollbar
	:
	public Widget_base
{
	int Value;
};
class VScrollbar
	:
	public Widget_base
{
	int Value;
};
class PictureBox
	:
	public Widget_base
{
	Texture *Picture;
};
class MenuBar
	:
	public Widget_base
{
public:
	MenuBar(Window_Widget *_parent);

	void OnUpdate(SDL_Event &_msg) override;
	void Render() override;

	SDL_Rect WindowTop = { 56,0, 23 , 23 };
};
class Boarder
	:
	public Widget_base
{
public:
	Boarder(boarder_type _type, Window_Widget *_parent);
	void Render() override;
	void OnUpdate(SDL_Event &_msg) override;
	void Update(SDL_Event &_msg) override;
	boarder_type BoarderType;
	SDL_Rect DrawRect; 
	bool Dragging;
};
class TestButton
	:
	public Button
{
public:
	TestButton(Vec2 _pos, Vec2 _size);

	virtual void OnUpdate(SDL_Event& _msg) override;
	virtual void OnRender() override;
	virtual void OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState) override;   // SDL_MOUSEMOTION/
	virtual void OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks) override; // SDL_MOUSEBUTTONDOWN
	virtual void OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state) override; //SDL_MOUSEBUTTONUP
	virtual void OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat) override; //SDL_KEYDOWN
	virtual void OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod) override;   //SDL_KEYUP
};
class GUI
{
public:
	GUI() = default;
	GUI(std::string _name);
	void Update(SDL_Event _msg);
	void Render();
	void Add(Widget_base *_widget);

	std::string Name;
	std::vector<Widget_base*> Controls;

	idTag make_Control(Widget_base *_widget);

	static GUI& get() { return *Active; }
private:
	static GUI* Active;
};



///=================================================================================================================================================
///              TRASH
///=================================================================================================================================================


	//for (auto &Control : Controls)
	//{
	//	if (Control->isMouseOver())
	//	{ // If Mouse is over out control, Pass the messages to it, else, pass back to the calling Widget
	//		for (auto& Child : Control->Children)
	//		{// If their are Children, Pass the Message further up
	//			Child->Area.Position = Child->Area.RelativePosition + Control->Area.Position;
	//			Child->Update(_msg);
	//			Child->OnUpdate(_msg);
	//		}
	//
	//		switch (_msg.type)
	//		{// If the Message is handled by the Child Widget then Null the message or Pass a new message to the Parent
	//		case SDL_MOUSEMOTION:
	//			if (Control->Dragable _EQUALS_ true)
	//			{
	//				if (Control->isClicked() _EQUALS_ true)
	//				{
	//					Control->Area.Position.x += _msg.motion.xrel;
	//					Control->Area.Position.y += _msg.motion.yrel;
	//				}
	//			}
	//			Control->OnMouseMove(Vec2(_msg.motion.x, _msg.motion.y), Vec2(_msg.motion.xrel, _msg.motion.yrel), _msg.motion.state);
	//			break;
	//
	//		case SDL_MOUSEBUTTONDOWN:
	//			Control->Select();
	//			Control->OnMouseButtonDown(Vec2(_msg.button.x, _msg.button.y), _msg.button.button, _msg.button.state, _msg.button.clicks);
	//			break;
	//
	//		case SDL_MOUSEBUTTONUP:
	//			Control->Unselect();
	//			Control->OnMouseButtonUp(Vec2(_msg.button.x, _msg.button.y), _msg.button.button, _msg.button.state);
	//			break;
	//
	//		case SDL_KEYDOWN:/// Maybe make it clicked if key like space or enter is pressed???
	//			Control->OnKeyDown(_msg.key.keysym.sym, _msg.key.keysym.scancode, _msg.key.keysym.mod, _msg.key.repeat);
	//			break;
	//
	//		case SDL_KEYUP:
	//			Control->OnKeyUp(_msg.key.keysym.sym, _msg.key.keysym.scancode, _msg.key.keysym.mod);
	//			break;
	//		}
	//		Control->OnUpdate(_msg);
	//		_msg.type = NULL;
	//	}
	//}



//{/// TURNED THEM ALL INTO THEIR OWN WIDGET AS THEY ALL HAVE SPECIFIC ACTIONS NEED TO BE TAKING
// Draw All the Window Widget piece by piece. Top, Sizes, Corners 
// Renders the Underlaying Grey Rect for the Window

//SDL_Rect dest = { Area.Position.x, Area.Position.y,  Area.Size.x, WindowTop.h };
//SDL_Point center = { 0, 0 };
//SDL_RenderCopyEx
//(// Render Top bar
//	Context,
//	Skin->Pixels,
//	&WindowTop,
//	&dest,
//	0,
//	&center,
//	SDL_FLIP_NONE
//);
//dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y + WindowTop.h }, { 7, Area.Size.y - WindowTop.h });
//SDL_RenderCopyEx
//(// Render the Left Hand Boarder 
//	Context,
//	Skin->Pixels,
//	&LeftBoarder,
//	&dest,
//	0,
//	&center,
//	SDL_FLIP_NONE
//);
//dest = Renderer::make_Rect({ Area.Position.x + 5, Area.Position.y + Area.Size.y }, { Area.Size.x - 10, 7 });
//SDL_RenderCopyEx
//(// // Render the Bottom Boarder
//	Context,
//	Skin->Pixels,
//	&BottomBoarder,
//	&dest,
//	0,
//	&center,
//	SDL_FLIP_NONE
//);
//  dest = Renderer::make_Rect({ Area.Position.x + Area.Size.x - RightBoarder.w, Area.Position.y + WindowTop.h }, { 7, Area.Size.y - WindowTop.h - 4 });
//  SDL_RenderCopyEx
//  (//// Render the Right Hand Boarder 
//  	Context,
//  	Skin->Pixels,
//  	&RightBoarder,
//  	&dest,
//  	0,
//  	&center,
//  	SDL_FLIP_NONE
//  );
//  dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y + Area.Size.y - 5 }, { BottomLeftCorner.w, BottomLeftCorner.h });
//  SDL_RenderCopyEx
//  (//// Render the Left Corner
//  	Context,
//  	Skin->Pixels,
//  	&BottomLeftCorner,
//  	&dest,
//  	0,
//  	&center,
//  	SDL_FLIP_NONE
//  );
//  dest = Renderer::make_Rect({ Area.Position.x + Area.Size.x - BottomLeftCorner.w + 1, Area.Position.y + Area.Size.y - 4 }, { BottomLeftCorner.w, BottomLeftCorner.h });
//  SDL_RenderCopyEx
//  (//// Render the Bottom Right
//  	Context,
//  	Skin->Pixels,
//  	&BottomRightCorner,
//  	&dest,
//  	0,
//  	&center,
//  	SDL_FLIP_NONE
//  );
//} // End of Draw Scope
//	if (_msg.type == SDL_MOUSEBUTTONDOWN)
//	{
//		Dragging = true;
//	}
//	if (_msg.type == SDL_MOUSEBUTTONUP)
//	{
//		Dragging = false;
//	}
//
//	if (_msg.type == SDL_MOUSEMOTION)
//	{
//		if (Dragging == true)
//		{
//			Area.Position.x += _msg.motion.xrel;
//			Area.Position.y += _msg.motion.yrel;
//		}
//	}
//	TODO("Make something to alter or change the message going to the child depending on whether or not the parent handled the message");
//	for (auto &C : Children)
//	{
//		//The Is Over was not really designed to handle this, also to avoid passing a vec to to every child function, 
//		//	possible even the _msg we should likely store the mouse Position adn teh message in the GUI function and allow all the children to have access to it... but how.
//		//	How can I have all my children have access to a single parent SINGLE parent.
////if (C->Area.isOver(_pos.x, _pos.y))
////{
////	C->OnUpdate(_msg);
////}
//	}
//
//bool CursorVis = SDL_ShowCursor(SDL_QUERY);
//if (C->Area.isOver(_pos.x, _pos.y))
//{
//	if (SDL_ShowCursor(SDL_QUERY) _EQUALS_ SDL_DISABLE)
//	{
//		SDL_ShowCursor(SDL_ENABLE);
//	}
//	C->Update(_msg);
//}
//else
//{
//	TODO("GUI::UPDATE : NEED TO GET THE DEFAULT STATE OF THE MOUSE AND USE THAT HERE");
//	SDL_ShowCursor(SDL_DISABLE);
//}
//
//switch (_type)
//{
//case LeftWindowBoarder:
//{
//	DrawRect =
//}break;
//
//case TopWindowBoarder:
//{
//	DrawRect =
//}break;
//
//case BottomWindowBoarder:
//{
//	DrawRect =
//}break;
//
//case RightWindowBoarder:
//{
//	DrawRect =
//}break;
//







//
//	boarder_type Type;
//
//	SDL_Rect TopBoarder = { 15,0,7,7 };
//	SDL_Rect LeftBoarder = { 8,7,7,7 };
//	SDL_Rect RightBoarder = { 23,7,7,7 };
//	SDL_Rect BottomBoarder = { 15,16,7,7 };
//};






//dest = Renderer::make_Rect({ Area.Position.x + Area.Size.x - 5, Area.Position.y }, { TopRightCorner.w, TopRightCorner.h });
//SDL_RenderCopyEx
//(// Render the top right
//	Context,
//	Skin->Pixels,
//	&TopRightCorner,
//	&dest,
//	0,
//	&center,
//	SDL_FLIP_NONE
//);
//
//dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y }, { TopRightCorner.w, TopRightCorner.h });
//SDL_RenderCopyEx
//(// Render the top Left corner
//	Context,
//	Skin->Pixels,
//	&TopLeftCorner,
//	&dest,
//	0,
//	&center,
//	SDL_FLIP_NONE
//);





///     LBoarder::LBoarder(Window_Widget *_win)// Vec2 _pos, Vec2 _size)
///     	:
///     	Widget_base({0,0}, { 5 ,_win->Area.Size.y })
///     {
///     	Area = _win->Area;
///     	Area.Size.x = 5;
///     }
///     void LBoarder::OnUpdate(SDL_Event _msg) {}
///     void LBoarder::Render() {
///     	SDL_Renderer *Context = Renderer::get().g_Context();
///     	SDL_Rect dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y + WindowTop.h }, { 7, Area.Size.y - WindowTop.h });
///     	SDL_Point center = { 0,0 };
///     	SDL_RenderCopyEx
///     	(// Render the Left Hand Boarder 
///     		Context,
///     		Skin->Pixels,
///     		&LeftBoarder,
///     		&dest,
///     		0,
///     		&center,
///     		SDL_FLIP_NONE
///     	);
///     }
///     
///     RBoarder::RBoarder(Window_Widget *_win) :
///     Widget_base({_win->Area.Size.x - 5, 0}, { 5 ,_win->Area.Size.y }) 
///     {}//Vec2 _pos, Vec2 _size) {}
///     void RBoarder::OnUpdate(SDL_Event _msg) {}
///     void RBoarder::Render() 
///     {
///     	SDL_Renderer *Context = Renderer::get().g_Context();
///     	SDL_Point center = { 0,0 };
///     	SDL_Rect dest = Renderer::make_Rect({ Area.Position.x + Area.Size.x - RightBoarder.w, Area.Position.y + WindowTop.h }, { 7, Area.Size.y - WindowTop.h - 4 });
///     	SDL_RenderCopyEx
///     	(//// Render the Right Hand Boarder 
///     		Context,
///     		Skin->Pixels,
///     		&RightBoarder,
///     		&dest,
///     		0,
///     		&center,
///     		SDL_FLIP_NONE
///     	);
///     }
///     
///     TBoarder::TBoarder(Window_Widget *_win) :
///     	Widget_base({0,0}, { _win->Area.Size.x, 5 })
///     {}//Vec2 _pos, Vec2 _size) {}
///     void TBoarder::OnUpdate(SDL_Event _msg) {}
///     void TBoarder::Render() 
///     {
///     }
///     
///     BBoarder::BBoarder(Window_Widget *_win) :
///     	Widget_base({0, _win->Area.Size.y }, { _win->Area.Size.x, 5 })
///     {
///     
///     }//Vec2 _pos, Vec2 _size) {}
///     
///     void BBoarder::OnUpdate(SDL_Event _msg) {}
///     
///     void BBoarder::Render()
///     {
///     	SDL_Renderer *Context = Renderer::get().g_Context();
///     	SDL_Point center = { 0,0 };
///     	SDL_Rect dest = Renderer::make_Rect({ Area.Position.x + 5, Area.Position.y + Area.Size.y }, { Area.Size.x - 10, 7 });
///     	SDL_RenderCopyEx
///     	(// // Render the Bottom Boarder
///     		Context,
///     		Skin->Pixels,
///     		&BottomBoarder,
///     		&dest,
///     		0,
///     		&center,
///     		SDL_FLIP_NONE
///     	);
///     }



//_area(Vec2 _pos, Vec2 _size)
//	:
//	Position(_pos), Size(_size)
//{}




//switch (msg.button.button)
//{
//case SDL_BUTTON_LEFT:
//	Input.Mouse.Buttons.Left = true;
//	break;
//case SDL_BUTTON_MIDDLE:
//	Input.Mouse.Buttons.Center = true;
//	break;
//case SDL_BUTTON_RIGHT:
//	Input.Mouse.Buttons.Right = true;
//	break;
//case SDL_BUTTON_X1:
//	Input.Mouse.Buttons.X1 = true;
//	break;
//case SDL_BUTTON_X2:
//	Input.Mouse.Buttons.X2 = true;
//	break;
//}//
//switch (msg.button.button)
//{
//case SDL_BUTTON_LEFT:
//	Input.Mouse.Buttons.Left = false;
//	break;
//case SDL_BUTTON_MIDDLE:
//	Input.Mouse.Buttons.Center = false;
//	break;
//case SDL_BUTTON_RIGHT:
//	Input.Mouse.Buttons.Right = false;
//	break;
//case SDL_BUTTON_X1:
//	Input.Mouse.Buttons.X1 = false;
//	break;
//case SDL_BUTTON_X2:
//	Input.Mouse.Buttons.X2 = false;
//	break;
//}		Input.Mouse.Position = Vec2(msg.motion.x, msg.motion.y);
//Input.Mouse.Relative = Vec2(msg.motion.xrel, msg.motion.yrel);
//	Input.Mouse.Buttons;///Dont need this//		Input.Keyboard.Keys[msg.key.keysym.scancode] = true;









///class LBoarder
///	:
///	public Widget_base
///{
///
///	LBoarder(Window_Widget *_win);// Vec2 _pos, Vec2 _size);
///	void OnUpdate(SDL_Event _msg);
///	void Render();
///	Window_Widget *Win;
///
///	SDL_Rect LeftBoarder = { 8, 7, 7, 7 };
///};
///class RBoarder 
///	:
///	public Widget_base
///{
///	RBoarder(Window_Widget *_win);// Vec2 _pos, Vec2 _size);
///	void OnUpdate(SDL_Event _msg);
///	void Render();
///	Window_Widget *Win;
///	SDL_Rect RightBoarder = { 23,7,7,7 };
///};
///class TBoarder 
///	:
///	public Widget_base
///{
///	TBoarder(Window_Widget *_win);// Vec2 _pos, Vec2 _size);
///	void OnUpdate(SDL_Event _msg);
///	void Render();
///	Window_Widget *Win;
///	SDL_Rect TopBoarder = { 15,0,7,7 };
///};
///class BBoarder 
///	:
///	public Widget_base
///{
///	BBoarder(Window_Widget *_win);// Vec2 _pos, Vec2 _size);
///	void OnUpdate(SDL_Event _msg);
///	void Render();
///	Window_Widget *Win;
///	SDL_Rect BottomBoarder = { 15,16,7,7 };
///};

