//#include"EventHandler.h"
#include"Camera.h"

_static Camera *Camera::ActiveCamera;
_static std::stack<Vec2> Camera::TransformationStack;



Camera::Camera(Vec2 _position, Vec2 _size)
    :
    Position(_position),
    TargetPosition(_position),
    TargetSize(_size),
    Viewport
    (
        {
            (int)_position.x,
            (int)_position.y, 
            (int)_size.x,
            (int)_size.y
        }
    ),
    Speed(100.5f),
	Halfsize( _size * 0.5f)
{}

void Camera::Goto(int _x, int _y)
{
    TargetPosition = Vec2(_x, _y);
}
void Camera::Goto(Vec2 _position)
{
	TargetPosition = _position;
}
void Camera::SnapTo(int _x, int _y)
{
	Position = { _x, _y };
}
void Camera::SnapTo(Vec2 _pos)
{
	Position = _pos;
}

void Camera::Update(float _deltaTime)
{
    //uint32_t Time = SDL_GetTicks();
  //  Delta_Time   = (Time - Current_Time);
   // Current_Time = (float)Time;
	Delta_Time = _deltaTime;
    Position += (TargetPosition - Position) * Speed * (_deltaTime / 1000.f); //(Delta_Time / 1000.f);
}
void Camera::MoveUp(float _dist)
{
    TargetPosition.y -= _dist;
}
void Camera::MoveDown(float _dist)
{
    TargetPosition.y += _dist;
}
void Camera::MoveLeft(float _dist)
{
    TargetPosition.x -= _dist;
}
void Camera::MoveRight(float _dist)
{
    TargetPosition.x += _dist;
}

Vec2 Camera::g_Position()
{/// Currently is the same as g_Center, will likely remove it in the near future
    return
    {// Vec2 of Viewport Topleft + [Width|Height] / 2 to find the Center
        Position.x + (Viewport.x + ((float)Viewport.w * .5f)),
        Position.y + (Viewport.y + ((float)Viewport.h * .5f))
    };
}
Vec2 Camera::g_Center()
{
    return
    {// Vec2 of Viewport Position + Topleft + [Width|Height] / 2 to find the Center
        Position.x + (Viewport.x + ((float)Viewport.w * .5f)),
        Position.y + (Viewport.y + ((float)Viewport.h * .5f))
    };
}

_static Vec2 Camera::toScreenSpace(Vec2 _position)
{
	return _position + ActiveCamera->g_Center();
}
_static Vec2 Camera::toScreenSpace(float _x, float _y)
{
	return Vec2(_x, _y) + ActiveCamera->g_Center();// The multiplication I was doing would be for Scaling Get Ratio of real ScreenSize vs Our Camera Size and Multiple the Pixel by that
}
_static Vec2 Camera::toWorldSpace(Vec2 _position)
{
	return _position - (ActiveCamera->Position * (ActiveCamera->TargetSize * 0.5f));
}
_static Vec2 Camera::toWorldSpace(float _x, float _y)
{
	return Vec2(_x, _y) - (ActiveCamera->Position * (ActiveCamera->TargetSize * 0.5f));
}