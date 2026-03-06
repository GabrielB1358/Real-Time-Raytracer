#pragma once
#include "Headers.h"

enum KeyInputs {
	w = SDLK_w,
	a = SDLK_a,
	s = SDLK_s,
	d = SDLK_d,
	space = SDLK_SPACE,
	lshift = SDLK_LSHIFT,
	escape = SDLK_ESCAPE,
	lcontrol = SDLK_LCTRL,
	p = SDLK_p,
	g = SDLK_g,
	r = SDLK_r,
	t = SDLK_t,
	y = SDLK_y,
	u = SDLK_u,
	tab = SDLK_TAB
};//! Enumerator for all relevant SDL keyboard inputs

struct Input
{
	Input();//! Constructor for Input class
	~Input();//! Destructor for Input class

	void Tick();//! Tick function runs every frame. It clears input vectors and resets mouse movement data

	/*****************************************************************************//**
	*	This function proccesses all the received inputs every frame
	*
	*	It takes in an SDL_Event variable and processes the keyboard input into 3
	*	vectors, a pressed keys, released keys, and one for keys in use at all.
	*	Also processes any mouse input, keeping track of mouse coordinates and movement
	*	
	*	@param _e SDL event variable containing all inputs each frame
	*	@param _resolution contains the windows resolution for mouse movement bounds
	*********************************************************************************/
	void ProcessInput(SDL_Event _e, glm::ivec2 _resolution);

	bool IsKey(KeyInputs _keycode);//! Returns if key is in use at all
	bool IsKeyDown(KeyInputs _keycode);//! Returns if key is pressed down
	bool IsKeyUp(KeyInputs _keycode);//! Returns if key is released

	glm::vec2 GetMouseMove();
	glm::vec2 GetMouseCoords();

private:
	std::vector<int> m_Keycodes; //! Stores all keys in use
	std::vector<int> m_PressedKeys;//! Stores all keys currently being pressed
	std::vector<int> m_ReleasedKeys;//! Stores all keys having been released that frame

	glm::vec2 m_MouseMove;//! Stores mouse movement data
	glm::vec2 m_MouseCoords;//! Stores mouse coordinates
};