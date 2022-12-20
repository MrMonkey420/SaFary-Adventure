#include "FadeToBlack.h"
#include "Render.h"
#include "app.h"
#include "Window.h"
#include "Log.h"

#include "SDL/include/SDL_render.h"

FadeToBlack::FadeToBlack(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("FadeToBlack");
	if (app != nullptr) {
		uint width, height, scale;
		app->win->GetWindowSize(width, height);
		scale = app->win->GetScale();
		int w = width;
		int h = height;
		int s = scale;
		screenRect = { 0, 0, w * h * s }; //CUIDADO (vigilar)
	}
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

 bool FadeToBlack::Update(float dt)
{
	if (currentStep == Fade_Step::NONE)
	{
		 return true;
	}

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}
	return true;
}

bool FadeToBlack::PostUpdate()
{
	if (currentStep == Fade_Step::NONE)
	{
		return true;
	}

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (int)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::StartFadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
		ret = true;
	}

	return ret;
}