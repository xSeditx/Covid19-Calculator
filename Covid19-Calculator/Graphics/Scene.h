#pragma once
#include<vector>
#include<unordered_map>
#include"../Common.h"


//Uses
struct Sprite;
class Camera;
class Simulation;

struct Layer
{
    std::vector<GameObject *> Objects;

    void Add(GameObject *_object);
	void Remove(GameObject &_object);
	void Update(float _deltaTime);
	void Render();// iVec2 _cameraposition);
};

class Scene
{
public:
    Scene(std::string _name);

    void Attach(Camera *_camera);
	void Attach(GameObject &_object, uint32_t _layer);
	void Remove(GameObject &_object, uint32_t _layer);


    virtual void Update(float deltaTime);
    virtual void Render();   
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}


	Vec2 toScreen();
	Vec2 toWorld();

    Camera *View;
	float 
		CurrentTime = 0,
		PreviousTime = 0;

    std::vector<Layer> Groups;
	static std::unordered_map<std::string, Scene *> SceneMap;
};
