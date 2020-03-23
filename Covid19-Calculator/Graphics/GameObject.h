#pragma once
#include"../Common.h"

#include<vector>
#include<unordered_map>
#include<string>


#define GAME_OBJECT_DEBUG(x)    if(_NOT_ Hitbox) std::cout << "Function: " << typeid(this).name() << " Msg: " << x << "\n"

struct Sprite;
struct Collider;
//#include"Collider.h"
enum Direction
{
	Left, Right, Up, Down, Pause
};
struct Script
{
	float Speed;
	std::vector<std::pair<Direction, int>> Action;
};
 

class GameObject
{
public:
	GameObject() = delete;
	GameObject(Sprite *_sprite) : GameObject(_sprite, NULL) {}
	GameObject(Sprite *_sprite, Collider *_collider);
	
	std::string Name;
	std::vector<GameObject*> Children;

	void Activate();
	void Deactivate();


	void make_Child(GameObject *_object);
	void applyFriction(float _coeff);


	virtual void Update(float _deltatime);
	virtual void OnUpdate(float _deltatime);	// Gets called AFTER the default Updater. Changes made will become visible physically the next frame unless explicitly stated

	virtual void Render();
	virtual void PreRender();
	virtual void PostRender();


	Sprite&   g_Sprite();	void s_Sprite(Sprite *_sprite);
	Collider& g_Collider();	void s_Collider(Collider *_collider);

	Vec2  g_Position();	void s_Position(Vec2 _position);    void s_Position(float _x, float _y);
	float g_PositionX();	void s_PositionX(float _x);
	float g_PositionY();	void s_PositionY(float _y);

	Vec2  g_Velocity();	void s_Velocity(Vec2 _speed);	
	float g_VelocityX();	void s_VelocityX(float _speed);
	float g_VelocityY();	void s_VelocityY(float _speed);

	float g_Rotation();	void s_Rotation(float _angle);


	void Move(float _x, float _y);	void MoveX(float _amount);	void MoveY(float _amount);
	void Move(Vec2 _amount);


	bool Alive();
	bool hasCollider(); 	  bool hasSprite();
	bool hasChildren();
	Vec2 toWorldSpace(float _x, float _y);    Vec2 toWorldSpace(Vec2 _position);
	Vec2 toModelSpace(float _x, float _y);    Vec2 toModelSpace(Vec2 _position);

	float toModelSpaceX(float _x);	float toModelSpaceY(float _Y);



	idTag ID;
	size_t GameObjectID;


	bool operator ==(GameObject rhv) { return this == &rhv; }
	friend std::ostream& operator << (std::ostream &lhv, GameObject &rhv);


	static size_t ObjectCount;
private:
	bool Active = true;
	bool Collidable = true;
	bool Renderable = true;

	Sprite *Picture = nullptr;
	Collider *Hitbox = nullptr;
};

std::ostream& operator << (std::ostream &lhv, GameObject &rhv);









/*
	SETTERS IN SOME CASES EXTRACTING THE VALUES FROM THE ATTACHED COLLIDER
*/

#include"Camera.h"
#include"Collider.h"
#include"Sprite.h"



/*
IF LHV type != to &GameObject true type Results in:
	__declspec(noreturn) void __CRTDECL __scrt_throw_std_bad_alloc()
	{
		throw std::bad_alloc{};
	}

#IDEA:	I could Template out Object cast so that Each base class can support it's own Children
Subclass of GameObject _sub_ = object_cast<GameObject>(_param_of_ GameObject type)
*/

struct object_cast 
{
	object_cast(GameObject *_object)
		:result(_object)
	{}
	object_cast(GameObject &_object)
		:result(&_object)
	{}

	template <class _Ty>
	operator _Ty() const { using value_type = _Ty; return *(_Ty*)result; }

	template <class _Ty>
	operator _Ty*() const { using pointer_type = _Ty;  return (_Ty*)result; }
private:

	void *result;
};



//
//
//template <class _Ty>
//operator _Ty() const
//{
//	static void *te = result;
//	static union
//	{
//		_Ty *o;
//		void *c;
//	}r;
//	return *(_Ty*)result;
//}