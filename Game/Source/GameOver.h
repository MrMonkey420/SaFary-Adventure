#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class GameOver : public Module
{
public:

	GameOver(App* application, bool start_enabled);

	~GameOver();

	bool Start() override;

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp() override;

public:

	bool ganaste;

	SDL_Texture* asganado = nullptr;
	const char* texturepath;
	int winfx;

	SDL_Texture* asperdido = nullptr;
	const char* texturepath2;
	int losefx;

private:
};

#endif


