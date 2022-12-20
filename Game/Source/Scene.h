#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Enemy1.h"
#include "Enemy2.h"


#define CAMERASPEED 13

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(App* application, bool start_enabled);

	// Destructor
	virtual ~Scene();

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

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	//void changeScale(pugi::xml_node&, bool s);

	void setScene(bool s) {
		escena = s;
	}

	List<Enemy1*> Enemiaires;
	List<Enemy2*> Enemisuelos;

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;

	bool fadeando = false;

	SDL_Texture* logoooo;
	const char* loguitotexturePath;

	SDL_Texture* titula;
	const char* titulowotexturePath;

	SDL_Texture* moriste;
	const char* asmuertotexturePath;

	SDL_Texture* winner;
	const char* lenguadotexturePath;

	const char* introcancion; //introcancion

	const char* apatrullando; //cancion


	SDL_Texture* faryarma1;
	const char* faryarma1texpath;
	SDL_Texture* faryarma2;
	const char* faryarma2texpath;

	bool level1;

	//bool godMode; //CUIDADO QUITAR

	bool escena = 0;
	bool jugando = 0;

private:
	SDL_Texture* img;
	float fadeo = 0.0f;

	
	bool pistolon = true;
	bool direccion = true;

	const char* auxTpath = NULL;

	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;
	iPoint origin;

};

#endif // __SCENE_H__