#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene2.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Map2.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Pathfinding.h"
#include "Scene.h"
#include "Defs.h"
#include "Log.h"


Scene2::Scene2(App* application, bool start_enabled) : Module(application, start_enabled)
{
	name.Create("scene2");
}

// Destructor
Scene2::~Scene2()
{}

// Called before render is available
bool Scene2::Awake(pugi::xml_node& config)
{
	//LOG("Loading Scene");
	//bool ret = true;

	//// iterate all objects in the scene
	//// Check https://pugixml.org/docs/quickstart.html#access
	//for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}

	////L02: DONE 3: Instantiate the player using the entity manager
	//player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	//player->parameters = config.child("player");

	//app->entityManager->Enable();
	//app->map->Enable();
	level2 = false;

	return true;
}

// Called before the first frame
bool Scene2::Start()
{

	LOG("Loading Scene2");

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	/*for (pugi::xml_node itemNode = app->LoadConfig2().child("scene").child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	for (pugi::xml_node itemNode = app->LoadConfigPro().child("scene2").child("Enemy2"); itemNode; itemNode = itemNode.next_sibling("groundenemy"))
	{
		Enemy2* enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::ENEMY2);
		enemy2->parameters = itemNode;
		Enemisuelos.Add(enemy2);
	}

	for (pugi::xml_node itemNode = app->LoadConfigPro().child("scene").child("Enemy1"); itemNode; itemNode = itemNode.next_sibling("Enemy1"))
	{
		Enemy1* enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::ENEMY1);
		enemy1->parameters = itemNode;
		Enemiaires.Add(enemy1);
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->LoadConfigPro().child("scene").child("player");
	app->entityManager->Enable();
	godmode = false;

	app->audio->PlayMusic("Assets/Audio/Music/song.ogg");
	mouseTileTex = app->tex->Load("Assets/Textures/path_square.png");
	originTex = app->tex->Load("Assets/Textures/x_square.png");

	// L03: DONE: Load map
	bool retLoad = app->map2->Load();
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map2->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map2->mapData.width,
		app->map2->mapData.height,
		app->map2->mapData.tileWidth,
		app->map2->mapData.tileHeight,
		app->map2->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	stopcamera = true;
	level2 = true;
	app->render->camera.x = 0;

	return true;
}

// Called each loop iteration
bool Scene2::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene2::Update(float dt)
{

	if (player->GetWinState() == true) {
		app->map2->Disable();
		app->fadetoblack->StartFadeToBlack((Module*)app->entityManager, (Module*)app->gameover, 20);
		app->render->camera.x = 0;
		level2 = false;
	}
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();


	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
		if (player->position.x <= 953 && player->position.x >= 943) {
			app->render->camera.x = 0;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		app->render->camera.y += CAMERASPEED;
		stopcamera = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		app->render->camera.y -= CAMERASPEED;
		stopcamera = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		app->render->camera.x += CAMERASPEED;
		stopcamera = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		app->render->camera.x -= CAMERASPEED;
		stopcamera = false;
	}


	//camera limits
	/*app->render->camera.y = 0;
	if (app->render->camera.x > 0) {
		app->render->camera.x = 0;
	}*/
	if (app->render->camera.x < -3340) {
		app->render->camera.x = -3340;
	}
	if (player->position.x >= 3340 && stopcamera == true) {
		app->render->camera.x = -3040;
	}

	if (player->position.x >= 300 && player->position.x < 3340 && stopcamera == true) {
		app->render->camera.x = -player->position.x + 300;
	}

	//godmode
	ListItem<PhysBody*>* colliderItem;
	colliderItem = app->map2->mapColliders.start;
	while (colliderItem != NULL)
	{
		if (godmode) {
			colliderItem->data->body->SetActive(false);

		}
		else {
			colliderItem->data->body->SetActive(true);
		}
		colliderItem = colliderItem->next;
	}

	// Draw map
	if (level2) {
		app->map2->Draw();
	}
	//pathfinding
	ListItem<PhysBody*>* enemyBodyItem = app->map2->enemies.start;
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
				iPoint pos = app->map2->MapToWorld(path->At(i)->x, path->At(i)->y);
				if (i == 1) {
					Enemy2Item->data->target.x = PIXEL_TO_METERS(pos.x);
					Enemy2Item->data->target.y = PIXEL_TO_METERS(pos.y);
				}

				if (app->physics->debug) app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
			}
			// L12: Debug pathfinding didn't change the names cause xd
			iPoint originScreen = app->map2->MapToWorld(destination.x, destination.y);
			if (app->physics->debug) app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
			Enemy2Item = Enemy2Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
		else
		{
			Enemy2Item = Enemy2Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
	}
	enemyBodyItem = app->map2->enemies.start;
	ListItem<Enemy1*>* Enemy1Item = Enemiaires.start;

	while (enemyBodyItem != NULL && Enemy1Item != NULL ) {
		if (enemyBodyItem->data->ctype != ColliderType::ENEMY1) {
			enemyBodyItem = enemyBodyItem->next;
		}
		else if (Enemy1Item->data->dead == true) {
			Enemy1Item = Enemy1Item->next;
			enemyBodyItem = enemyBodyItem->next;
		}
		else if (enemyBodyItem->data->ctype == ColliderType::ENEMY1) {

			if (Enemy1Item->data->estado = Estado::CHASEMODE) {
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
					iPoint pos = app->map2->MapToWorld(path->At(i)->x, path->At(i)->y);
					if (i == 1) {
						Enemy1Item->data->target.x = PIXEL_TO_METERS(pos.x);
						Enemy1Item->data->target.y = PIXEL_TO_METERS(pos.y);
					}

					if (app->physics->debug) app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
				}

				// L12: Debug pathfinding didn't change the names cause xd
				iPoint originScreen = app->map2->MapToWorld(destination.x, destination.y);
				if (app->physics->debug) app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
				Enemy1Item = Enemy1Item->next;
				enemyBodyItem = enemyBodyItem->next;
			}
		}
		else
		{
			enemyBodyItem = enemyBodyItem->next;
			Enemy1Item = Enemy1Item->next;
		}
	}
	
		
		
	
	return true;
}

// Called each loop iteration
bool Scene2::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	//debug of the path

	if (app->physics->debug )
	{
		ListItem<PhysBody*>* ItemListE = app->map2->enemies.start;
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
				Enemy1Item->data->estado == Estado::CHASEMODE && !Enemy2Item->data->dead)
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

// Called before quitting
bool Scene2::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
