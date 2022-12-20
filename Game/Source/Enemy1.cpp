#include "Enemy1.h"
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


Enemy1::Enemy1() : Entity(EntityType::ENEMY1)
{
	name.Create("Enemy1");
}

Enemy1::~Enemy1()
{

}

bool Enemy1::Awake() {

	//L02: DONE 5: Get Player parameters from XML
	return true;
}

bool Enemy1::Start() {

	position.x = parameters.attribute("posx").as_int();
	position.y = parameters.attribute("posy").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
	dead = false;

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 15, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	ebody->listener = this;

	// L07 DONE 7: Assign collider type
	ebody->ctype = ColliderType::ENEMY1;
	app->map->enemies.Add(ebody);

	typeOfMovement = false;

	return true;
}

bool Enemy1::Update()
{
	playerPos = app->scene->player->position;

	if (dead == true) {
			position.x = parameters.attribute("x1").as_int();
			position.y = parameters.attribute("y1").as_int();

		b2Vec2 pos(position.x, position.y);
		ebody->body->SetTransform(PIXEL_TO_METERS(pos), 0);
		app->render->camera.x = 0;
		dead = false;
	}

	if (playerPos.x - position.x > NOTCHILLDISTANCE || playerPos.x - position.x < -NOTCHILLDISTANCE) {
		estado = Estado::NOPROBLEM;
	}
	else {
		estado = Estado::CHASEMODE;
	}

	b2Vec2 vel(0, 0);
	ebody->body->SetGravityScale(0);

	if (estado == Estado::NOPROBLEM) {
		ebody->body->SetLinearVelocity(vel);
	}
	else if (estado == Estado::CHASEMODE) {

		if (target.x > (int)ebody->body->GetPosition().x) {
			moveStateX = DER;
			vel.x = 4;
		}
		else if (target.x < (int)ebody->body->GetPosition().x) {
			moveStateX = IZQ;
			vel.x = -4;
		}

		if (target.y > (int)ebody->body->GetPosition().y) {
			moveStateY = ABJ;
			vel.y = 4;
		}
		else if (target.y < (int)ebody->body->GetPosition().y) {
			moveStateY = ARR;
			vel.y = -4;
		}
		if (typeOfMovement) {
			ebody->body->SetLinearVelocity(vel);
		}
		else {
			b2Vec2 vel = ebody->body->GetLinearVelocity();
			float desiredVelx = 0;
			float desiredVely = 0;
			switch (moveStateX)
			{
			case IZQ:  desiredVelx = -4; break;
			case PARAO:  desiredVelx = 0; break;
			case DER: desiredVelx = 4; break;
			}
			float velChangex = desiredVelx - vel.x;
			float impulsex = ebody->body->GetMass() * velChangex; //disregard time factor
			switch (moveStateY)
			{
			case ABJ: desiredVely = 4; break;
			case PARAO: desiredVely = 0; break;
			case ARR: desiredVely = -4; break;
			}
			float velChangey = desiredVely - vel.y;
			float impulsey = ebody->body->GetMass() * velChangey;
			ebody->body->ApplyLinearImpulse(b2Vec2(impulsex, impulsey), ebody->body->GetWorldCenter(), true);
		}
	}

	position.x = METERS_TO_PIXELS(ebody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(ebody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		typeOfMovement = !typeOfMovement;
	} 

	if (dead == true) {
		ebody->body->SetActive(false);
		this->Disable();
	}
	return true;
}

bool Enemy1::CleanUp()
{
	app->tex->UnLoad(texture);
	return true;
}

void Enemy1::OnCollision(PhysBody* physA, PhysBody* physB) {

	iPoint physbpos;
	physB->GetPosition(physbpos.x, physbpos.y);

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
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

		int playerY, enemyY;

		playerY = METERS_TO_PIXELS(physB->body->GetPosition().y);
		enemyY = METERS_TO_PIXELS(physA->body->GetPosition().y);

		if (enemyY > playerY + physB->height - 2) {
			dead = true;
		}
		else {
			app->scene->player->deathbool = true;
		}
		break;
	}
	
}


bool Enemy1::LoadState(pugi::xml_node& data)
{
		position.x = parameters.attribute("x1").as_int();
		position.y = parameters.attribute("y1").as_int();
	b2Vec2 pos(position.x, position.y);
	ebody->body->SetTransform(PIXEL_TO_METERS(pos), 0);

	return true;
}

bool Enemy1::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

		play.append_attribute("x1") = position.x;
		play.append_attribute("y1") = position.y;

	return true;
}

//bool Enemy1::GetWinState() {
//	return win;
//}
//bool Enemy1::GetDeadState() {
//	return isdead;
//}
