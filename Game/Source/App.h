#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Loguito;
class Titulo;
class Scene;
class Scene2;
class GameOver;
class EntityManager;
class Map;
class Map2;
class Physics;
class PathFinding;
class Animation;
class FadeToBlack;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGameRequest();
	void SaveGameRequest();
	bool LoadFromFile();
	bool SaveToFile() ;
	pugi::xml_node LoadConfigPro();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	//bool LoadGame();
	//bool SaveGame() const; //CUIDADO

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Loguito* loguito;
	Titulo* titulo;
	Scene* scene;
	Scene2* scene2;
	GameOver* gameover;
	EntityManager* entityManager;
	Map* map;
	Map2* map2;
	FadeToBlack* fadetoblack;
	Physics* physics;
	PathFinding* pathfinding;
	Animation* animation;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

	// L03: DONE 1: Create control variables to control that the real Load and Save happens at the end of the frame
    bool saveGameRequested;
	bool loadGameRequested;
};

extern App* app;

#endif	// __APP_H__