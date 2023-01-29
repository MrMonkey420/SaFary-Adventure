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
	SDL_Texture* nuevapartidahover;
	SDL_Texture* nuevapartidahold;
	SDL_Texture* continuar;
	SDL_Texture* continuarhover;
	SDL_Texture* continuarhold;
	SDL_Texture* ajustes;
	SDL_Texture* ajusteshover;
	SDL_Texture* ajusteshold;
	SDL_Texture* creditos_;
	SDL_Texture* creditoshover;
	SDL_Texture* creditoshold;
	SDL_Texture* salir_;
	SDL_Texture* salirhover;
	SDL_Texture* salirhold;
		
};

#endif // __GUIBUTTON_H__
