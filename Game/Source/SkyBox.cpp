#include"SkyBox.h"

SkyBox::SkyBox()
{

}

bool SkyBox::Initialize(std::wstring _modelName, ID3D11Device * _device)
{
	Model::Initialize(_modelName, _device);

	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(L"SkyBoxVertexShader.hlsl", NULL, nullptr, "VS_main", "vs_4_0", 0, NULL, &pVS, nullptr);

	HRESULT result  = _device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &skyVertexShader);
	pVS->Release();
	if (FAILED(result))
	{
		return false;
	}

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(L"SkyBoxPixelShader.hlsl", NULL, nullptr, "PS_main", "ps_4_0", 0, NULL, &pPS, nullptr);

	result = _device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &skyPixelShader);
	pPS->Release();
	if (FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


	// Create the depth stencil state.
	result = _device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

SkyBox::~SkyBox()
{

}
void SkyBox::Update(const Player& _player)
{
	XMMATRIX scale;
	XMMATRIX translation;

	scale = XMMatrixScaling(20, 20, 20);
	
	translation = DirectX::XMMatrixTranslationFromVector(XMLoadFloat4(&_player.GetPosition()));
	
	XMStoreFloat4x4(&objMatrix, scale * translation);
}

void SkyBox::Shutdown()
{
	Model::Shutdown();

	if (skyVertexShader)
	{
		skyVertexShader->Release();
		skyVertexShader = 0;
	}
	if (skyPixelShader)
	{
		skyPixelShader->Release();
		skyPixelShader = 0;
	}
	if (depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = 0;
	}
}

void SkyBox::Render(ID3D11DeviceContext* _deviceContext,ID3D11DepthStencilState * _depthState)
{
	_deviceContext->OMSetDepthStencilState(depthStencilState, 1);

 	_deviceContext->VSSetShader(skyVertexShader, NULL, 0);
	_deviceContext->GSSetShader(NULL, NULL, 0);
	_deviceContext->PSSetShader(skyPixelShader, NULL, 0);

	Model::Render(_deviceContext, _depthState);

	_deviceContext->OMSetDepthStencilState(_depthState, 1);

}
