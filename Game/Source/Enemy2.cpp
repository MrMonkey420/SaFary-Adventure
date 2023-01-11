#include "Enemy2.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Player.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Scene2.h"
#include "Map.h"
#include "Map2.h"


Enemy2::Enemy2() : Entity(EntityType::ENEMY2)
{
	name.Create("Enemy2");
}

Enemy2::~Enemy2()
{

}

bool Enemy2::Awake()
{
	return true;
}

bool Enemy2::Start() {
	position.x = parameters.attribute("posx").as_int();
	position.y = parameters.attribute("posy").as_int();

	texturePath = parameters.attribute("texturepath").as_string();
		
		dead = false;
		
	texture = app->tex->Load(texturePath);

	ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 15, bodyType::DYNAMIC);

	ebody->listener = this;

	ebody->ctype = ColliderType::ENEMY2;

	app->map->enemies.Add(ebody);

	remainingJumps = 1;

	return true;
}

bool Enemy2::Update()
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

	if (playerPos.x -  position.x > NOTCHILLDISTANCE || playerPos.x - position.x < -NOTCHILLDISTANCE) {
		estado = Estado::NOPROBLEM;
	}
	else {
		estado = Estado::CHASEMODE;
	}

	//enemy movement 
 	b2Vec2 vel(0,-GRAVITY_Y);
	if (estado == Estado::NOPROBLEM) {
		if (changedir == true) {
			vel.x = -VENEM;
		}
		else {
			vel.x = VENEM;
		}
		ebody->body->SetLinearVelocity(vel);
	}
	else if (estado == Estado::CHASEMODE) {
		if (abs(target.x + PIXEL_TO_METERS(app->map->mapData.tileWidth / 2) - ebody->body->GetPosition().x) <= PIXEL_TO_METERS(1) && jumpVel == false) {

			moveState = PARAO;
		}
		else if (target.x > (int)ebody->body->GetPosition().x) {
			moveState = DER;
		}
		else if (target.x  < (int)ebody->body->GetPosition().x) {
			moveState = IZQ;
		}
		b2Vec2 vel = ebody->body->GetLinearVelocity();
		float desiredVelx = 0;
		float desiredVely = 0;
		switch (moveState)
		{
		case IZQ:  if (jumpVel) { desiredVelx = -6; } else { desiredVelx = -4; }; break;
		case PARAO:  desiredVelx = 0; break;
		case DER: if (jumpVel) { desiredVelx = 6; }else { desiredVelx = 4; }; break;
		}
		float velChangex = desiredVelx - vel.x;
		float impulsex = ebody->body->GetMass() * velChangex; //disregard time factor
		ebody->body->ApplyLinearImpulse(b2Vec2(impulsex, 0), ebody->body->GetWorldCenter(), true);

		if (jump == true) {
			b2Vec2 xd(ebody->body->GetLinearVelocity().x, 0);
			ebody->body->SetLinearVelocity(xd);
			float impulsey = ebody->body->GetMass() * JUMPIMPULSEENEMYUP;
			ebody->body->ApplyLinearImpulse(b2Vec2(0, impulsey), ebody->body->GetWorldCenter(), false);
			jump = false;
		}
	}

	position.x = METERS_TO_PIXELS(ebody->body->GetTransform().p.x) - 24;
	position.y = METERS_TO_PIXELS(ebody->body->GetTransform().p.y) - 24;

	app->render->DrawTexture(texture, position.x, position.y);

	if (dead == true) {
		ebody->body->SetActive(false);
		this->Disable();
	}

	return true;
}

bool Enemy2::CleanUp()
{
	if (ebody != nullptr) {
		ebody->body->GetWorld()->DestroyBody(ebody->body);
		ebody = NULL;
		app->tex->UnLoad(texture);
	}
	return true;
}

void Enemy2::OnCollision(PhysBody* physA, PhysBody* physB) {

	iPoint physbpos;
	physB->GetPosition(physbpos.x, physbpos.y);

	switch (physB->ctype)
	{
	case ColliderType::BALAGUA:
		LOG("Collision BALAGUAAAAAA");
		CleanUp();
		break;
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
		dead = true;
		
		break;
	case ColliderType::LENGUADO:
		LOG("Collision WIN");
		
		break;
	/*case ColliderType::GROUND:
		LOG("Collision JUMPS RESTORED");
		remainingJumps = 1;
		jumpVel = false;
		break;*/
	case ColliderType::CDIRECCION:
		LOG("Collision ENEMY CHANGE DIR");
		if (estado == NOPROBLEM) {
			changedir = !changedir;
		}
		break;
	case ColliderType::SALTASALTA:
		LOG("Collision ENEMY CHANGE DIR");
		if (estado == CHASEMODE) {
			if (remainingJumps > 0) {
				--remainingJumps;
				jump = true;
				jumpVel = true;
			}
		}
		break;
	case ColliderType::PLAYER:

		int playerY, enemyY;

		playerY = METERS_TO_PIXELS(physB->body->GetPosition().y);
		enemyY = METERS_TO_PIXELS(physA->body->GetPosition().y);

		if (enemyY > playerY + physB->height -2) {
			dead = true;
		}
		else {
			app->scene->player->deathbool = true;
		}		
		break;
	}
}


bool Enemy2::LoadState(pugi::xml_node& data)
{
	position.x = parameters.attribute("x1").as_int();
	position.y = parameters.attribute("y1").as_int();
	b2Vec2 pos(position.x, position.y);
	ebody->body->SetTransform(PIXEL_TO_METERS(pos), 0);

	return true;
}

bool Enemy2::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

		play.append_attribute("x1") = position.x;
		play.append_attribute("y1") = position.y;

	return true;
}
bool Enemy2::GetDeadState() {
	return dead;
}
