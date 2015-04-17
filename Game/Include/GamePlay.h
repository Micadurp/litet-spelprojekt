#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "Direct3D.h"
#include "Model.h"
#include "RenderManager.h"

class GamePlay
{
private:
	RenderManager* renderer;
	Model* cube;
public:
	GamePlay();
	virtual ~GamePlay();

	bool Initialize(ID3D11Device* device);
	void Shutdown();

	void Update();
	void Render(ID3D11DeviceContext* deviceContext);
};

#endif