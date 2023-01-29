#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Window.h"
#include "Physics.h"
#include "Item.h"
#include "GameOver.h"
#include "FadeToBlack.h"
#include "Pathfinding.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//if (config.child("player")) {
	//	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//	//player->parameters = config.child("player");
	//	player->parameters = app->LoadConfigPro().child("scene").child("player");
	//}



	//pugi::xml_node arma2 = config.child("faryarma2");
	//faryarma2texpath = arma2.attribute("texturepath").as_string();

	//pugi::xml_node logo = config.child("loguito");
	//loguitotexturePath = logo.attribute("texturepath").as_string();

	//pugi::xml_node intro = config.child("titulowo");
	//titulowotexturePath = intro.attribute("texturepath").as_string();

	//pugi::xml_node game_over = config.child("asmuerto");
	//asmuertotexturePath = game_over.attribute("texturepath").as_string();

	//pugi::xml_node win_screen = config.child("lenguado");
	//lenguadotexturePath = win_screen.attribute("texturepath").as_string();

	//pugi::xml_node bala = config.child("balagua");
	//auxTpath = bala.attribute("texturepath").as_string();

	//pugi::xml_node level_1_song = config.child("apatrullando");
	//apatrullando = level_1_song.attribute("audiopath").as_string();

	//pugi::xml_node silence_song = config.child("introcancion");
	//introcancion = silence_song.attribute("audiopath").as_string();

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	for (pugi::xml_node itemNode = app->LoadConfigPro().child("scene").child("enemy1"); itemNode; itemNode = itemNode.next_sibling("enemy1"))
	{
		Enemy1* enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::ENEMY1);
		enemy1->parameters = itemNode;
		Enemiaires.Add(enemy1);
	}

	for (pugi::xml_node itemNode = app->LoadConfigPro().child("scene").child("enemy2"); itemNode; itemNode = itemNode.next_sibling("enemy2"))
	{
		Enemy2* enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::ENEMY2);
		enemy2->parameters = itemNode;
		Enemisuelos.Add(enemy2);
	}

	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER); //SUSSY2 (un poco cambiado y en el Awake)
	player->parameters = app->LoadConfigPro().child("scene").child("player");
	app->entityManager->Enable();

	bool retLoad = app->map->Load();

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->WalkMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	SString title("Aventura en el Sa-Fary - Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	app->render->camera.x = 0;

	//pugi::xml_node Muni = config.child("Ui_BalasAwa");
	//MuniTP = Muni.attribute("texturepath").as_string();
	//TextMuni =

	faryarma1 = app->tex->Load("Assets/Textures/Farynator.png");
	faryarma2 = app->tex->Load("Assets/Textures/FarynatorBolteado.png");
	logoooo = app->tex->Load("Assets/Textures/loguito.jpg");
	titula = app->tex->Load("Assets/Textures/titulowo.jpg");
	moriste = app->tex->Load("Assets/Textures/asmuerto.jpg");
	winner = app->tex->Load("Assets/Textures/lenguado.jpg");
	TextMuni = app->tex->Load("Assets/Textures/Muniagua.png");

	uint w, h;
	app->win->GetWindowSize(w, h);
	button1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Button 1", { (int)w / 2 - 50,(int)h / 2 - 30, 100, 20 }, this);
	button2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Button 2", { (int)w / 2 - 50,(int)h / 2, 100, 20 }, this);

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	if (player->deathbool == true) {
		app->gameover->ganaste = false;
		//app->map->Disable();
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->gameover, 0);
		app->render->camera.x = 0;

		jugando = false;
		app->entityManager->DestroyEntity(player);
	}
	if (player->lenguado == true) {
		app->gameover->ganaste = false;
		//app->map->Disable();
		app->fadetoblack->StartFadeToBlack(this, (Module*)app->gameover, 0);
		app->render->camera.x = 0;
		jugando = false;
	}

	//SAVE GAME
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	//LOAD GAME
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	//Municion->Draw(MuniTP);

	//MOVER CAMARA (no sirve de nada pq está fijada a player position) SUSSY14
	//if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	app->render->camera.y += 1;

	//if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	app->render->camera.y -= 1;

	float speed = 0.2 * dt;
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);


	//Cambiar direccion de pistolo

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		direccion = false;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		direccion = true;


	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	else if (fadeo > 0.0f)
	{
		fadeo -= 0.02f;
	}
	else if (fadeo <= 0.0f)
	{
		fadeo = 0.0f;
		fadeando = false;
	}

	//MOVER CÁMARA CUANDO ESTÁ EN EL NIVEL
	if (jugando == 1) {

		app->render->camera.x = -player->position.x * app->win->GetScale() + 650;
		app->render->camera.y = -player->position.y * app->win->GetScale() + 600;

	}

	// Draw map
	app->map->Draw();

	app->guiManager->Draw();
	//pathfinding
	ListItem<PhysBody*>* enemyBodyItem = app->map->enemies.start;
	ListItem<Enemy2*>* Enemy2Item = Enemisuelos.start;

	while (enemyBodyItem != NULL && Enemy2Item != NULL) {
		if (enemyBodyItem->data->ctype != ColliderType::ENEMY2) {
			enemyBodyItem = enemyBodyItem->next;
		}
		else if (Enemy2Item->data->dead == true) {
			Enemy2Item = Enemy2Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
		else if (enemyBodyItem->data->body->IsActive() && enemyBodyItem->data->ctype == ColliderType::ENEMY2
			&& Enemy2Item->data->estado == Estado::CHASEMODE) {
			origin.x = enemyBodyItem->data->body->GetPosition().x;
			origin.y = enemyBodyItem->data->body->GetPosition().y;
			iPoint destination;
			destination.x = player->GetBody()->body->GetPosition().x;
			destination.y = player->GetBody()->body->GetPosition().y;
			app->pathfinding->ClearLastPath();
			app->pathfinding->CreatePath(origin, destination);
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				if (i == 1) {
					Enemy2Item->data->target.x = PIXEL_TO_METERS(pos.x);
					Enemy2Item->data->target.y = PIXEL_TO_METERS(pos.y);
				}

				if (app->physics->debug) app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
			// L12: Debug pathfinding didn't change the names cause xd
			iPoint originScreen = app->map->MapToWorld(destination.x, destination.y);
			if (app->physics->debug) app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
			Enemy2Item = Enemy2Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
		else {
			Enemy2Item = Enemy2Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}

	}
	enemyBodyItem = app->map->enemies.start;
	ListItem<Enemy1*>* Enemy1Item = Enemiaires.start;

	while (enemyBodyItem != NULL && Enemy1Item != NULL) {
		if (enemyBodyItem->data->ctype != ColliderType::ENEMY1) {
			enemyBodyItem = enemyBodyItem->next;
		}
		else if (Enemy1Item->data->dead == true) {
			Enemy1Item = Enemy1Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
		else if (enemyBodyItem->data->ctype == ColliderType::ENEMY1
			&& Enemy1Item->data->estado == Estado::CHASEMODE) {
			origin.x = enemyBodyItem->data->body->GetPosition().x;
			origin.y = enemyBodyItem->data->body->GetPosition().y;
			iPoint destination;
			destination.x = player->GetBody()->body->GetPosition().x;
			destination.y = player->GetBody()->body->GetPosition().y;
			app->pathfinding->ClearLastPath();
			app->pathfinding->CreatePath(origin, destination);
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				if (i == 1) {
					Enemy1Item->data->target.x = PIXEL_TO_METERS(pos.x);
					Enemy1Item->data->target.y = PIXEL_TO_METERS(pos.y);
				}

				if (app->physics->debug) app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
			// L12: Debug pathfinding didn't change the names cause xd
			iPoint originScreen = app->map->MapToWorld(destination.x, destination.y);
			if (app->physics->debug) app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
			Enemy1Item = Enemy1Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
		else {
			enemyBodyItem = enemyBodyItem->next;
			Enemy1Item = Enemy1Item->next;
		}
	}

	// L08: DONE 3: Test World to map method

	/*
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
												mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);
	}
	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
												mouseY - app->render->camera.y);
	}

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, mouseTile);
			originSelected = false;
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}

	// L12: Get the latest calculated path and draw
	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	*/

	if (player->numBalas > 25) {
		MuniPin = { 0, 0, 34, 75 };
	}
	if (player->numBalas > 20 && player->numBalas <= 25) {
		MuniPin = { 34, 0, 34, 75 };
	}
	if (player->numBalas > 15 && player->numBalas <= 20) {
		MuniPin = { 68, 0, 34, 75 };
	}
	if (player->numBalas > 10 && player->numBalas <= 15) {
		MuniPin = { 102, 0, 34, 75 };
	}
	if (player->numBalas > 5 && player->numBalas <= 10) {
		MuniPin = { 136, 0, 34, 75 };
	}
	if (player->numBalas > 0 && player->numBalas <= 5) {
		MuniPin = { 170, 0, 34, 75 };
	}
	if (player->numBalas == 0) {
		MuniPin = { 204, 0, 34, 75 };
	}

	app->render->DrawTexture(TextMuni, player->position.x - 600, player->position.y + 25, &MuniPin);

	return true;
}

bool Scene::LoadState(pugi::xml_node& data)
{
	player->MoveTo(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());
	player->numBalas = data.child("player").attribute("numbalasAgua").as_int();

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNode = data.append_child("player");

	playerNode.append_attribute("x") = player->position.x;
	playerNode.append_attribute("y") = player->position.y;
	playerNode.append_attribute("numbalasAgua") = player->numBalas; 

	return true;
}

void changeScale(bool s) {
	/*int s1 = 1;

	if (s == 0) {
		Window::SetScale(s1);
	}
	else {
		Window::SetScale(s1);
	}*/
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//playerpistola pium pium
	if (jugando == true && pistolon == true && direccion == true) {

		app->render->DrawTexture(faryarma1, player->position.x + 28, player->position.y - 10);
	}

	if (jugando == true && pistolon == true && direccion == false) {

		app->render->DrawTexture(faryarma2, player->position.x - 38, player->position.y - 10);
	}

	if (app->physics->debug)
	{
		ListItem<PhysBody*>* ItemListE = app->map->enemies.start;
		ListItem<Enemy2*>* Enemy2Item = Enemisuelos.start;
		ListItem<Enemy1*>* Enemy1Item = Enemiaires.start;
		PhysBody* ebody;
		PhysBody* pbody = player->GetBody();


		while (ItemListE != NULL)
		{
			ebody = ItemListE->data;

			if (ItemListE->data->body->IsActive() && Enemy2Item != NULL
				&& Enemy2Item->data->estado == Estado::CHASEMODE && !Enemy2Item->data->dead)
			{

				app->render->DrawLine(METERS_TO_PIXELS(ebody->body->GetPosition().x),
					METERS_TO_PIXELS(ebody->body->GetPosition().y),
					METERS_TO_PIXELS(pbody->body->GetPosition().x),
					METERS_TO_PIXELS(pbody->body->GetPosition().y),
					255, 0, 0);
				Enemy2Item = Enemy2Item->next;
			}
			if (ItemListE->data->body->IsActive() && Enemy1Item != NULL &&
				Enemy1Item->data->estado == Estado::CHASEMODE && !Enemy1Item->data->dead)
			{

				app->render->DrawLine(METERS_TO_PIXELS(ebody->body->GetPosition().x),
					METERS_TO_PIXELS(ebody->body->GetPosition().y),
					METERS_TO_PIXELS(pbody->body->GetPosition().x),
					METERS_TO_PIXELS(pbody->body->GetPosition().y),
					255, 0, 0);
				Enemy1Item = Enemy1Item->next;
			}
			ItemListE = ItemListE->next;
		}
	}

	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		break;
	case 2:
		LOG("Button 2 click");
		break;
	}

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	Enemisuelos.Clear();
	Enemiaires.Clear();
	app->tex->UnLoad(img);
	app->tex->UnLoad(faryarma1);
	app->tex->UnLoad(faryarma2);
	app->tex->UnLoad(mouseTileTex);
	app->tex->UnLoad(originTex);

	return true;
}
