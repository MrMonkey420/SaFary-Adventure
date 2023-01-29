#ifndef __TITULO_H__
#define __TITULO_H__

#include "Module.h"
#include "Animation.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

class Titulo : public Module
{
public:
	Titulo(App* application, bool start_enabled);

	~Titulo();

	bool Start() override; //CUIDADO quitar overrides de todos si va mal

	bool Awake(pugi::xml_node& config);
	// Called when the module is activated
	// Loads the necessary textures for the map background

	bool PreUpdate() override;
	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp() override;

public:

	SDL_Texture* titulowo=nullptr;
	const char* titulowotexturePath;
	int introcancion;

	bool nuevapartida;
	bool continuar;
	bool salir;

	bool creditos;
	SDL_Texture* creditostex;

	GuiButton* botonnuevapartida;
	GuiButton* botoncontinuar;
	GuiButton* botonsettings;
	GuiButton* botoncreditos;
	GuiButton* botonsalir;
};

#endif