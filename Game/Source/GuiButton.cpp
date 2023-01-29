#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	nueva_partida = app->tex->Load("Assets/Textures/ui/nuevapartida.png");
	playhover = app->tex->Load("Assets/Textures/ui/playhover.png");
	playclick = app->tex->Load("Assets/Textures/ui/playclick.png");
	continue_ = app->tex->Load("Assets/Textures/ui/continue.png");
	continue_hover = app->tex->Load("Assets/Textures/ui/continuehover.png");
	continue_click = app->tex->Load("Assets/Textures/ui/continueclick.png");
	settings = app->tex->Load("Assets/Textures/ui/settings.png");
	settingshover = app->tex->Load("Assets/Textures/ui/settingshover.png");
	settingsclick = app->tex->Load("Assets/Textures/ui/settingsclick.png");
	credits = app->tex->Load("Assets/Textures/ui/creditos1.png");
	creditshover = app->tex->Load("Assets/Textures/ui/creditos2.png");
	creditsclick = app->tex->Load("Assets/Textures/ui/creditos3.png");
	exit = app->tex->Load("Assets/Textures/ui/exit.png");
	exithover = app->tex->Load("Assets/Textures/ui/exithover.png");
	exitclick = app->tex->Load("Assets/Textures/ui/exitclick.png");
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}




bool GuiButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	}

	//app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });

	return false;
}