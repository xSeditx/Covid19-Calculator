#include"Time.h"

Timer::Timer()
	:
	DeltaTime(NULL),
	PreviousTime(NULL)
{
    StartTime = SDL_GetTicks();
}
double Timer::Update()
{
    CurrentTime = SDL_GetTicks();
    DeltaTime = CurrentTime - PreviousTime;
    PreviousTime = CurrentTime;
    return CurrentTime;
}

FrameTimer::FrameTimer()
	:
	Counter(NULL),
	DeltaTime(NULL),
	PreviousTime(NULL)
{
	CurrentTime = (float)SDL_GetTicks();
}

void FrameTimer::Set(float rate)
{
    Coefficient = 1000.0f / rate;
}

uint16_t FrameTimer::Update()
{
	CurrentTime = (float)SDL_GetTicks();
	DeltaTime = CurrentTime - PreviousTime;
	PreviousTime = CurrentTime;
	++Counter;
	return (uint16_t)DeltaTime;
}


















 
Trigger_Timer::Trigger_Timer(uint32_t _ms)
	:
	Wait(_ms),
	StartTime(SDL_GetTicks())
{}


bool Trigger_Timer::Check()// FUKKKKKKKKKKKKKKK Thats the problem, It returns 0 Also when the Timer has been Active as well as when it has never been activated at all so it is immediately turning hitbox back on.
{// My brain is shit today, we are making tis an Int because I dont have time for this shit
	if (Active != true)
	{
		return false;
	}

	uint32_t Current = SDL_GetTicks();
	float DeltaTime = Current - StartTime;

	if ((DeltaTime) > Wait)
	{
		Active = false;
		return false;
	}
	//ELSE
	return true;
}

void Trigger_Timer::Start()
{
	Active = true;
	StartTime = SDL_GetTicks();
}

void Trigger_Timer::Start(uint32_t _ms)
{
	Wait = _ms;
	Active = true;
	StartTime = SDL_GetTicks();
}

void Trigger_Timer::Set_Timer(uint32_t _ms)
{
	Wait = _ms;
}

