#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window(App* application, bool start_enabled = true);

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* title);

	//ni idea de nada
	void ToggleFullscreen();

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// anchurita
	int GetWidth() const;

	// alturita
	int GetHeight() const;

	// Retrieve window scale
	uint GetScale() const;

	uint SetScale(int s);

public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;

private:
	SString title;
	uint width;
	uint height;
	uint scale;
};

#endif // __WINDOW_H__