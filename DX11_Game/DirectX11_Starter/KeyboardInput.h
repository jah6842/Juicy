#pragma once

#include <unordered_map>
#include <Windows.h>
using namespace std;

class KeyboardInput
{
public:
	KeyboardInput(void);
	~KeyboardInput(void);

	void Update(float dt);
	bool GetKeyDown(unsigned char key);
	bool GetKeyUp(unsigned char key);
	bool GetKey(unsigned char key);
private:
	unordered_map<unsigned char, pair<bool, bool>> keyStates;	// In the pair, first value is previous state, second value is current/new state
};

