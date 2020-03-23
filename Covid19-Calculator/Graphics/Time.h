#pragma once
#include<functional>

#include"../Common.h"

struct Timer
{
    Timer();
    double Update();
    double StartTime;
    double DeltaTime;
    double PreviousTime;
    double CurrentTime;
};


struct FrameTimer
{
	FrameTimer();
	float Rate;
    void Set(float rate);
    float Get() { return Rate; }

	uint16_t Update();


    float CurrentTime = 0.0f;
    float DeltaTime = 0.0;
    float PreviousTime = 0.0f;

    float Coefficient = 0.0f;
    float Tracker = 0.0f;

    unsigned int Counter = 0;
};



//template<typename _Func , typename... Arg>
struct DeltaTimer
{
	DeltaTimer(float _msCount)///l , _Func&& _f, Arg... _args)
	{
		DeltaTime = _msCount;
		StartTime = SDL_GetTicks();
	///	f = std::bind(_f, _args);
	}

	float WaitUntil;

	bool Update()
	{
	//	bool result = ;
		//if (result _EQUALS_ true)
		//{
		//	f();
		//}
		return ((SDL_GetTicks() - StartTime) > DeltaTime);// result;
	}

	float CurrentTime = 0.0f;
	float DeltaTime = 0.0;
	float PreviousTime = 0.0f;
	float StartTime = 0.0;

	///_Func f;
};


struct Trigger_Timer
{
	/*
	Starts a Timer when the Player is Hit and returns the Opposite of the Hitbox Alive state
	Use: g_Collider().Alive = ~Invincibility_Timer::Check();
	User prior to Update(); when possible else effects won't take place till next frame.
	*/
	Trigger_Timer(uint32_t _ms);

	bool Check();
	void Start();
	void Start(uint32_t _ms);
	void Set_Timer(uint32_t _ms);
	bool Active;
	uint32_t Wait;
	uint32_t StartTime;
};


// https://www.learncpp.com/cpp-tutorial/99-overloading-the-parenthesis-operator/
class Accumulator
{
private:
	uint32_t wait = 0;
	double counter = 0;
	bool active = false;
public:
	Accumulator(uint32_t _ms)
	{
	}
	void reset()
	{
		active = false; 
		counter = 0.0f;
	}
	void start()
	{
		active = true;
		counter = 0.0f;
	}
	bool ready()
	{
		if(active != true)
		{
			return false;
		}
		if(counter > wait)
		{
			reset();
			return false;
		} 
		return true;
	}
	int operator() (double _i) { return (counter += _i); }
};









//
//template<typename _Func>
//struct TimerTrigger
//{
//	TimerTrigger(float _time, _Func&& f)
//		:
//		Timer(_time)
//	{
//		Response = f;
//	}
//
//	template<typename ...ARGS>
//	bool Update(ARGS&&...args)
//	{
//		float Deltatime = SDL_GetTicks() - CurrentTime;
//		if (Deltatime < Timer)
//		{
//			Response(args...);
//		}
//	}
//	template<typename ...ARGS>
//	_Func Response(ARGS&&...args)
//	{
//		Print("Response");
//	}
//	float Timer;
//	float CurrentTime = 0.0f;
//	float DeltaTime = 0.0;
//	float PreviousTime = 0.0f;
//};