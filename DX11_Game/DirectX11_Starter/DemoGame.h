#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>
#include "DXGame.h"
#include "GameObject.h"
#include "Camera.h"
#include "Renderer.h"
#include "Skybox.h"
#include <vector>
#include "Ship.h"

// For DirectX Math
using namespace DirectX;

enum GameState
{
	GAME_STATE_TITLE,
	GAME_STATE_MAIN,
	GAME_STATE_WIN,
	GAME_STATE_LOSE,
	GAME_STATE_PAUSE
};

// Demo class which extends the base DXGame class
class DemoGame : public DXGame
{
public:
	DemoGame(HINSTANCE hInstance);
	~DemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseScroll(WPARAM whlState, int delta);

private:
	static const int NUM_GO = 15;
	std::vector<GameObject*> gameobjects;

	Skybox* skybox;

	Renderer* renderer;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
	POINT prevMouseScroll;

	GameState state;

	std::vector<char*> pauseOptions;
	int pauseOption;
};