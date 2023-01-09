#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Scene2.h"
#include "EntityManager.h"
#include "Map.h"
#include "Map2.h"
#include "Physics.h"
#include "Animation.h"
#include "FadeToBlack.h"
#include "Loguito.h"
#include "Titulo.h"
#include "GameOver.h"
#include "Pathfinding.h"


//#include "Timer.h"
//#include "PerfTimer.h"


#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new Input(this);
	win = new Window(this);
	render = new Render(this);
	tex = new Textures(this);
	audio = new Audio(this);
	physics = new Physics(this);
	pathfinding = new PathFinding(this);

	loguito = new Loguito(this, true); //CUIDADO CAMBIADO
	titulo = new Titulo(this, false);
	scene = new Scene(this, false);
	scene2 = new Scene2(this, false);
	gameover = new GameOver(this, false);

	entityManager = new EntityManager(this);
	map = new Map(this);
	map2 = new Map2(this);
	fadetoblack = new FadeToBlack(this);



	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(pathfinding);
	AddModule(loguito);
	AddModule(titulo);
	AddModule(scene);
	AddModule(scene2);
	AddModule(gameover);
	AddModule(entityManager);
	AddModule(map);
	AddModule(map2);
	AddModule(fadetoblack);
	// Render last to swap buffer
	AddModule(render);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	//module->Init(); REVISAR MODULE INIT
	modules.Add(module);
	
}

// Called before render is available
bool App::Awake()
{
	bool ret = false;

	// L01: DONE 3: Load config from XML
	ret = LoadConfig();

	if (ret == true)
	{
		title = configNode.child("app").child("title").child_value(); // L01: DONE 4: Read the title from the config file

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	titulo->active = false;
	scene->active = false;
	gameover->active = false;
	entityManager->active = false;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true)
		{
			ret = item->data->Start();
		}
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

pugi::xml_node App::LoadConfigPro() { //REVISAR

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if (parseResult) {
		configNode = configFile.child("config");
		LOG("config correctly");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return configNode;
}


// ---------------------------------------------
void App::PrepareUpdate()
{
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadFromFile();
	if (saveGameRequested == true) SaveToFile();
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		if (item->data->active == true)
		{
			pModule = item->data;
			ret = item->data->PreUpdate();
		}

		/*pModule = item->data; ESTA VERSIÓN NO VA MUY BIEN
		if (pModule->active == false)
		{
			continue;
		}
		ret = item->data->PreUpdate();*/
	}
	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		if (item->data->active == true)
		{
			pModule = item->data;

			ret = item->data->Update(dt);
		}

		/*pModule = item->data;
		if (pModule->active == false) {
			continue;
		}
		ret = item->data->Update(dt);*/
	}
	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		if (item->data->active == true)
		{
			pModule = item->data;
			ret = item->data->PostUpdate();
		}

		/*pModule = item->data;
		if (pModule->active == false) {
			continue;
		}
		ret = item->data->PostUpdate();*/
	}
	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadGameRequest()
{
	loadGameRequested = true;
}

void App::SaveGameRequest() 
{
	saveGameRequested = true;
}

bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// check https://pugixml.org/docs/quickstart.html#modify

bool App::SaveToFile() 
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}

	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}