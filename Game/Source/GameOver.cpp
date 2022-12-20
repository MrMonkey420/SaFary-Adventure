#include "GameOver.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Player.h"
#include "EntityManager.h"
#include "Log.h"
#include "EntityManager.h"
#include <stdio.h>

GameOver::GameOver(App* application, bool start_enabled) : Module(application, start_enabled)
{

}

GameOver::~GameOver()
{

}

// Load assets
bool GameOver::Start()
{
	bool ret = true;
	texturepath2 = app->LoadConfigPro().child("GameOverWin").attribute("texturepath").as_string();
	asganado = app->tex->Load(texturepath2);

	texturepath = app->LoadConfigPro().child("GameOverLose").attribute("texturepath2").as_string();
	asperdido = app->tex->Load(texturepath);

	if (ganaste == true)
	{
		asganado = app->tex->Load("Assets/Textures/win.jpg");
	}
	
	if (ganaste==false)
	{
		asperdido = app->tex->Load("Assets/Textures/prueba.jpg");
	}

	app->render->camera.x = 0;

	return ret;
}

bool GameOver::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		ret = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->titulo, 0); // REVISAR cambiar 0 por 50? nosequehace
	}

	return ret;
}

bool GameOver::PostUpdate()
{
	if (ganaste == true)
	{
		app->render->DrawTexture(asganado, 0, 0);
	}

	if (ganaste == false)
	{
		app->render->DrawTexture(asperdido, 0, 0);
	}

	return true;
}

bool GameOver::CleanUp()
{
	app->tex->UnLoad(asganado);
	asganado = nullptr;
	app->tex->UnLoad(asperdido);
	asperdido = nullptr;
	//App->textures->Unload(bgTexture);
	//App->fonts->UnLoad(introFont);
	//bgTexture = nullptr;
	return true;
}