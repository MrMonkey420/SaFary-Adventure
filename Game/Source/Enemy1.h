#ifndef __ENEMY1_H__
#define __ENEMY1_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Enemy1 : public Entity
{
public:

	Enemy1();

	virtual ~Enemy1();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);
	bool GetDeadState() { return dead; };

public:

	bool dead;
	Estado estado;
	iPoint target;

private:

	PhysBody* ebody;
	iPoint playerPos;
	iPoint bodyPos;
	mov moveStateX;
	mov moveStateY;
	bool typeOfMovement;
	iPoint position;

	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __ENEMY1_H__
