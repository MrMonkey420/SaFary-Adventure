#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	uint audioFxId;

	SDL_Texture* nueva_partida;
	SDL_Texture* playhover;
	SDL_Texture* playclick;
	SDL_Texture* continue_;
	SDL_Texture* continue_hover;
	SDL_Texture* continue_click;
	SDL_Texture* settings;
	SDL_Texture* settingshover;
	SDL_Texture* settingsclick;
	SDL_Texture* credits;
	SDL_Texture* creditshover;
	SDL_Texture* creditsclick;
	SDL_Texture* exit;
	SDL_Texture* exithover;
	SDL_Texture* exitclick;
		
};

#endif // __GUIBUTTON_H__
