#include "KeyboardInput.h"


KeyboardInput::KeyboardInput(void)
{
	// Keys handled.  Add as needed
	keyStates.emplace('W', make_pair(false, false));
	keyStates.emplace('A', make_pair(false, false));
	keyStates.emplace('S', make_pair(false, false));
	keyStates.emplace('D', make_pair(false, false));
	keyStates.emplace(VK_UP, make_pair(false, false));
	keyStates.emplace(VK_DOWN, make_pair(false, false));
	keyStates.emplace(VK_ESCAPE, make_pair(false, false));
}


KeyboardInput::~KeyboardInput(void)
{
}

void KeyboardInput::Update(float dt)
{
	for (auto itr = keyStates.begin(); itr != keyStates.end(); itr++)
	{
		itr->second.first = itr->second.second;

		itr->second.second = GetAsyncKeyState(itr->first);
	}
}

bool KeyboardInput::GetKey(unsigned char key)
{
	return keyStates[key].second;
}

bool KeyboardInput::GetKeyDown(unsigned char key)
{
	if (keyStates[key].second == true && keyStates[key].first == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool KeyboardInput::GetKeyUp(unsigned char key)
{
	if (keyStates[key].second == false && keyStates[key].first == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}