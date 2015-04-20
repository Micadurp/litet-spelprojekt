#include "GamePlay.h"


GamePlay::GamePlay()
{

}
GamePlay::~GamePlay()
{

}
void GamePlay::Shutdown()
{
	if (cube)
	{
		cube->Shutdown();
		delete cube;
		cube = 0;
	}
}

bool GamePlay::Initialize(ID3D11Device* device)
{

	renderer = new RenderManager(device);
	if (!renderer)
	{
		return false;
	}

	camera = new Camera();
	if (!camera)
	{
		return false;
	}

	cube = new Model();
	if (!cube)
	{
		return false;
	}	

	cube->Initialize(L"cube.obj", device);

	return true;
}

void GamePlay::Update()
{

}

void GamePlay::Render(ID3D11DeviceContext* deviceContext)
{
	renderer->SetShader(deviceContext);
	cube->Render(deviceContext);
	
}