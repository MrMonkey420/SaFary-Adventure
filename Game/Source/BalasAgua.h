#ifndef _BALASAGUA_H_
#define _BALASAGUA_H_

#include "Entity.h"


struct SDL_Texture;

class BalasAgua : public Entity {
public:

	BalasAgua();

	virtual ~BalasAgua();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	PhysBody* bala;

	iPoint pos;
	iPoint target;

	//b2Vec2 vel = NULL;

	SDL_Texture* texture;
	const char* texturePath;

}; 

#endif // !_BALASAGUA_H_
