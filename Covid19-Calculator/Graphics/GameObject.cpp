#include"GameObject.h"
#include"Sprite.h"
#include"Collider.h"
#include"Image.h"

#ifdef _DEBUG
#  define _RENDER_HITBOXES_
#endif

_static size_t GameObject::ObjectCount = 0;

/// ==============================================================================================================
/// Going to need to perform a has_value check on Hitbox and if it has a Collider use that, else, use the Sprite
/// ______________________________________________________________________________________________________________
/// ==============================================================================================================

GameObject::GameObject(Sprite *_sprite, Collider *_collider)
	:
	Picture(_sprite),
    Hitbox(_collider),	
	GameObjectID(ObjectCount++),
	ID(ObjectCount)
{	
	if (Picture)
	{
		s_Position(_sprite->Position);
		s_Rotation(_sprite->g_Rotation());
	}
}
void GameObject::Update(float _deltaTime)
{
	if (! Active)
	{
		return;
	}

	if (Hitbox)
	{// Shitty hack I have to do because I want to allow GameObjects to exist without having a Collider. Likely design something better in the future
		if (g_Collider().Alive)
		{
			assert(&g_Collider().g_Parent() != nullptr);
			g_Collider().Update(_deltaTime);   // Update the Physics Object
			g_Sprite().Position = g_Position(); // Places the Picture to where the Object now is
			g_Sprite().s_Rotation(g_Rotation()); // So that the Picture, Object, and Collider all now Line up and are in the Same position
		}
	}
	for (auto& C : Children)
	{
		C->Update(_deltaTime);
	}

	g_Sprite().Update(_deltaTime);            /// Feels redundent and there might be a better way but it will have to be carefully thought out before I implement anything I am considering and aint nobodi gotz time 'fo dat
	OnUpdate(_deltaTime);         // Must remember this is being called AFTER the default update procedure so all changes will no be visible until next frame. Possibly change this
}


void GameObject::Render()
{
	if (! Active)
	{
		return;
	}
 	PreRender();

#ifdef _RENDER_HITBOXES_
	if (Hitbox)
	{
		g_Collider().Render();
	}
#endif

	g_Sprite().Render();

	for (auto& C : Children)
	{
		C->Render();
	}

	PostRender();
}
void GameObject::MoveX(float _amount)
{ 
    if (Hitbox) g_Collider().Position.x += _amount;
	else g_Sprite().Position.x += _amount;
}
void GameObject::MoveY(float _amount)
{ 
	if (Hitbox)g_Collider().Position.y += _amount;
	else g_Sprite().Position.y += _amount;
}
void GameObject::Move(float _x, float _y)
{ 	
	if (Hitbox) g_Collider().Position += Vec2(_x, _y);
	else g_Sprite().Position += Vec2(_x, _y);
}
void GameObject::Activate()
{ 
	if (Hitbox)g_Collider().Alive = true;
	Active = true; 
}
void GameObject::Deactivate()
{
	if (Hitbox)
	{
		g_Collider().Alive = false;
	}
	Active = false;
}

void GameObject::OnUpdate(float _deltatime) 
{}
void GameObject::PostRender()
{}
void GameObject::PreRender()
{}

std::ostream& operator << (std::ostream &lhv, GameObject  &rhv)
{// For debug reasons I wish this to post all player information when asked
	lhv << "Position:  " << "X: " << rhv.g_Position().x << "Y: " << rhv.g_Position().y << "\n";	
	lhv << "Angle: " << rhv.g_Rotation();

	if (rhv.Hitbox)
	{
 
		lhv << "HBPosition:  " << "X: " << rhv.g_Collider().Position.x << "Y: " << rhv.g_Collider().Position.y << "\n";
		lhv << "HBVelocity:  " << "X: " << rhv.g_Collider().Velocity.x << "Y: " << rhv.g_Collider().Velocity.y << "\n";
	}
	else
	{
		lhv << "Object Contains no Collidable Physics Object \n";
	}
	return lhv;
}

 

void GameObject::make_Child(GameObject *_object)
{//TODO Should make the Childrens position Relative to the Main Object as it would open many doors but good for now
	Children.push_back(_object);
}



/*
    SETTERS IN SOME CASES SETTING THE VALUES OF THE ATTACHED COLLIDER
*/
void GameObject::s_Velocity(Vec2 _speed)
{
	if (Hitbox)/// This should really just be an assert so that it goes away in finished code
		g_Collider().Velocity = _speed;
}
void GameObject::s_VelocityX(float _speed)
{
	if (Hitbox)
		g_Collider().Velocity.x = _speed;
}
void GameObject::s_VelocityY(float _speed)
{
	if (Hitbox)
		g_Collider().Velocity.y = _speed;
}

void GameObject::s_PositionX(float _x)
{
	if (Hitbox)
	{
		g_Collider().Position.x = _x;
	}
	g_Sprite().Position.x = _x;
}
void GameObject::s_PositionY(float _y)
{
	if (Hitbox)
	{
		g_Collider().Position.y = _y;
	}
	g_Sprite().Position.y = _y;
}
void GameObject::s_Position(float _x, float _y)
{
	/// Keep a close Eye on these two, does this have unintended consequences perhaps?
	if (Hitbox)
	{
		g_Collider().Position = Vec2(_x, _y);
	}
	g_Sprite().Position = Vec2(_x, _y);
}
void GameObject::s_Position(Vec2 _position)
{
	if (Hitbox)
	{
		g_Collider().Position = _position;
	}
	g_Sprite().Position = _position;
}

void GameObject::s_Rotation(float _angle)
{ 	/// Going to need to perform a has_value check on Hitbox and if it has a Collider use that, else, use the Sprite
	if (Hitbox)
	{
		g_Collider().Rotation = _angle;
	}
	g_Sprite().s_Rotation(_angle);
}
void GameObject::s_Sprite(Sprite *_sprite)
{
	Picture = _sprite;
}
void GameObject::s_Collider(Collider *_collider)
{
	Hitbox = _collider;
}

void GameObject::applyFriction(float _coeff)
{// Should be in Collider Class and we call that perhaps
	Vec2 V = g_Velocity();
	s_Velocity(V * _coeff);
}

bool GameObject::Alive()
{
	return Active;
}








bool GameObject::hasCollider()
{
	return Hitbox != nullptr;
	//Collidable
}
bool GameObject::hasSprite()
{
	return Picture != nullptr;
	//Renderable
}
bool GameObject::hasChildren()
{
	return !(Children.empty());
}

float GameObject::g_Rotation()
{
	return Hitbox ? g_Collider().Rotation : g_Sprite().g_Rotation();
}
Vec2 GameObject::g_Position()
{
	return Hitbox ? g_Collider().Position : g_Sprite().Position;
}
float GameObject::g_PositionX()
{
	return Hitbox ? g_Collider().Position.x : g_Sprite().Position.x;
}
float GameObject::g_PositionY()
{
	return Hitbox ? g_Collider().Position.y : g_Sprite().Position.y;
}
Vec2 GameObject::g_Velocity()
{
	return Hitbox ? g_Collider().Velocity : Vec2(0, 0);
}
Sprite& GameObject::g_Sprite()
{
	return *Picture;
}
Collider& GameObject::g_Collider()
{
	return *Hitbox;
}
float GameObject::g_VelocityX()
{
	return Hitbox ? g_Collider().Velocity.x : 0;
}
float GameObject::g_VelocityY()
{
	return Hitbox ? g_Collider().Velocity.x : 0;
}
Vec2  GameObject::toWorldSpace(float _x, float _y)
{
	return  Vec2(_x, _y) + g_Position();
}
Vec2  GameObject::toWorldSpace(Vec2 _position)
{
	return _position + g_Position();
}
Vec2  GameObject::toModelSpace(float _x, float _y)
{
	return Vec2(_x, _y) + (g_Position() + Camera::get().g_Center());
}
Vec2  GameObject::toModelSpace(Vec2 _position)
{
	return _position + (g_Position() - Camera::get().g_Center());
}
float GameObject::toModelSpaceX(float _x)
{
	return _x - (g_PositionX() + Camera::get().g_Center().x);
}
float GameObject::toModelSpaceY(float _y)
{
	return _y - (g_PositionY() + Camera::get().g_Center().y);
}
