#ifndef __SCENE2_H__
#define __SCENE2_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy2.h"
#include "Enemy1.h"

struct SDL_Texture;


#define CAMERASPEED 15

class Scene2 : public Module
{
public:

	Scene2(App* application, bool start_enabled);

	// Destructor
	virtual ~Scene2();

	List<Enemy2*> Enemisuelos;
	List<Enemy1*> Enemiaires;
	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;


	bool stopcamera;
	bool godmode;
	bool level2;

private:

	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;
	iPoint origin;

};

#endif // __SCENE2_H__