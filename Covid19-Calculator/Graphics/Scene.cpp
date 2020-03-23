#include"../Common.h"

#include"Sprite.h"
#include"Camera.h"
#include"Scene.h"
#include"Window.h"
#include"Renderer.h"
#include"GameObject.h"

//#include"Physics.h"


std::unordered_map<std::string, Scene *> Scene::SceneMap;

void Layer::Add(GameObject *_object)
{
    Objects.push_back(_object);
	if (_object->hasCollider())
	{
		Objects.back()->g_Collider().s_Parent(_object);
	}
}

void Layer::Update(float _deltaTime)
{

    for (auto& O : Objects)
    {
		if (O->Alive())O->Update(_deltaTime);
    }
}


void Layer::Render()// iVec2 _cameraposition)
{
    for (auto& O : Objects)
    {
		if (O->Alive())
		{
			O->Render();
		}
    }
}


Scene::Scene(std::string _name)
{// Constructs a Level to handle Sprites and Collisions
	Groups.push_back(Layer());
	SceneMap.insert({ _name,  this });
	View = &Camera::get();
}
Vec2 Scene::toScreen()
{
	return  View->Position;
}
Vec2 Scene::toWorld()
{
	return View->Position; 
}
#include"Time.h"
void Scene::Update(float deltaTime)
{// Updates All Colliders and Animations for the Level

//	BenchMark Tester;
	{
		CurrentTime = SDL_GetTicks() / 1000.f;
		float DeltaTime = (CurrentTime - PreviousTime);
		PreviousTime = CurrentTime;

		Camera::get().Update(DeltaTime);
		for (auto& L : Groups)
		{
			L.Update(DeltaTime);
		}
///		Simulation::get().Update(DeltaTime);
		OnUpdate(DeltaTime);
	}
}


void Scene::Render()
{// Renders all the Sprites for the Level
     for (auto& L : Groups)
     {
		 L.Render();// Camera::get().g_Center());
     }
	OnRender(); // Calls User defined custom Update
}


void Scene::Attach(Camera *_camera)
{/// This is not even in use at the moment
	__debugbreak();
    View = _camera;
}
void Scene::Attach(GameObject &_object, uint32_t _layer)
{// Adds a Gameobject to a Given Level

	if (_object.hasCollider())
	{// If the Object has a Collidable interfact add it to the Collision Solver
		_object.g_Collider().s_Parent(&_object);
	///	Simulation::get().Add(&_object);
	}

	if (Groups.size() <= _layer)
	{// If the number of Groups is lower than the desired Layer, create the intermediate layers and place the Object in said Layer
		Groups.push_back(Layer());
		Groups.back().Add(&_object);
	}
	else
	{// Place object into desired Layer
		Groups[_layer].Add(&_object);
	}
}


void Scene::Remove(GameObject &_object, uint32_t _layer)
{
	Groups[_layer].Remove(_object);
	/// Simulation::get().Remove(&_object);
}


void Layer::Remove(GameObject &_object)
{
	//_object.Deactivate();
    auto It = std::find(Objects.begin(), Objects.end(), &_object);
    if (It != Objects.end())
    {
    	Objects.erase(It);
    }

}