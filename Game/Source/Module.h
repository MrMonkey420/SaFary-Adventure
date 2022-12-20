#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class PhysBody;

class Module 
{
public:
	SString name;
	bool active = true;

public:

	App* Application;
	Module(App* parent, bool startEnabled) : Application(parent), active(startEnabled)
	{

	}

	void Init()
	{
		active = true; //CUIDADO activar
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&) //const
	{
		return true;
	}

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
	{

	}

	void Enable()
	{
		if (!active){
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active){
			active = false;
			CleanUp();
		}
	}

	inline bool IsEnabled() const 	//bool normal va mal
	{
		return active;
	}

};
#endif // __MODULE_H__