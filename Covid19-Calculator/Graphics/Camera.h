#pragma once
#include"../Common.h"

class Camera
{
public:
    Camera(Vec2 _position, Vec2 _size);

    Vec2 Position; /// This may go idk yet
    Vec2 TargetPosition;
    Vec2 TargetSize;

    SDL_Rect Viewport;

    Vec2 g_Position();
    float Speed;

    float Delta_Time = 0.0f;
    float Current_Time = 0.0f;

	void Goto(int _x, int _y); 
	void Goto(Vec2 _pos);
	void SnapTo(int _x, int _y);
	void SnapTo(Vec2 _pos);
	void MoveUp(float _dist);
    void MoveDown(float _dist);
    void MoveLeft(float _dist);
    void MoveRight(float _dist);
	virtual void Update(float _deltaTime);

    Vec2 g_Center();

	inline static void set(Camera *_camera)
	{
		ActiveCamera = _camera;
	}
	inline static Camera &get()
	{
		return *ActiveCamera;
	}
	inline static bool Initialize(Vec2 _position, iVec2 _size)
	{
		if (ActiveCamera == nullptr) { ActiveCamera = new Camera(_position, _size); }
	}

	static Vec2 toScreenSpace(Vec2 _position);
	static Vec2 toScreenSpace(float _x, float _y);

	static Vec2 toWorldSpace(Vec2 _position);
	static Vec2 toWorldSpace(float _x, float _y);

	static std::stack<Vec2> TransformationStack;
private:
	static Camera *ActiveCamera;
	Vec2 Halfsize;
};


