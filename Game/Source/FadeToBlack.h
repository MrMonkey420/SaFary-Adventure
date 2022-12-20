#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

class FadeToBlack : public Module
{
public:
	FadeToBlack(App* application, bool start_enabled = true);

	~FadeToBlack();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool StartFadeToBlack(Module* toDisable, Module* toEnable, float frames = 60);

private:
	enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;

	int frameCount = 0; //con el uint32 da errores
	int maxFadeFrames = 0;

	SDL_Rect screenRect;

	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;
};

#endif //__MODULEFADETOBLACK_H__