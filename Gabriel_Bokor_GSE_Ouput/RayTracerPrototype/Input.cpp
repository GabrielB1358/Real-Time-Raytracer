#include "Input.h"

Input::Input() : m_MouseMove{ glm::vec2(0) }, m_MouseCoords{ glm::vec2(0) }
{
}

Input::~Input()
{
}

void Input::Tick()
{
	m_PressedKeys.clear();
	m_ReleasedKeys.clear();

	m_MouseMove = glm::vec2(0);
}

void Input::ProcessInput(SDL_Event _e, glm::ivec2 _resolution)
{
	//Mouse Set
	static int mSet = 0;
	//Detected Mouse
	bool dM = false;


	//Keyboard Inputs
	if (_e.type == SDL_KEYDOWN)
	{
		if (std::find(m_Keycodes.begin(), m_Keycodes.end(), _e.key.keysym.sym) == m_Keycodes.end())
		{
			m_Keycodes.push_back(_e.key.keysym.sym);
			m_PressedKeys.push_back(_e.key.keysym.sym);
		}
	}
	else if (_e.type == SDL_KEYUP)
	{
		for (int i = 0; i < m_Keycodes.size(); i++)
		{
			if (m_Keycodes[i] == _e.key.keysym.sym)
			{
				m_Keycodes.erase(m_Keycodes.begin() + i);
				m_ReleasedKeys.push_back(_e.key.keysym.sym);
			}
		}
	}

	//Mouse Inputs
	if (_e.type == SDL_MOUSEMOTION)
	{
		dM = true;
		if(mSet > 0)
		{
			m_MouseCoords.x = _e.motion.x;
			m_MouseCoords.y = _e.motion.y;

			m_MouseMove.x += m_MouseCoords.x - (_resolution.x / 2);
			m_MouseMove.y += m_MouseCoords.y - (_resolution.y / 2);
		}
	}
	if (_e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (std::find(m_Keycodes.begin(), m_Keycodes.end(), _e.button.button) == m_Keycodes.end())
		{
			m_Keycodes.push_back(_e.button.button);
			m_PressedKeys.push_back(_e.button.button);
		}
	}
	else if (_e.type == SDL_MOUSEBUTTONUP)
	{
		for (int i = 0; i < m_Keycodes.size(); i++)
		{
			if (m_Keycodes[i] == _e.button.button)
			{
				m_Keycodes.erase(m_Keycodes.begin() + i);
				m_ReleasedKeys.push_back(_e.button.button);
			}
		}
	}

	if (dM && mSet == 0)
	{
		mSet = 1;
	}
}

bool Input::IsKey(KeyInputs _keycode)
{
	if (std::find(m_Keycodes.begin(), m_Keycodes.end(), _keycode) != m_Keycodes.end())
	{
		return true;
	}
	else
		return false;
}

bool Input::IsKeyDown(KeyInputs _keycode)
{
	if (std::find(m_PressedKeys.begin(), m_PressedKeys.end(), _keycode) != m_PressedKeys.end())
	{
		return true;
	}
	else
		return false;
}

bool Input::IsKeyUp(KeyInputs _keycode)
{
	if (std::find(m_ReleasedKeys.begin(), m_ReleasedKeys.end(), _keycode) != m_ReleasedKeys.end())
	{
		return true;
	}
	else
		return false;
}

glm::vec2 Input::GetMouseCoords()
{
	return m_MouseCoords;
}

glm::vec2 Input::GetMouseMove()
{
	return m_MouseMove;
}