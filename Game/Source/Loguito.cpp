#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"
#include "Input.h"
#include "Loguito.h"
#include "Titulo.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Loguito::Loguito(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("loguito");
}

Loguito::~Loguito()
{

}

bool Loguito::Awake(pugi::xml_node& config)
{
	bool ret = true;

	/*pugi::xml_node logo = config.child("loguito");
	loguitotexturePath = logo.attribute("texturepath").as_string();*/

	/*pugi::xml_node logomusic = config.child("logofx");
	logofx = logomusic.attribute("audiopath").as_string();*/

	return ret;
}

// Load assets
bool Loguito::Start()
{
	bool ret = true;

	loguito = app->tex->Load("Assets/Textures/loguito.jpg");
	//loguito = app->tex->Load(loguitotexturePath);
	//logofx = app->audio->PlayMusic(logofx, 1.0f);
	//logofx = app->audio->LoadFx("Assets/Audio/Fx/logofx.wav");
	//logofx = app->audio->LoadFx("Assets/Audio/Fx/logofx.wav");
	app->audio->PlayMusic("Assets/Audio/Fx/logofx.wav", 2.0F);
	//app->audio->PlayMusic(logofx, 2.0F);

	//App->render->camera.x = 0;
	//App->render->camera.y = 0;

	//app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");
	//fxEnter = app->audio->LoadFx("Assets/audio/fx/enter.wav");

	return ret;
}

bool Loguito::PreUpdate()
{
	return true;
}

bool Loguito::Update(float dt)
{
	/*if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->fade->FadeToBlack(this, (Module*)App->sceneIntroGame, 90);
	}*/

	//app->render->Blit(loguito, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_UP) {
		return false;
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->audio->CleanUp();
		app->audio->Enable();
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->titulo, 0); // REVISAR cambiar 0 por 50? nosequehace
	}

	return true;
}

// Update: draw background
bool Loguito::PostUpdate()
{
	bool ret = true;
	// Draw everything --------------------------------------
	app->render->DrawTexture(loguito, 0, 0);
	//fxEnter = 0;

	//return Update_Status::UPDATE_CONTINUE; CUIDADO
	return ret;
}

bool Loguito::CleanUp()
{
	app->tex->UnLoad(loguito);
	//unload fx?
	return true;
}