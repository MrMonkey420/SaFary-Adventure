#ifndef __LOGUITO_H__
#define __LOGUITO_H__

#include "Module.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

class Loguito : public Module
{
public:
	Loguito(App* application, bool start_enabled);

	// REVISAR (era un destructor virtual)
	~Loguito();

	bool Awake(pugi::xml_node& config);
	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override; //CUIDADO quitar overrides de todos si va mal

	bool PreUpdate() override;
	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool CleanUp() override;

public:
	//SDL_Texture* logo = nullptr;
	SDL_Texture* loguito = nullptr;
	const char* loguitotexturePath;

	int logofx; //cancion

private:

	//uint fxEnter; //CUIDADO para el audio
};

#endif