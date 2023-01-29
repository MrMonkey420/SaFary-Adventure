#include "Titulo.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Player.h"
#include "Scene.h"
#include "Log.h"
#include "Loguito.h"
#include "GameOver.h"
#include "EntityManager.h"
#include "GuiManager.h"

#include <stdio.h>

Titulo::Titulo(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("titulo");
}

Titulo::~Titulo()
{

}

bool Titulo::Awake(pugi::xml_node& config)
{
	bool ret = true;

	//pugi::xml_node intro = config.child("titulowo");
	//titulowotexturePath = intro.attribute("texturepath").as_string();

	//pugi::xml_node titlesong = config.child("espanacani");
	//introcancion = titlesong.attribute("audiopath").as_string();

	return ret;
}

// Load assets
bool Titulo::Start()
{
	bool ret = true;

	titulowo = app->tex->Load("Assets/Textures/titulowo.jpg");
	//app->audio->PlayMusic("Assets/audio/music/espanacani.ogg", 1.0f);
	introcancion = app->audio->PlayMusic("Assets/Audio/Music/titulocancion.wav");
	creditostex = app->tex->Load("Assets/Textures/nubedepegatina.png");
	//BUTTONS
	SDL_Rect playbounds{ 10250,190,129,33 };
	botonnuevapartida = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 1, "o", playbounds, this);
	playbounds = { 10190,240,129,33 };
	botoncontinuar = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "o", playbounds, this);
	playbounds = { 10190,290,129,33 };
	botonsettings = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 3, "o", playbounds, this);
	playbounds = { 10210,340,129,33 };
	botoncreditos = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 4, "o", playbounds, this);
	playbounds = { 10250,390,129,33 };
	botonsalir = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 5, "o", playbounds, this);

	nuevapartida = false;
	continuar = false;
	creditos = false;
	salir = false;
	return ret;
}

bool Titulo::PreUpdate()
{
	return true;
}

bool Titulo::Update(float dt)
{
	botonnuevapartida->Draw(app->render);
	botoncontinuar->Draw(app->render);
	botonsettings->Draw(app->render);
	botoncreditos->Draw(app->render);
	botonsalir->Draw(app->render);
	
	//app->render->Blit(botonnuevapartida, 0, 0);

	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->scene->jugando = true;
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->scene, 0); // REVISAR cambiar 0 por 50? nosequehace
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) //KEY DOWN O UP??? nosotros usamos el down
	{
		ret = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		app->gameover->ganaste = true;
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->gameover, 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		app->gameover->ganaste = false;
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->gameover, 0);
	}
		

	return ret;
}

// Update: draw background
bool Titulo::PostUpdate()
{
	bool ret = true;
	app->render->DrawTexture(titulowo,0,0);
	return ret;
}

bool Titulo::CleanUp()
{
	app->tex->UnLoad(titulowo);
	titulowo = nullptr;

	return true;
}