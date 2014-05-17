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
	while(!buttons.empty()){
		delete buttons.back();
		buttons.pop_back();
	}
	delete titleScreen;

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

	SoundSetup();
	
	fmodResult = fmodSystem->createStream("../Resources/Sound/Title.wav", FMOD_DEFAULT, 0, &titleMusic);
	SoundErrorCheck(fmodResult);

	fmodResult = fmodSystem->playSound(FMOD_CHANNEL_FREE, titleMusic, true, &musicChannel);
	SoundErrorCheck(fmodResult);
	musicChannel->setMode(FMOD_LOOP_NORMAL);
	musicChannel->setPaused(false);

	fmodResult = fmodSystem->createStream("../Resources/Sound/Laser.wav", FMOD_DEFAULT, 0, &startEffect);
	SoundErrorCheck(fmodResult);

	fmodResult = fmodSystem->playSound(FMOD_CHANNEL_FREE, startEffect, true, &startChannel);
	SoundErrorCheck(fmodResult);
	startChannel->setMode(FMOD_LOOP_OFF);

	keyboard = new KeyboardInput();

	// Set up the main camera
	Camera::MainCamera = Camera(windowWidth, windowHeight);
	Camera::MainCamera.SetPosition(-60.0f, 100.0f, -60.0f);
	
	Camera::MainCamera.SetDirection(XMConvertToRadians(45.0f), XMConvertToRadians(30.0f));

	// Set up our main renderer
	renderer = new Renderer();

	// Set up the skybox
	skybox = new Skybox(MATERIAL_SKYBOX);

	DebugTimer::Start(L"TIME TAKEN TO CREATE GAMEOBJECTS");

	// for cycling through textures
	int flip = 0;
	// Create some game objects
	/*for(int i = 0; i < NUM_GO; i++){
		for(int j = 0; j < NUM_GO; j++){
			for(int k = 0; k < NUM_GO; k++){
				GameObject* g = new GameObject(MESH_FRIGATE, MATERIAL_FRIGATE);
				g->transform.SetPosition(i * 50.0f, j * 50.0f, k * 50.0f);
				g->transform.SetRotationalVelocity(RNG::randFloat(-2,2), RNG::randFloat(-2,2), 0.0f);
				gameobjects.push_back(g);
			}
		}
	}*/

	ship = new Ship(MESH_FRIGATE, MATERIAL_FRIGATE, keyboard);
	gameobjects.push_back(ship);

	
	Button * startButton = new Button(MESH_BUTTON, MATERIAL_START_BUTTON, XMFLOAT4(300.0f, 300.0f, 200.0f, 100.0f));
	startButton->Visible = true;
	buttons.push_back(startButton);
	Button * returnButton = new Button(MESH_BUTTON, MATERIAL_RETURN_BUTTON, XMFLOAT4(100.0f, 200.0f, 200.0f, 100.0f));
	buttons.push_back(returnButton);
	Button * quitButton = new Button(MESH_BUTTON, MATERIAL_QUIT_BUTTON, XMFLOAT4(100.0f, 400.0f, 200.0f, 100.0f));
	buttons.push_back(quitButton);
	titleScreen = new Button(MESH_BUTTON, MATERIAL_TITLE_SCREEN, XMFLOAT4(0.0f, 0.0f, 800.0f, 600.0f));
	
	//grid setup
	GameObject* grid = new GameObject(MESH_CUBE,MATERIAL_GRID);
	grid ->transform.SetScale(60,60,60);
	grid->transform.SetPosition(40,-60,40);
	gameobjects.push_back(grid);

	//enemies setup
	spawnCooldown = 5.0;
	numEnemies = 0;
	Enemy* enemy = new Enemy(MESH_INVADER,MATERIAL_INVADER, true);
	enemies[numEnemies] = enemy;

	DebugTimer::Stop();

	state = GAME_STATE_TITLE;
	pauseOptions.push_back("Resume");
	pauseOptions.push_back("Quit");
	pauseOption = 0;

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
	keyboard->Update(dt);

	if (state == GAME_STATE_TITLE)
	{
		musicChannel->setPaused(false);
		if (keyboard->GetKeyDown(VK_RETURN))
		{
			state = GAME_STATE_MAIN;
			startChannel->setPaused(false);
		}
		else if (keyboard->GetKeyDown(VK_ESCAPE))
		{
			exit(0);
		}
	}
	else if (state == GAME_STATE_MAIN)
	{
		musicChannel->setVolume(1.0f);
		float speed = 100.0f;
		if(keyboard->GetKey(VK_SHIFT))
		{
			speed *= 3.0f;
		}

		// Rotate camera with arrow keys
		if(keyboard->GetKey(VK_RIGHT)){
			Camera::MainCamera.Rotate(5.0f * dt, 0.0f);
		}
		if(keyboard->GetKey(VK_LEFT)){
			Camera::MainCamera.Rotate(-5.0f * dt, 0.0f);
		}
		if(keyboard->GetKey(VK_UP)){
			Camera::MainCamera.Rotate(0.0f, -5.0f * dt);
		}
		if(keyboard->GetKey(VK_DOWN)){
			Camera::MainCamera.Rotate(0.0f, 5.0f * dt);
		}

		if (keyboard->GetKeyDown(VK_ESCAPE))
		{
			state = GAME_STATE_PAUSE;
			//musicChannel->setPaused(true);
			musicChannel->setVolume(0.3f);

			buttons.at(BUTTON_RETURN)->Visible = true;
			buttons.at(BUTTON_QUIT)->Visible = true;
		}

		Camera::MainCamera.Update(dt);

		skybox->Update(dt);
		//Enemy spawning
		spawnCooldown-= dt;
		if(spawnCooldown < 0.0 && numEnemies < MAX_ENEMIES-1)
		{
			spawnCooldown = 5.0;
			numEnemies++;
			Enemy* enemy = new Enemy(MESH_INVADER,MATERIAL_INVADER, true);
			enemies[numEnemies] = enemy;
		}
		//Enemies update
		for(int i = 0; i <= numEnemies; i++)
		{
			enemies[i] ->Update(dt);

			for (int j = 0; j < enemies[i]->GetBullets().size(); j++)
			{
				if (ship->GetRow() == enemies[i]->GetBullets()[j]->GetRow() && ship->GetColumn() == enemies[i]->GetBullets()[j]->GetColumn())
				{
					if (enemies[i]->GetBullets()[j]->transform.PosY() <= ship->transform.PosY() + 50 && enemies[i]->GetBullets()[j]->transform.PosY() >= ship->transform.PosY() - 50)
					{
						ship->Collision(enemies[i]->GetBullets()[j]);
						enemies[i]->GetBullets()[j]->Collision();
					}
				}
			}

			for (int j = 0; j < ship->GetBullets().size(); j++)
			{
				if (enemies[i]->GetRow() == ship->GetBullets()[j]->GetRow() && enemies[i]->GetColumn() == ship->GetBullets()[j]->GetColumn())
				{
					if (ship->GetBullets()[j]->transform.PosY()  >= enemies[i]->transform.PosY() - 50 && ship->GetBullets()[j]->transform.PosY()  <= enemies[i]->transform.PosY() + 50)
					{
						ship->GetBullets()[j]->Collision();
						enemies[i]->setActive(false);
					}
				}
			}

			if(!enemies[i] ->getActive())
			{
				//damage the ship

				//destroy the enemy
				delete enemies[i];
				enemies[i] = nullptr;
				int k = i;
				for(int j = (i+1); j<=numEnemies; j++)
				{
					enemies[k]=enemies[j];
					enemies[j] = nullptr;
					k++;
				}
				numEnemies--;
			}
		}
		
		for(UINT i = 0; i < gameobjects.size(); i++){
			gameobjects[i]->Update(dt);
		}

		if (ship->IsDead())
		{
			state = GAME_STATE_LOSE;
		}
	}
	else if (state == GAME_STATE_PAUSE)
	{
		if (keyboard->GetKeyDown(VK_ESCAPE))
		{
			state = GAME_STATE_MAIN;
		}
		
		if (keyboard->GetKeyDown(VK_RETURN))
		{
			if (pauseOptions[pauseOption] == "Resume")
			{
				state = GAME_STATE_MAIN;
			}
			else if (pauseOptions[pauseOption] == "Quit")
			{
				exit(0);
			}
		}

		if (keyboard->GetKeyDown(VK_UP) || keyboard->GetKeyDown('W'))
		{
			pauseOption--;

			if (pauseOption < 0)
			{
				pauseOption = pauseOptions.size() - 1;
			}
		}
		
		if (keyboard->GetKeyDown(VK_DOWN) || keyboard->GetKeyDown('S'))
		{
			pauseOption++;
			
			if (pauseOption >= pauseOptions.size())
			{
				pauseOption = 0;
			}
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
		renderer->DrawButton(titleScreen);
	}
	else if (state == GAME_STATE_MAIN)
	{
		ship->Draw(renderer);
	}
	else if (state == GAME_STATE_LOSE)
	{
		renderer->DrawString("GAME OVER", 500.0f, 500.0f, 50.0f);
	}

	for(std::vector<Button*>::const_iterator itr = buttons.begin(); itr != buttons.end(); ++itr)
	{
		Button* b = *itr;
		if(b->Visible)
		{
			renderer->DrawButton(b);
		}
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

	POINT* p = new POINT();
	UINT width, height;
	Camera::MainCamera.GetScreenSize(width, height);
	p->x = (x * 800) / width;
	p->y = (y * 600) / height;

	for(std::vector<Button*>::const_iterator itr = buttons.begin(); itr != buttons.end(); ++itr)
	{
		Button* b = *itr;
		if(b->Visible && b->Clicked(p))
		{
			if(b == buttons.at(BUTTON_START))
			{
				b->Visible = false;
				state = GAME_STATE_MAIN;
			}
			else if(b == buttons.at(BUTTON_RETURN))
			{
				b->Visible = false;
				buttons.at(BUTTON_QUIT)->Visible = false;
				state = GAME_STATE_MAIN;
			}
			else if(b == buttons.at(BUTTON_QUIT))
			{
				exit(0);
			}
		}
	}

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

	//Camera::MainCamera.Rotate((float)-(prevMousePos.x - x)/ 100.0f, 0.0f);
	//Camera::MainCamera.Rotate(0.0f, (float)-(prevMousePos.y - y)/ 100.0f);

	prevMousePos.x = x;
	prevMousePos.y = y;
}

void DemoGame::OnMouseScroll(WPARAM whlState, int delta){
	//Camera::MainCamera.transform.Move(0, 0, delta / 10.0f);
}
#pragma endregion

void DemoGame::SoundErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void DemoGame::SoundSetup()
{
	fmodResult = FMOD::System_Create(&fmodSystem);
	SoundErrorCheck(fmodResult);

	fmodResult = fmodSystem->getVersion(&fmodVer);
	SoundErrorCheck(fmodResult);

	if (fmodVer < FMOD_VERSION)
	{
		printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", fmodVer, FMOD_VERSION);
		return;
	}

	fmodResult = fmodSystem->getNumDrivers(&numDrivers);
	SoundErrorCheck(fmodResult);

	if (numDrivers == 0)
	{
		fmodResult = fmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		SoundErrorCheck(fmodResult);
	}
	else
	{
		fmodResult = fmodSystem->getDriverCaps(0, &fmodCaps, 0, &speakerMode);
		SoundErrorCheck(fmodResult);

		fmodResult = fmodSystem->setSpeakerMode(speakerMode);
		SoundErrorCheck(fmodResult);

		if (fmodCaps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			fmodResult = fmodSystem->setDSPBufferSize(1024, 10);
			SoundErrorCheck(fmodResult);
		}

		fmodResult = fmodSystem->getDriverInfo(0, fmodName, 256, 0);
		SoundErrorCheck(fmodResult);

		if (strstr(fmodName, "SigmaTel"))
		{
			fmodResult = fmodSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			SoundErrorCheck(fmodResult);
		}
	}

	fmodResult = fmodSystem->init(100, FMOD_INIT_NORMAL, 0);
	if (fmodResult == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		fmodResult = fmodSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		SoundErrorCheck(fmodResult);
		
		fmodResult = fmodSystem->init(100, FMOD_INIT_NORMAL, 0);
	}
	SoundErrorCheck(fmodResult);
}