#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

struct tpPlayer {

	bool CanTP;
	int x;
	int y;
};


class Player : public Entity
{
public:

	Player();

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void MoveTo(int x, int y);

	int getPX() { return position.x; }
	int getPY() { return position.y; }

	bool GetWinState();
	bool GetDeadState();

	PhysBody* GetBody();

public:

	PhysBody* pbody;
	PhysBody* Abalas[1000];


	bool auxUp = false;
	bool jump = false;
	int auxjump = 0;

	//WIN / DEATH
	bool deathbool = false;
	bool lenguado = false; //cuidado doble;; ?

	tpPlayer tp;

	SDL_Texture* TaBala = nullptr;
	const char* TaBalatexturePath;

	int numBalas;
	int balafuera = 0;
	int maxbalas;
	int totalBalas = 1000;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture = nullptr;
	const char* texturePath;

	int remainingJumps;
	mov dir;

	// L07 DONE 5: Add physics to the player - declare a Physics body



};

#endif // __PLAYER_H__
