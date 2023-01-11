#include "BalasAgua.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Player.h"
#include "Point.h"
#include "Physics.h"
#include "Scene2.h"
#include "Map.h"
#include "Map2.h"


BalasAgua::BalasAgua() : Entity(EntityType::BALA)
{
	name.Create("BalaAgua");
}

BalasAgua::~BalasAgua()
{

}

bool BalasAgua::Awake() {

	//L02: DONE 5: Get Player parameters from XML
	return true;
}

bool BalasAgua::Start() {

	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	bala = app->physics->CreateCircle(position.x + 16, position.y + 16, 5, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	bala->listener = this;

	// L07 DONE 7: Assign collider type
	bala->ctype = ColliderType::BALAGUA;
	
	//app->map->enemies.Add(ebody);


	return true;
}

bool BalasAgua::Update()
{


	return true;
}

bool BalasAgua::CleanUp()
{
	if (bala != nullptr) {
		bala->body->GetWorld()->DestroyBody(bala->body);
		bala = NULL;
		app->tex->UnLoad(texture);
	}
	return true;
}

void BalasAgua::OnCollision(PhysBody* physA, PhysBody* physB) {

	iPoint physbpos;
	physB->GetPosition(physbpos.x, physbpos.y);

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		CleanUp();
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::ASMUERTO:
		LOG("Collision DEATH");
		break;
	case ColliderType::LENGUADO:
		LOG("Collision WIN");
		break;
	case ColliderType::PLAYER:
		LOG("Collision player");
		break;
	case ColliderType::ENEMY1:
		LOG("Collision Enmigo Volador");
		break;
	case ColliderType::ENEMY2:
		LOG("Collision Enemigo Suelador");
		break;
	}

}