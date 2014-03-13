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
}

DemoGame::~DemoGame()
{
	// Release all of the D3D stuff that's still hanging out
	while(!gameobjects.empty()){
		delete gameobjects.back();
		gameobjects.pop_back();
	}

	TextRenderer::Cleanup();
	delete renderer;
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
	// Move the camera away from the origin
	Camera::MainCamera.transform.SetPosition(0.0f, 0.0f, -5.0f);

	// Setup the text renderer
	TextRenderer::Setup();

	// Set up our main renderer
	renderer = new Renderer();

	DebugTimer::Start(L"TIME TAKEN TO CREATE GAMEOBJECTS");

	// for cycling through textures
	int flip = 0;
	// Create some game objects
	for(int i = 0; i < NUM_GO; i++){
		for(int j = 0; j < NUM_GO; j++){
			for(int k = 0; k < NUM_GO; k++){
				GameObject* g = new GameObject(MESH_CUBE, MATERIAL_DEFAULT);
				g->transform.SetPosition(i * 5.0f, j * 5.0f, k * 5.0f);
				gameobjects.push_back(g);
			}
		}
	}

	DebugTimer::Stop();

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
	if(GetAsyncKeyState(VK_ADD)){
		std::random_device rd;
		std::default_random_engine rnd(rd());
		std::uniform_int_distribution<int> uInt(-50, 50);
		GameObject* go = new GameObject();
		go->transform.SetPosition(uInt(rnd), uInt(rnd), uInt(rnd));
		gameobjects.push_back(go);
	}
	if(GetAsyncKeyState(VK_SUBTRACT)){
		if(gameobjects.size() > 0){
			delete gameobjects.back();
			gameobjects.pop_back();
		}
	}

	float speed = 10.0f;
	if(GetAsyncKeyState('W'))
		Camera::MainCamera.transform.Move(0, speed * dt, 0);
	if(GetAsyncKeyState('S'))
		Camera::MainCamera.transform.Move(0, -speed * dt, 0);
	if(GetAsyncKeyState('A'))
		Camera::MainCamera.transform.Move(-speed * dt, 0, 0);
	if(GetAsyncKeyState('D'))
		Camera::MainCamera.transform.Move(speed * dt, 0, 0);

	Camera::MainCamera.Update(dt);

	for(UINT i = 0; i < gameobjects.size(); i++){
		gameobjects[i]->Update(dt);
	}
}

// Clear the screen, redraw everything, present
void DemoGame::DrawScene()
{
	// Clear the buffer
	deviceContext->ClearRenderTargetView(renderTargetView, Colors::Black);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	renderer->Draw();

	TextRenderer::DrawString("!\"#$%&\\\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", 0,0,24, XMFLOAT4(1,0,0,1));
	TextRenderer::DrawString("\"The quick brown fox jumps over the lazy dog\"", 0,24,36, XMFLOAT4(0,1,0,1));
	TextRenderer::DrawString("!\"#$%&\\\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", 0,50,48, XMFLOAT4(0,0,1,1));

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
	prevMousePos.x = x;
	prevMousePos.y = y;
}

void DemoGame::OnMouseScroll(WPARAM whlState, int delta){
	Camera::MainCamera.transform.Move(0, 0, delta / 10.0f);
}
#pragma endregion