/* PROJECT SETTINGS COMMENTS
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------*/

#include "DemoGame.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// Give our program a console for debugging
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
#endif

	// Make the game, initialize and run
	DemoGame game(hInstance);
	
	if( !game.Init() )
		return 0;
	
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

DemoGame::DemoGame(HINSTANCE hInstance) : DXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Demo DX11 Game";
	windowWidth = 800;
	windowHeight = 600;

	prevMousePos.x = -99999;
	prevMousePos.y = -99999;
}

DemoGame::~DemoGame()
{
	// Release all of the D3D stuff that's still hanging out
	while(!gameobjects.empty()){
		delete gameobjects.back();
		gameobjects.pop_back();
	}

	delete renderer;
	delete skybox;

	DeviceManager::Cleanup(DeviceManager::GetCurrentDevice());
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool DemoGame::Init()
{
	if( !DXGame::Init() )
		return false;

	// Set up the main camera
	Camera::MainCamera = Camera(windowWidth, windowHeight);

	// Set up our main renderer
	renderer = new Renderer();

	// Set up the skybox
	skybox = new Skybox(MATERIAL_SKYBOX);

	DebugTimer::Start(L"TIME TAKEN TO CREATE GAMEOBJECTS");

	// for cycling through textures
	int flip = 0;
	// Create some game objects
	for(int i = 0; i < NUM_GO; i++){
		for(int j = 0; j < NUM_GO; j++){
			for(int k = 0; k < NUM_GO; k++){
				GameObject* g = new GameObject(MESH_FRIGATE, MATERIAL_FRIGATE);
				g->transform.SetPosition(i * 50.0f, j * 50.0f, k * 50.0f);
				g->transform.SetRotationalVelocity(RNG::randFloat(-2,2), RNG::randFloat(-2,2), 0.0f);
				gameobjects.push_back(g);
			}
		}
	}

	DebugTimer::Stop();

	state = GAME_STATE_TITLE;

	return true;
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void DemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DXGame::OnResize();

	Camera::MainCamera.Resize(windowWidth, windowHeight);
}
#pragma endregion

#pragma region Game Loop

// Updates the local constant buffer and 
// push it to the buffer on the device
void DemoGame::UpdateScene(float dt)
{
	if(GetAsyncKeyState(VK_ESCAPE)){
		exit(0);
	}
	if (state == GAME_STATE_TITLE)
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			state = GAME_STATE_MAIN;
		}
	}
	else if (state == GAME_STATE_MAIN)
	{
		float speed = 100.0f;
		if(GetAsyncKeyState(VK_SHIFT)){
			speed *= 3.0f;
		}
		// Move camera with WASD
		if(GetAsyncKeyState('W'))
		{
			Camera::MainCamera.Move(CameraMovement::FORWARD, speed);
		}
		if(GetAsyncKeyState('S'))
		{
			Camera::MainCamera.Move(CameraMovement::BACKWARD, -speed);
		}
		if(GetAsyncKeyState('A'))
		{
			Camera::MainCamera.Move(CameraMovement::LEFT, -speed);
		}
		if(GetAsyncKeyState('D'))
		{
			Camera::MainCamera.Move(CameraMovement::RIGHT, speed);
		}

		// Rotate camera with arrow keys
		if(GetAsyncKeyState(VK_RIGHT)){
			Camera::MainCamera.Rotate(5.0f * dt, 0.0f);
		}
		if(GetAsyncKeyState(VK_LEFT)){
			Camera::MainCamera.Rotate(-5.0f * dt, 0.0f);
		}
		if(GetAsyncKeyState(VK_UP)){
			Camera::MainCamera.Rotate(0.0f, -5.0f * dt);
		}
		if(GetAsyncKeyState(VK_DOWN)){
			Camera::MainCamera.Rotate(0.0f, 5.0f * dt);
		}

		if(GetAsyncKeyState(VK_SPACE)){
			int numCubes = static_cast<int>(dt * 1000.0f);
			for(int i = 0; i < numCubes; i++){
				GameObject* go = new GameObject(MESH_CUBE, MATERIAL_DEFAULT);
				go->transform.SetScale(1.0f,1.0f,1.0f);
				go->transform.SetPosition(0,0,0);
				go->transform.SetVelocity(RNG::randFloat(-50,50), RNG::randFloat(-50,50), RNG::randFloat(-50,50));
				go->transform.SetRotationalVelocity(RNG::randFloat(-50,50), RNG::randFloat(-50,50), RNG::randFloat(-50,50));
				gameobjects.push_back(go);
			}
		}

		Camera::MainCamera.Update(dt);

		skybox->Update(dt);

		for(UINT i = 0; i < gameobjects.size(); i++){
			gameobjects[i]->Update(dt);
		}
	}
}

// Clear the screen, redraw everything, present
void DemoGame::DrawScene()
{
	// Clear the buffer
	deviceContext->ClearRenderTargetView(renderTargetView, Colors::Black);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	renderer->Draw();

	if (state == GAME_STATE_TITLE)
	{
		renderer->DrawString("Press Enter to Begin", 150, 600, 60, XMFLOAT4(1, 1, 1, 1));
	}

	// Present the buffer
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void DemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hMainWnd);
}

void DemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void DemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	if(prevMousePos.x == -99999 || prevMousePos.y == -99999){
		prevMousePos.x = x;
		prevMousePos.y = y;
	}

	Camera::MainCamera.Rotate((float)-(prevMousePos.x - x)/ 100.0f, 0.0f);
	Camera::MainCamera.Rotate(0.0f, (float)-(prevMousePos.y - y)/ 100.0f);

	prevMousePos.x = x;
	prevMousePos.y = y;
}

void DemoGame::OnMouseScroll(WPARAM whlState, int delta){
	//Camera::MainCamera.transform.Move(0, 0, delta / 10.0f);
}
#pragma endregion