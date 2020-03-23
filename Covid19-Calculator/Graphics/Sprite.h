#pragma once

#include<vector>
#include<unordered_map>

#include"Time.h"
#include"../Common.h"


typedef std::pair<int, int> KeyFrame;
struct FrameTimer;
struct Texture;


struct Frame
{
    Frame(iVec2 _topleft, iVec2 _size)
        :
        Quad({_topleft.x, _topleft.y,_size.x, _size.y})
    {}
    SDL_Rect Quad;
};


/*
SDL_Rect _framesize   X/Y are the Starting positions in the Sprite Sheet, W/H are the Size
uint16_t _framecount  Number of Frames to add to the Sprite Animation
*/
struct Animation
{// Sequence of SDL_Rects that displays an Animation
	// (SDL_Rect _framesize<{x,y,w,h}>,  uint16_t _framecount = 1<number of sequential frames to make>);
	Animation()
		:
		Speed(NULL)
	{}
    Animation(SDL_Rect _framesize,  uint16_t _framecount = 1);

    void Add(SDL_Rect _frame);
	FrameTimer FrameTime;
	SDL_Rect Update(float _deltaTime);
    uint16_t CurrentFrame;
    uint32_t Speed;
	uint32_t PreviousTime;
    std::vector<Frame> Frames;
};

///==================================

struct Keyframe_Sequence
{
	void AddSequence(int _state, int _start, int _count);	// Animation State, < StartFrame,Framecount>
	std::vector<std::pair<int, std::pair<int, int>>> Sequence;	/// [Index].first{return state}-second{first{ starter frame}-second{ framecounter}}
	int Index = 0;;
	int CurrentFrame = 0;
	KeyFrame Update(float _deltaTime);// Returns Current State and Current Frame


	float PreviousTime = 0;
	float Speed = 100;
};



/*
Member Order
double
void*
size_t
int
short
char
bool
*/
#include"Gameobject.h"

struct Sprite 
{
	Sprite(Texture *_spritesheet, Vec2 _size);
	Sprite(std::string _file, Vec2 _size);
	~Sprite();
	idTag CurrentState;
	idTag PreviousState;

	SDL_RendererFlip Flip;
	SDL_Rect SourceRect;
	Texture *SpriteSheet;

	Vec2 Position;
	Vec2 Size;
	Vec2 RotationCenter; // This is different than what g_Center returns as that returns true Center and what we want here is the point which rotation spins around;

	Vec2 g_Center();
	Vec2 g_Offset();

	void Update(float _deltaTime);
	void Render();

	void s_AnimationSpeed(uint32_t _speed);
	void s_Rotation(float _angle);
	float g_Rotation() { return Rotation; }
	void s_Position(Vec2 _pos);
	void s_Position(float _x, float _y);
	void Rotate(float _amount);
    void AddState(std::string _name, Animation _state);
    void s_CurrentState(std::string _currentState);

	std::vector<Animation> States;
	std::unordered_map<std::string, int> Statemap;
		
	static Sprite& New(std::string _name, Texture *_spritesheet)
	{
		SpriteMap[_name] = new Sprite(_spritesheet, Vec2(NULL));
		return *SpriteMap[_name];
	}
    int g_State(std::string _name)
    {
    	return Statemap[_name];
    }

	
	std::vector<Keyframe_Sequence> KeyFrames;

	static std::unordered_map<std::string, Sprite*> SpriteMap;
	inline static Sprite& GetSprite(std::string _name) { return *SpriteMap[_name]; };

	idTag Play(idTag _seq);

	bool isPlaying();
private:
	int Playing = -1;
	float Rotation = 0;

};










/*

Need something new for Sprite Animation.





vector<SDL_Rect> Frames;

vector<State> States;
class State
{
    vector<SDL_Rect> Sequence;    
}
AddSequence(startFrame, count)
{
};


unordered_map<string, int> StateNames;

*/