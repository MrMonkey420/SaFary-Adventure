#ifndef __ENEMY2_H__
#define __ENEMY2_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

#define VENEM 2
#define JUMPIMPULSEENEMYUP -12

struct SDL_Texture;

class Enemy2 : public Entity
{
public:

	Enemy2();

	virtual ~Enemy2();

	bool Awake();

	bool Start();
	 
	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);
	bool GetDeadState();

public:

	bool dead;
	bool changedir;
	Estado estado;
	iPoint target;

private:

	mov moveState;
	PhysBody* ebody;
	iPoint playerPos;
	int remainingJumps;
	bool jump,jumpVel;
	iPoint position;

	SDL_Texture* texture;
	const char* texturePath;
};

#endif // __ENEMY2_H__
