#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "Enemy1.h"
#include "Enemy2.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	//position.x = parameters.attribute("x").as_int(); //SUSSY2
	//position.y = parameters.attribute("y").as_int();
	//texturePath = parameters.attribute("texturepath").as_string();
	//maxbalas = parameters.attribute("maxbalas").as_int();
	//numBalas = parameters.attribute("maxbalas").as_int();

	return true;
}

bool Player::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	maxbalas = parameters.attribute("maxbalas").as_int();
	numBalas = parameters.attribute("maxbalas").as_int();

	for (int i = 0; i < 50; i++) {
		Abalas[i] = NULL;
	}

	deathbool = false;

	//initilize textures
	texture = app->tex->Load(texturePath);
	TaBala = app->tex->Load("Assets/Textures/BALAGUA.png"); //SUSSY7

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x + 15, position.y + 15, 14, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;
	//pbody->body->SetLinearVelocity(b2Vec2(0, -GRAVITY_Y)); SUS


	return true;
}

bool Player::Update()
{
	/*printf("Camera X: %d, Camera Y: %d\n", app->render->camera.x, app->render->camera.y);
	printf("Pos X: %d, Pos Y: %d\n", position.x, position.y);
	printf("Pos X respecto camara: %d\n", position.x + app->render->camera.x);*/
	int speed = 10;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); //SUSSY7

	LOG("%d, %d", position.y, app->render->camera.y);

	if (deathbool == true) {
		//position.x = parameters.attribute("x1").as_int();
		//position.y = parameters.attribute("y1").as_int();

		b2Vec2 pos(position.x, position.y);
		pbody->body->SetTransform(PIXEL_TO_METERS(pos), 0);
		app->render->camera.x = 0;
		deathbool = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (remainingJumps > 0) {
			b2Vec2 jump(pbody->body->GetLinearVelocity().x, 0);
			pbody->body->SetLinearVelocity(jump);
			float impulse = pbody->body->GetMass() * (-9);
			pbody->body->ApplyLinearImpulse(b2Vec2(0, impulse), pbody->body->GetWorldCenter(), false);
			--remainingJumps;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		dir = IZQ;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		dir = PARAO;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		dir = DER;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		dir = PARAO;
	}

	if (pbody != nullptr) { //SUSSY2
		b2Vec2 vel = pbody->body->GetLinearVelocity();
		float vX = 0;
		float vY = 0;
		switch (dir)
		{
		case IZQ:  vX = -7; break;
		case PARAO:  vX = 0; vY = 0; break;
		case DER: vX = 7; break;
		}

		float Vfinal = vX - vel.x;
		float impulseX = pbody->body->GetMass() * Vfinal;
		pbody->body->ApplyLinearImpulse(b2Vec2(impulseX, 0), pbody->body->GetWorldCenter(), true);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 15;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15;
	}

	app->render->DrawTexture(texture, position.x, position.y);

	//Tp player to position
	if (tp.CanTP == 1) {

		b2Vec2 changeP = b2Vec2(PIXEL_TO_METERS(tp.x), PIXEL_TO_METERS(tp.y));
		pbody->body->SetTransform(changeP, 0);

		tp.CanTP = 0;
	}

	//BALAS
	
	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && balafuera < maxbalas) {
		Abalas[1000 - totalBalas] = app->physics->CreateCircle(position.x + 40, position.y + 10, 5, bodyType::DYNAMIC);
		Abalas[1000 - totalBalas]->ctype = ColliderType::BALAGUA;
		Abalas[1000 - totalBalas]->body->SetLinearVelocity(b2Vec2(75, 0));

		numBalas--;
		totalBalas--;
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && balafuera < maxbalas) {

		Abalas[1000 - totalBalas] = app->physics->CreateCircle(position.x - 35, position.y + 10, 5, bodyType::DYNAMIC);
		Abalas[1000 - totalBalas]->ctype = ColliderType::BALAGUA;
		Abalas[1000 - totalBalas]->body->SetLinearVelocity(b2Vec2(-75, 0));

		numBalas--;
		totalBalas--;
	}

	balafuera = maxbalas - numBalas;

	//TEXTURA BALAS
	b2Vec2 pos;
	for (int i = 0; i < 1000; i++) {
		if (Abalas[i] != NULL) {
			pos = b2Vec2(Abalas[i]->body->GetPosition());
			app->render->DrawTexture(TaBala, METERS_TO_PIXELS(pos.x) - 5, METERS_TO_PIXELS(pos.y) - 5);
		}
	}

	b2Vec2 locura = b2Vec2(-1000, 1000);
	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
		for (int i = 0; i < totalBalas; i++) {
			if (Abalas[i] != NULL) {
				Abalas[i]->body->SetTransform(locura, 0.0f);
			}
		}
	}

	//DESTRUIR BALAS CUANDO COLISIONAN
	/*for (int i = 0; i < numBalas; i++) {
		Abalas[i]->listener->OnCollision(Abalas[i], );
	}*/

	/*if (app->physics->destructBala == true) {

		if (Abalas[numBalas - 1] != NULL) {
			Abalas[numBalas - 1]->body->SetTransform(locura, 0.0f);
			app->physics->destructBala = false;
		}
	}*/

	//RECARGAR BALAS
	/*if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		numBalas = 100; 
	}*/

	printf("Balas: %d\n", numBalas);
	printf("Balas disparadas: %d\n", 1000 - totalBalas);

	return true;
}

bool Player::CleanUp()
{
	if (pbody != nullptr) {
		app->scene->jugando = false;
		pbody->body->GetWorld()->DestroyBody(pbody->body);
		pbody = NULL;
		app->tex->UnLoad(texture);
	}
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		remainingJumps = 2;
		break;
	case ColliderType::LENGUADO:
		LOG("Collision LENGUADO");
		lenguado = true;
		break;
	case ColliderType::ASMUERTO:
		LOG("Collision ASMUERTO");
		deathbool = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::BALAGUA:
		LOG("Collision BALAGUA");
		break;
	}
}

void Player::MoveTo(int x, int y) {

	tp.x = x;
	tp.y = y;
	tp.CanTP = true;
}

bool Player::GetWinState() {
	return lenguado;
}
bool Player::GetDeadState() {
	return deathbool;
}
PhysBody* Player::GetBody() {
	return pbody;
}