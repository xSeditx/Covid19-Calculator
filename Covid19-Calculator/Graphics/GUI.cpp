#include"GUI.h"
#include"Renderer.h"
#include"Application.h"
#include"Image.h"

_static SDL_Renderer *Widget_base::Context; /// AGAIN CONTEXT NEEDS TO BE IN THE GUI CLASS
_static FontRender Widget_base::Fonts;
_static GUI* GUI::Active;



Widget_base::Widget_base(Vec2 _pos, Vec2 _size)
	:
	Area({ _pos, _size })
{
	//	Area = { _pos.x, _pos.y, _size.x, _size.y };
	static bool isInit = false;
	if (isInit == false)
	{
		SDL_ShowCursor(SDL_ENABLE);
		isInit = true;
		Fonts = FontRender(std::string("Resources/Prestige.ttf").c_str(), 25);
		Context = Renderer::get().g_Context();/// AGAIN CONTEXT NEEDS TO BE IN THE GUI CLASS
		assert(Context != nullptr);
	}
}

//Currently the MouseOver is not working on the Boarder because I have the Boarder setup funky
//int the way it handles the mouse Over events so it detects the mouse being over it when it is 
//far away from it s normal spio

void Widget_base::End()
{
	OnEnd();
}
void Widget_base::Create()
{
	OnCreate();
}
void Widget_base::MouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState)
{
	OnMouseMove(_position, _relative, _buttonState);
}
void Widget_base::MouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks)
{
 	OnMouseButtonDown( _positon,  _button,  _state,  _clicks);
}
void Widget_base::MouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state)
{
	OnMouseButtonUp(_positon, _button, _state);
}
void Widget_base::KeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat)
{
	OnKeyDown(_keycode, _scancode, _mod, _repeat);
}
void Widget_base::KeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod)
{
	OnKeyUp(_keycode, _scancode, _mod);
}
void Widget_base::OnEnd() {}
void Widget_base::OnCreate(){}
void Widget_base::OnUpdate(SDL_Event &_msg){}
void Widget_base::OnRender(){}
void Widget_base::OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState){  /* SDL_MOUSEMOTION*/} 
void Widget_base::OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks) { /* SDL_MOUSEBUTTONDOWN*/}
void Widget_base::OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state) {/*SDL_MOUSEBUTTONUP*/} 
void Widget_base::OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat) {/*SDL_KEYDOWN*/} 
void Widget_base::OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod) { /*SDL_KEYUP*/}  
void Widget_base::s_Skin(Texture *_skin)
{
	Skin = _skin;
}
void Widget_base::Select()
{
	Clicked = true;
}
void Widget_base::Unselect()
{
	Clicked = false;
}
Vec2 Widget_base::Topleft() 
{
	return Area.Position;
}
Vec2 Widget_base::Bottomleft()
{
	return { Area.Position.x, Area.Position.y + Area.Size.y };
}
Vec2 Widget_base::Topright() 
{
	return  { Area.Position.x + Area.Size.x, Area.Position.y  };
}
Vec2 Widget_base::Bottomright()
{
	return  { Area.Position.x + Area.Size.x, Area.Position.y + Area.Size.y };
}
float Widget_base::g_Width()
{// Return the Area Width of the Widget
	return Area.Size.x; 
}
float Widget_base::g_Height() 
{// Return the Area Height of the Widget
	return Area.Size.y;
}


Texture& Widget_base::g_Skin()
{
	return *Skin;
}
bool Widget_base::isMouseOver() 
{ 
	return Area.isOver(InputDevices::get().Mouse.Position.x, InputDevices::get().Mouse.Position.y);
};
bool Widget_base::_area::isOver(int _x, int _y)
{
	bool results = (
		_x > Position.x &&
		_y > Position.y &&
		_x < (Position.x + Size.x) &&
		_y < (Position.y + Size.y)
		);
	return results;
}

void Widget_base::make_Child(Widget_base *_child)
{
	_child->s_Parent(this);
	assert(Skin); _child->s_Skin(Skin); // Had an If here but we really do not want to create a child without the Parent Widget being fully initialized
	_child->Area.RelativePosition = _child->Area.Position;
	_child->Area.Position = Area.Position + _child->Area.RelativePosition;
	Children.push_back(_child);
}
void Widget_base::s_Parent(Widget_base *_parent) 
{
	Parent = _parent;
}
Widget_base&  Widget_base::g_Parent()
{
	return *Parent;
}


void Widget_base::Update(SDL_Event &_msg)
{
    for (auto& C : Children)
    {// If their are Children, Pass the Message further up
		C->Area.Position = C->Area.RelativePosition + Area.Position;
    	C->Update(_msg);    
    }

	OnUpdate(_msg);


	if (isMouseOver())
	{ // If Mouse is over out control, Pass the messages to it, else, pass back to the calling Widget
		switch (_msg.type)
		{// If the Message is handled by the Child Widget then Null the message or Pass a new message to the Parent
		case SDL_MOUSEMOTION:
			if (Dragable)
			{
				if (isClicked())
				{
					Area.Position.x += _msg.motion.xrel;
					Area.Position.y += _msg.motion.yrel;
				}
			}
			OnMouseMove(Vec2(_msg.motion.x, _msg.motion.y), Vec2(_msg.motion.xrel, _msg.motion.yrel), _msg.motion.state);
			break;

		case SDL_MOUSEBUTTONDOWN:
			Select();
			OnMouseButtonDown(Vec2(_msg.button.x, _msg.button.y), _msg.button.button, _msg.button.state, _msg.button.clicks);
			break;

		case SDL_MOUSEBUTTONUP:
			Unselect();
			OnMouseButtonUp(Vec2(_msg.button.x, _msg.button.y), _msg.button.button, _msg.button.state);
			break;

		case SDL_KEYDOWN: /// Maybe make it clicked if key like space or enter is pressed???
			OnKeyDown(_msg.key.keysym.sym, _msg.key.keysym.scancode, _msg.key.keysym.mod, _msg.key.repeat);
			break;

		case SDL_KEYUP:
			OnKeyUp(_msg.key.keysym.sym, _msg.key.keysym.scancode, _msg.key.keysym.mod);
			break;
		}
		_msg.type = NULL;
	}
}
void Widget_base::Render()
{
	for (auto& C : Children)
	{	
		C->Render();
	}
	OnRender(); 
	//Render();
}

///=================================================================================================================================================
///              BUTTON
///=================================================================================================================================================


Button::Button(Vec2 _position, Vec2 _size)
	:
	Widget_base(_position, _size)
{}
void Button::Render()
{
	SDL_Rect R =
	{
		 (int)Area.Position.x,
		 (int)Area.Position.y,
		 (int)Area.Size.x,
		 (int)Area.Size.y
	};
	SDL_Rect T = Area;

	float Scale = 1;
	Vec2 TL = {Area.Position.x / Scale, Area.Position.y / Scale };
	Vec2 BottomRight = { (TL.x + R.w) / Scale, (TL.y + R.h) / Scale };

	if(isClicked())
	{

		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 100, 100, 100, 255);
		SDL_RenderFillRect(Renderer::get().g_Context(), &R);
		SDL_RenderDrawRect(Context, &R);

		SDL_RenderSetScale(Context, Scale, Scale);

		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 50, 50, 50, 255);
		SDL_RenderDrawLine(Context, TL.x, TL.y , BottomRight.x , TL.y);
		SDL_RenderDrawLine(Context, TL.x , TL.y , TL.x , BottomRight.y);
		
		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 150, 150, 150, 255);
		SDL_RenderDrawLine(Context, TL.x , BottomRight.y , BottomRight.x , BottomRight.y);
		SDL_RenderDrawLine(Context, BottomRight.x , BottomRight.y, BottomRight.x, TL.y );

		Fonts.WriteFormatedShadow(Text.c_str(), Vec4(Area.Position.x + 8.0f, Area.Position.y + 8.0f, Area.Size.x - 8.0, Area.Size.y - 8.0),4);
	}
	else
	{
		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 100, 100, 100, 255);
		SDL_RenderFillRect(Renderer::get().g_Context(), &R);

		SDL_RenderSetScale(Context, Scale, Scale);

		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 150, 150, 150, 255);
    	SDL_RenderDrawLine(Context, TL.x, TL.y, BottomRight.x, TL.y);
		SDL_RenderDrawLine(Context, TL.x, TL.y, TL.x, BottomRight.y);
		SDL_SetRenderDrawColor(Renderer::get().g_Context(), 50, 50, 50, 255);

		SDL_RenderDrawLine(Context, TL.x, BottomRight.y, BottomRight.x, BottomRight.y);
		SDL_RenderDrawLine(Context, BottomRight.x, BottomRight.y, BottomRight.x, TL.y);
		Fonts.WriteFormated(Text.c_str(), Vec4(Area.Position.x + 8.0f, Area.Position.y + 8.0f, Area.Size.x - 8.0, Area.Size.y - 8.0));
	}

	SDL_RenderSetScale(Context, 1, 1);
	SDL_SetRenderDrawColor(Renderer::get().g_Context(), DEFAULT_DRAW_COLOR);
}

void Button::s_Text(std::string _text)
{
	Text = _text;
}
 

///================================================================================================
/// TEST BUTTON
///================================================================================================
TestButton::TestButton(Vec2 _pos, Vec2 _size)
	:
	Button(_pos, _size)
{
//	std::cout << ("Created Test Button");
	s_Text("Test Button");
}
void TestButton::OnUpdate(SDL_Event &_msg) 
{
	std::cout << ("OnUpdate");
}
void TestButton::OnRender()
{
	std::cout << ("OnRender");
}
void TestButton::OnMouseButtonDown(Vec2 _positon, uint8_t _button, uint8_t _state, uint8_t _clicks)
{
	Select();
	std::cout << ("OnMouseButtonDown");
} // SDL_MOUSEBUTTONDOWN
void TestButton::OnMouseButtonUp(Vec2 _positon, uint8_t _button, uint8_t _state) 
{
	Unselect();
	std::cout << ("OnMouseButtonUp");
} //SDL_MOUSEBUTTONUP
void TestButton::OnMouseMove(Vec2 _position, Vec2 _relative, Uint32 _buttonState)
{
	std::cout << ("OnMouseMove");
}   // SDL_MOUSEMOTION/
void TestButton::OnKeyDown(uint32_t _keycode, uint32_t _scancode, uint16_t _mod, uint8_t _repeat)
{
	std::cout << ("OnKeyDown");
} //SDL_KEYDOWN
void TestButton::OnKeyUp(uint32_t _keycode, uint32_t _scancode, uint16_t _mod)
{
	std::cout << ("OnKeyUp");
}   //SDL_KEYUP
///________________________________________________________________________________________________
///================================================================================================



///================================================================================================
/// Window
///================================================================================================


Window_Widget::Window_Widget(Vec2 _pos, Vec2 _size)
	:
	Widget_base(_pos, _size)
{
	s_Text("Test Window");

	static bool Init = false;
	if (!Init)
	{
		Skin = new Texture(new Image("Resources/GUIskin.png"));
		WindowTop = { 56,0, 23 , 23 };
		Sides = { 0,0,6,23 };
		TopLeft = { 8,0,8,12 };
		BottomLeft = { 8,11,8,12 }; ///19 23
		TopLeft = { 19, 0, 8, 12 };
		BottomRight = { 19, 11, 8, 12 };


		CloseX = { 92,0,24,24 };
		CloseXPressed = { 118, 0, 24, 24 };
		MinButton = { 144, 0, 24, 24 };
		MinButtonPressed = { 170,0,24,24 };
		Menu = { 32,0,24,24 };

		TopBoarder = { 15,0,7,7 };
		LeftBoarder = { 8,7,7,7 };
		RightBoarder = { 23,7,7,7 };
		BottomBoarder = { 15,16,7,7 };

		TopLeftCorner = { 8, 0,5,5 };// 11 might be wrong here
		BottomLeftCorner = { 8,11,12,12 };

		TopRightCorner = { 25, 0,5,5 };
		BottomRightCorner = { 19,12,12,12 };
	}
	Dragable = true;
	make_Child(new Boarder(LeftWindowBoarder, this));
	make_Child(new Boarder(RightWindowBoarder, this));
	make_Child(new Boarder(BottomWindowBoarder, this));
	make_Child(new MenuBar(this));

	//make_Child(new MenuButton({ 1,1 }, { 23,23 }));
	//make_Child(new CloseXButton({ _size.x - 23,1 }, { 23,23 }));
	//make_Child(new MinmizeButton({ _size.x - 23 * 2, 1 }, { 23,23 }));
	make_Child(new TestButton({ 20,20 }, { 145, 50 }));

}
void Window_Widget::OnUpdate(SDL_Event &_msg)
{
	//-----------------------------------------------------------	

	//LeftWindowBoarder:

	Children[0]->Area.Size = { 7, Area.Size.y };
	Children[0]->Area.RelativePosition.x = 0;
	Children[0]->Area.RelativePosition.y = 0;
	//	Area.RelativePosition.x = 0;		Area.RelativePosition.y = 0;

	//-----------------------------------------------------------	
//RightWindowBoarder:

	Children[1]->Area.Size = { 7, Area.Size.y };
	Children[1]->Area.RelativePosition.x = Area.Size.x - Children[1]->Area.Size.x;
	Children[1]->Area.RelativePosition.y = 0;
	//	Area.RelativePosition.x = Area.Size.x;		Area.RelativePosition.y = 0;

	//-----------------------------------------------------------	
    //BottomWindowBoarder:

	Children[2]->Area.Size = { Area.Size.x, 7 };
	Children[2]->Area.RelativePosition.x = 0;
	Children[2]->Area.RelativePosition.y = Area.Size.y;
	//	Area.RelativePosition.x = 0;		Area.RelativePosition.y = Area.Size.y;
	//-----------------------------------------------------------	

	//-----------------------------------------------------------	
	//Children[3]->Area.Size = { Area.Size.x, 23 };
	Children[3]->Area.RelativePosition = { 0, -23 };
	Children[3]->Area.Size = { Area.Size.x + 7, 24 };// form width + boarderWidth, Menubar Height
//	Widget_base({ 0,-23 }, { _parent->Area.Size.x + 7, 24 })
	//	make_Child(new MenuButton({ 1,1 }, { 23,23 }));
	//	make_Child(new CloseXButton({ Area.Size.x - 23,1 }, { 23,23 }));
	//	make_Child(new MinmizeButton({ Area.Size.x - 46, 1 }, { 23,23 }));
	//}
 //TopWindowBoarder:
//	Area.Position.x = 0;		Area.Position.y = 0;
	//	Area.RelativePosition.x = 0;		Area.RelativePosition.y = 0;

}

void Window_Widget::Render()
{
	SDL_Rect R =
	{
		 (int)Area.Position.x,
		 (int)Area.Position.y,
		 (int)Area.Size.x,
		 (int)Area.Size.y
	};

	SDL_SetRenderDrawColor(Context, 150, 150, 150, 255);
	SDL_RenderFillRect(Context, &R);

/// TEST BY RENDERING AS A TEXTURED RECT
//SDL_Point center = { 0,0 };
//SDL_Rect dest = {0, 0, 10, 10};// Position on the screen
//SDL_RenderCopyEx
//(// // Render the Bottom Boarder
//	Context,
//	Skin->Pixels,
//	&dest,
//	&R,
//	0,
//	&center,
//	SDL_FLIP_NONE
//);

	// Render the Children of the Window
	for (auto & C : Children)
	{// In Update, Children first, In Render We draw last so they remain ontop
		C->Render();
	}

	SDL_SetRenderDrawColor(Context, DEFAULT_DRAW_COLOR);
}
void Window_Widget::s_Text(std::string _text)
{
	Text = _text;
}

Boarder::Boarder(boarder_type _type, Window_Widget *_parent)
	:
	Widget_base(_parent->Area.Position, _parent->Area.Size),
	BoarderType(_type)
{
	SDL_Rect TopBoarder = { 15,0,7,7 };/// Assign directly in the c_tor
	SDL_Rect LeftBoarder = { 8,7,7,7 };
	SDL_Rect RightBoarder = { 23,7,7,7 };
	SDL_Rect BottomBoarder = { 15,16,7,7 };

	switch (_type)
	{
	case LeftWindowBoarder:
	{
		Area.Size = { 7, _parent->Area.Size.y };
		Area.Position.x = 0;		Area.Position.y = 0;
	//	Area.RelativePosition.x = 0;		Area.RelativePosition.y = 0;
		DrawRect = LeftBoarder;
	}break;

	case TopWindowBoarder:
	{	
 	Area.Position.x = 0;		Area.Position.y = 0;
//	Area.RelativePosition.x = 0;		Area.RelativePosition.y = 0;
	DrawRect = TopBoarder;
	}break;
	
	case BottomWindowBoarder: 
	{
	Area.Size = { _parent->Area.Size.x, 7 };
//
	Area.Position.x = 0;		Area.Position.y = _parent->Area.Size.y;
//	Area.RelativePosition.x = 0;		Area.RelativePosition.y = Area.Size.y;
	DrawRect = BottomBoarder;
	}break;

	case RightWindowBoarder:
	{	
	Area.Size = { 7, _parent->Area.Size.y };
	Area.Position.x = _parent->Area.Size.x - Area.Size.x;
	Area.Position.y = 0;
//	Area.RelativePosition.x = Area.Size.x;		Area.RelativePosition.y = 0;
	DrawRect = RightBoarder;
	}break;


	}
}
void Boarder::Update(SDL_Event &_msg)
{

	if (isMouseOver())
	{ // If Mouse is over out control, Pass the messages to it, else, pass back to the calling Widget

		switch (_msg.type)
		{// If the Message is handled by the Child Widget then Null the message or Pass a new message to the Parent
		case SDL_MOUSEMOTION:
			//if (Dragable _EQUALS_ true)
			//{
			//	if (isClicked())
			//	{
			//		Area.Position.x += _msg.motion.xrel;
			//		Area.Position.y += _msg.motion.yrel;
			//	}
			//}
			OnMouseMove(Vec2(_msg.motion.x, _msg.motion.y), Vec2(_msg.motion.xrel, _msg.motion.yrel), _msg.motion.state);
			break;

		case SDL_MOUSEBUTTONDOWN:
			Select();
			OnMouseButtonDown(Vec2(_msg.button.x, _msg.button.y), _msg.button.button, _msg.button.state, _msg.button.clicks);
			break;

		case SDL_MOUSEBUTTONUP:
			Unselect();
			OnMouseButtonUp(Vec2(_msg.button.x, _msg.button.y), _msg.button.button, _msg.button.state);
			break;

		case SDL_KEYDOWN:
			/// Maybe make it clicked if key like space or enter is pressed???
			OnKeyDown(_msg.key.keysym.sym, _msg.key.keysym.scancode, _msg.key.keysym.mod, _msg.key.repeat);
			break;

		case SDL_KEYUP:
			OnKeyUp(_msg.key.keysym.sym, _msg.key.keysym.scancode, _msg.key.keysym.mod);
			break;
		}

		_msg.type = NULL;
	}	
	OnUpdate(_msg);
}
void Boarder::OnUpdate(SDL_Event &_msg)
{
	if(_msg.type == SDL_MOUSEBUTTONUP)
	{
		Unselect();
	}///Make this an Else statement
	switch (BoarderType)
	{
	case LeftWindowBoarder:
	{
		if (isClicked())
		{
			float Displacement = InputDevices::get().Mouse.Relative.x;
			Parent->Area.Size.x -= Displacement;
			Parent->Area.Position.x += Displacement;
			//Area.Position.x += Displacement;
		}
	}break;

	case TopWindowBoarder:
	{
		if (isClicked())
		{
		}
	}break;

	case BottomWindowBoarder:
	{
		if (isClicked())
		{
			Parent->Area.Size.y += InputDevices::get().Mouse.Relative.y;
		}
	}break;

	case RightWindowBoarder:
	{
		if (isClicked())
		{
			float Displacement = InputDevices::get().Mouse.Relative.x;
			Parent->Area.Size.x += Displacement;
		//	Area.RelativePosition.x = Parent->Area.Size.x - Area.Size.x;
		}
	}break;
	}
}
void Boarder::Render()
{
	SDL_Point center = { 0,0 };
	SDL_Rect dest;// Position on the screen

	switch (BoarderType)
	{
	case LeftWindowBoarder:
	{
		dest = Renderer::make_Rect({Area.Position.x ,  Area.Position.y }, { DrawRect.w, Area.Size.y });
	}break;

	case TopWindowBoarder:
	{
		dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y }, { Area.Size.x, DrawRect.h});
	}break;

	case BottomWindowBoarder:
	{
		dest = Renderer::make_Rect({ Area.Position.x  , Area.Position.y + (Area.Size.y - DrawRect.h)  }, { Area.Size.x , DrawRect.h });
	}break;

	case RightWindowBoarder:
	{
		dest = Renderer::make_Rect({ Area.Position.x + Area.Size.x, Area.Position.y }, { DrawRect.w, Area.Size.y });
	}break;
	}


    SDL_RenderCopyEx
    (// // Render the Bottom Boarder
    	Context,
    	Skin->Pixels,
    	&DrawRect,
    	&dest,
    	0,
    	&center,
    	SDL_FLIP_NONE
    );

}


///================================================================================================
/// MENU BUTTON FOR TOP OF WINDOW CLOSE/MENU/MINIMIZE
///================================================================================================

MenuBar::MenuBar(Window_Widget *_parent)
	: 
	Widget_base({ 0,-23 }, {_parent->Area.Size.x + 7, 24})
{
	s_Skin(&_parent->g_Skin());
	make_Child(new MenuButton({ 1,1 }, { 23,23 }));
	make_Child(new CloseXButton({Area.Size.x - 23,1 }, { 23,23 }));
	make_Child(new MinmizeButton({ Area.Size.x - 46, 1 }, { 23,23 }));
}
void MenuBar::Render()
{
    SDL_Rect dest =
    {
        (int)Area.Position.x,
        (int)Area.Position.y, 
        (int)Area.Size.x, 
        (int)WindowTop.h
    };

    SDL_Point center =
    {
        0, 
        0
    };

    SDL_RenderCopyEx
    (// Render Top bar
    	Context,
    	Skin->Pixels,
    	&WindowTop,
    	&dest,
    	0,
    	&center,
    	SDL_FLIP_NONE
    );

	for (auto& Child : Children)
	{// 
		Child->Render();
	}
}
void MenuBar::OnUpdate(SDL_Event &_msg)
{
	Children[0]->Area.RelativePosition = { 1, 1 };                 // , { 23,23 })); // MenuButton
	Children[1]->Area.RelativePosition = { Area.Size.x - 23, 1 };  // , { 23,23 })); // CloseXButton
	Children[2]->Area.RelativePosition = { Area.Size.x - 46, 1 };  // , { 23,23 })); // MinmizeButton
}



MenuButton::MenuButton(Vec2 _position, Vec2 _size)
	:
	Button(_position, _size)
{
}
void MenuButton::Render()
{

	if (isClicked() == true)
	{
		SDL_Rect dest = Renderer::make_Rect({ Area.Position.x , Area.Position.y }, { 19, 19 });
		SDL_Point center = { 0,0 };
		SDL_RenderCopyEx
		(// Render Menu Button
			Context,
			Skin->Pixels,
			&Menu,
			&dest,
			0,
			&center,
			SDL_FLIP_NONE
		);
	}
	else
	{
		SDL_Rect dest = Renderer::make_Rect({ Area.Position.x ,   Area.Position.y }, { 19, 19 });
		SDL_Point center = { 0,0 };
		SDL_RenderCopyEx
		(// Render Menu Button
			Context,
			Skin->Pixels,
			&Menu,
			&dest,
			0,
			&center,
			SDL_FLIP_NONE
		);
	}
}

///================================================================================================
/// MENU BUTTON FOR TOP OF WINDOW CLOSE/MENU/MINIMIZE
///================================================================================================


MinmizeButton::MinmizeButton(Vec2 _position, Vec2 _size)
	:
	Button(_position, _size)
{
}
void MinmizeButton::Render()
{

	if (isClicked() == true)
	{
		SDL_Rect dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y }, { 19, 19 });
		SDL_Point center = { 0,0 };
		SDL_RenderCopyEx
		(// Render Menu Button
			Context,
			Skin->Pixels,
			&MinButtonPressed,
			&dest,
			0,
			&center,
			SDL_FLIP_NONE
		);
	}
	else
	{
		SDL_Rect dest = Renderer::make_Rect({ Area.Position.x, Area.Position.y }, { 19, 19 });
		SDL_Point center = { 0,0 };
		SDL_RenderCopyEx
		(// Render Menu Button
			Context,
			Skin->Pixels,
			&MinButton,
			&dest,
			0,
			&center,
			SDL_FLIP_NONE
		);
	}
}

///================================================================================================
/// MENU BUTTON FOR TOP OF WINDOW CLOSE/MENU/MINIMIZE
///================================================================================================

CloseXButton::CloseXButton(Vec2 _position, Vec2 _size)
	:
	Button(_position, _size)
{}
void CloseXButton::Render()
{
	if (isClicked() == true)
	{
		SDL_Rect dest = Renderer::make_Rect({ Area.Position.x ,  Area.Position.y }, { 19, 19 });
		SDL_Point center = { 0,0 };
		SDL_RenderCopyEx
		(// Render Menu Button
			Context,
			Skin->Pixels,
			&CloseXPressed,
			&dest,
			0,
			&center,
			SDL_FLIP_NONE
		);
	}
	else
	{
		SDL_Rect dest = Renderer::make_Rect({ Area.Position.x ,  Area.Position.y }, { 19, 19 });
		SDL_Point center = { 0,0 };
		SDL_RenderCopyEx
		(// Render Menu Button
			Context,
			Skin->Pixels,
			&CloseX,
			&dest,
			0,
			&center,
			SDL_FLIP_NONE
		);
	}
}







///================================================================================================
/// GUI CLASS
///================================================================================================

GUI::GUI(std::string _name)
	:
	Name(_name)
	//Widget_base({0,0},{1,1})
{
	//	TestButton *newbutton = new TestButton();
	Window_Widget *testWindow = new Window_Widget({ 500,200 }, { 300,300 });

	Window_Widget *testWindow2 = new Window_Widget({ 80,200 }, { 150,420 });
	Window_Widget *testWindow3 = new Window_Widget({ 500,20 }, { 400,100 });

	//Window_Widget *testWindow4 = new Window_Widget({ 50,20 }, { 50,50 });

	//	Add(newbutton);
	Add(testWindow);
	Add(testWindow2);
	Add(testWindow3);
//	Add(testWindow4);
}//


void GUI::Update(SDL_Event _msg)
{
	for (auto &C : Controls)
	{
		C->Update(_msg);
	}
}
void GUI::Render()
{
	for (auto &C : Controls)
	{
		C->Render();
	}
}
void GUI::Add(Widget_base *_widget)
{
	Controls.push_back(_widget);
}


