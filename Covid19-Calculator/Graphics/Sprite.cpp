#include"Sprite.h"
#include"Image.h"
#include"Camera.h"
#include"Renderer.h"

std::unordered_map<std::string, Sprite*> Sprite::SpriteMap;
///__________________________________________________________________________________________________________________________
///============================================ ANIMATION ===================================================================

/*
_framesize.x/y = starting position to begin cutting the sprite sheet
_framesize.w/h = how large each quad is to be in size
*/

Animation::Animation(SDL_Rect _framesize, uint16_t _framecount)
	:
	CurrentFrame(0),
	Speed(100)
{
	Vec2 Size = Vec2(_framesize.w, _framesize.h);
	Vec2 Topleft = Vec2(_framesize.x, _framesize.y);
	Vec2 Bottomright = Topleft + Size;
	for_loop(i, _framecount)
	{
		Frames.push_back(Frame(Topleft, Size));
		Topleft.x += Size.x;
	}
}


//float Acum = 0; Worth a Shot.
/// TODO: Something is odd, I need to just be able to use _deltaTime here to decide when to update the Sprite
/// That should just consist of Accumulating the _deltaTimes until it passes the amount desired however that appears to cause choppy animation so idk yet 
SDL_Rect Animation::Update(float _deltaTime)
{

	uint32_t CurrentTime = SDL_GetTicks();

	if (CurrentTime - PreviousTime > Speed)	//float dt = CurrentTime - PreviousTime;
	{
		PreviousTime = CurrentTime;
		int A = FrameTime.Update();
		//TODO("IMPLEMENT THE SPRITE TIMER IN THE SPRITE CLASS TO DECIDE IF TO EVEN ATTEMPT AT UPDATING IT, maybe an if_Time(...do stuff) function");
		++CurrentFrame;
		CurrentFrame %= Frames.size();
	}
	return Frames[CurrentFrame].Quad;
}
void  Animation::Add(SDL_Rect _frame)
{
	Frames.push_back
	(
		Frame
		(
			Vec2
			(
				_frame.x,_frame.y
			),
			Vec2
			(
				_frame.w,_frame.h 
			)
		)
	);
}



///__________________________________________________________________________________________________________________________
///=============================================== SPRITE ===================================================================

Sprite::Sprite(std::string _file, Vec2 _size)
	:
	Sprite(new Texture(new Image(_file)), _size)
{
	std::cout << "Loading: " << _file;
	Rotation = 0;
	if (Size.x == NULL || Size.y == NULL)
	{
		Size = SpriteSheet->Size;
	}
	RotationCenter = _size * 0.5f;
	assert(SourceRect.x != 64);
	Playing = -1;
}

Sprite::Sprite(Texture *_spritesheet, Vec2 _size)
    :
    Position(iVec2(0)),
	Rotation(0),
    Size(_size),
    SpriteSheet(_spritesheet),
    Flip(SDL_FLIP_NONE),
    SourceRect
    (
        {
            (int)0,
            (int)0,
            (int)_size.x,
            (int)_size.y
		}
	)
{
	Rotation = 0;
	if (Size.x == NULL || Size.y == NULL)
	{
		Size = _spritesheet->Size;
	}
	RotationCenter = _size * 0.5f;
	assert(SourceRect.x != 64);

	Playing = -1;
}
Vec2 Sprite::g_Center()
{
	return (Vec2)Position + ((Vec2)Size * .5f);
}
Vec2 Sprite::g_Offset()
{
	return (Vec2)Size * .5f;
}
void Sprite::AddState(std::string _name, Animation _state)
{
	Statemap[_name] = States.size();/// Might have to put this first so it points to the correct one
	States.push_back(_state);
	s_CurrentState(_name);
}
void Sprite::Update(float _deltaTime)
{
	//TODO("Delta is not being passed into Sprite Update Fix it");
	if (Playing != -1)
	{
		
		KeyFrame Seq = { 0,0 };
		Seq = KeyFrames[Playing].Update(_deltaTime);
		
	//	Print("Sprite::Update() - " << "First: " << Seq.first << " Second: " << Seq.second);
		if (Seq.first == -1)
		{// The Animation is done playing and we should return to where we were before
			Playing = -1;
			CurrentState = PreviousState;
		}
		else
		{
			CurrentState = Seq.first;
			int F = Seq.second % States[CurrentState].Frames.size();
			SourceRect = States[CurrentState].Frames[F].Quad;
		}
	}
	else
	{ 
	    SourceRect = States[CurrentState].Update(_deltaTime);
	}
}
void Sprite::Render()
{
	Vec2 AdjustedPos = (Vec2)Position - (Vec2)Size * .5f;  // Find the Center of the Sprite
	Vec2 Offset = (iVec2)Camera::get().g_Center(); // Adjust the Render Position According to the Camera Position sent in              
	
	SDL_Rect dest = Renderer::make_Rect
	(// The point in Screen Space which the image will be Rendered
		Vec2((AdjustedPos.x + Offset.x), (AdjustedPos.y + Offset.y)),
		Vec2(Size.x, Size.y)
	);

	SDL_Point center = { (int)RotationCenter.x, (int)RotationCenter.y };
	SDL_RenderCopyEx
	(// Render Sprite in position and Orientation as defined by its state and that of the Camera
		Renderer::get().g_Context(),
		SpriteSheet->Pixels,
		&SourceRect,
		&dest,
		Rotation,
	    &center,
		Flip
	);
}
void Sprite::s_CurrentState(std::string _currentState)
{
#if NDEBUG 
	CurrentState = Statemap[_currentState];
#else
	auto Location = Statemap.find(_currentState);
	assert(Location != Statemap.end());
	CurrentState = Statemap[_currentState];
#endif
}
 
void Sprite::s_AnimationSpeed(uint32_t _speed)
{
	for (auto& S : States)
	{
		S.Speed = _speed;
	}
}


void Sprite::s_Position(Vec2 _pos) { Position = _pos; }
void Sprite::s_Position(float _x, float _y) { Position = { _x,_y }; }
void Sprite::s_Rotation(float _angle) 
{
	Rotation = _angle;
}
void Sprite::Rotate(float _amount)
{
	Rotation += _amount;
}

Sprite::~Sprite()
{
	//DEBUG_TRACE("Sprite Destroyed");
	/// warning C4150: deletion of pointer to incomplete type 'SDL_Texture'; no destructor called
	// delete(SpriteSheet->Pixels);// IDK, Fuck it... close enough for now...
	delete(SpriteSheet); // Creation Consisted of..... This clusterfuck..... new Sprite(new Texture(new Image(RESOURCE_PATH + "launcher_base.png")), Vec2(64, 64));
}


idTag Sprite::Play(idTag _seq)
{
	if (Playing == -1)
	{
		PreviousState = CurrentState;
		Playing = _seq;
		return PreviousState;
	}
	else
	{ // We are already playing an Animation and must Cancel and play new
	//	CurrentState = PreviousState;
	//	Playing = -1;
		Playing = _seq;
		return -1;
	}
}

void Keyframe_Sequence::AddSequence(int _state, int _start, int _count)
{
	Sequence.push_back
	(
		{
			_state,
		{_start, _count }
		}
	);
}
std::pair<int, int> Keyframe_Sequence::Update(float _deltaTime)// Returns Current State and Current Frame
{
	//TODO("DELTA is not being passed into Keyframe Update, Fix it");
	uint32_t CurrentTime = SDL_GetTicks();
	if (CurrentTime - PreviousTime > Speed)
	{
		PreviousTime = CurrentTime;
		if (CurrentFrame > (Sequence[Index].second.first + Sequence[Index].second.second))/* current is greater than start frame + frame count*/
		{
			CurrentFrame = Sequence[Index].second.first;
			if (++Index >= Sequence.size())
			{
				Index = 0;
				return { -1, 0 };
			}
			return { Sequence[Index].first, CurrentFrame++ };
		}
		else
		{
 			return { Sequence[Index].first, CurrentFrame++ };
		}
	}
	return { Sequence[Index].first, CurrentFrame };
}





bool Sprite::isPlaying()
{
	return !(Playing == -1);
}
