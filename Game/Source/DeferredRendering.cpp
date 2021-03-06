#include"DeferredRendering.h"

DeferredRendering::DeferredRendering()
{
	meshVertBuff = nullptr;
	indexBuffer = nullptr;
	renderTargetView = nullptr;
	shaderResourceView = nullptr;
	clearRV = nullptr;
	deferredPixelShader = nullptr;
	deferredCB = nullptr;
}

DeferredRendering::~DeferredRendering()
{

}

void DeferredRendering::Initilize(ID3D11Device* _device, const DirectX::XMMATRIX &_projectionMatrix, int _screenWidth, int _screenHeight)
{
	nrOfRenderTargets = 3;
	vertexSize = sizeof(Vertex);

#pragma region Create Vertex buffer
	int nrOfVertices = 0;

	Vertex vertices[4];
	

	vertices[nrOfVertices++] = Vertex(-1.0f, 1.0f, 0, 0.0f, 0.0f); //1

	vertices[nrOfVertices++] = Vertex(1.0f, 1.0f, 0, 1.0f, 0.0f); //2

	vertices[nrOfVertices++] = Vertex(-1.0f, -1.0f, 0, 0.0f, 1.0f); //3

	vertices[nrOfVertices++] = Vertex(1.0f, -1.0f, 0, 1.0f, 1.0f); //4


	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;
	_device->CreateBuffer(&bufferDesc, &data, &meshVertBuff);

#pragma endregion

	ID3D11Texture2D** renderTargetTextureMap = new ID3D11Texture2D*[nrOfRenderTargets];

	renderTargetView = new ID3D11RenderTargetView*[nrOfRenderTargets];
	shaderResourceView = new ID3D11ShaderResourceView*[nrOfRenderTargets];

	clearRV = new ID3D11ShaderResourceView*[nrOfRenderTargets];

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = _screenWidth;
	textureDesc.Height = _screenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	for (int n = 0; n < nrOfRenderTargets; n++)
	{
		_device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap[n]);
		clearRV[n] = NULL;
	}

	/////////////////////// Map's Render Target
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	for (int n = 0; n < nrOfRenderTargets; n++)
		_device->CreateRenderTargetView(renderTargetTextureMap[n], &renderTargetViewDesc, &renderTargetView[n]);


	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shacder resource view.
	for (int n = 0; n < nrOfRenderTargets; n++ )
		_device->CreateShaderResourceView(renderTargetTextureMap[n], &shaderResourceViewDesc, &shaderResourceView[n]);


	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(L"DeferredPixelShader.hlsl", NULL, nullptr, "PS_main", "ps_5_0", 0, NULL, &pPS, nullptr);

	_device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &deferredPixelShader);
	pPS->Release();

	// Fill in a buffer description

	for (int n = 0; n < nrOfRenderTargets; n++)
		renderTargetTextureMap[n]->Release();
	delete[] renderTargetTextureMap;
	renderTargetTextureMap = 0;

#pragma region Create ConstantBuffers

	DirectX::XMStoreFloat4x4(&vertexCB.viewProjection, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&vertexCB.world, DirectX::XMMatrixIdentity());

	D3D11_BUFFER_DESC matrixBufferDesc;
	memset(&matrixBufferDesc, 0, sizeof(matrixBufferDesc));

	matrixBufferDesc.ByteWidth = sizeof(VertexCB);
	matrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vsInitData;
	vsInitData.pSysMem = &vertexCB;
	vsInitData.SysMemPitch = 0;
	vsInitData.SysMemSlicePitch = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	_device->CreateBuffer(&matrixBufferDesc, &vsInitData, &deferredCB);

#pragma endregion
}

void DeferredRendering::Shutdown()
{	
	if (meshVertBuff)
	{
		meshVertBuff->Release();
		meshVertBuff = 0;
	}

	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	if (renderTargetView)
	{
		for (int i = 0; i < nrOfRenderTargets; i++)
		{
			renderTargetView[i]->Release();
		}
		delete[] renderTargetView;
		renderTargetView = 0;
	}

	if (shaderResourceView)
	{
		for (int i = 0; i < nrOfRenderTargets; i++)
		{
			shaderResourceView[i]->Release();
		}
		delete[] shaderResourceView;
		shaderResourceView = 0;
	}

	if (clearRV)
	{
		//for (int i = 0; i < nrOfRenderTargets; i++)
		//{
		//	clearRV[i]->Release();
		//}
		delete[] clearRV;
		clearRV = 0;
	}

	if (deferredPixelShader)
	{
		deferredPixelShader->Release();
		deferredPixelShader = 0;
	}

	if (deferredCB)
	{
		deferredCB->Release();
		deferredCB = 0;
	}
}

void DeferredRendering::FirstPass(ID3D11DeviceContext *_deviceContext, ID3D11DepthStencilView* _depthStencilView)
{

	float clearColor[] = { 0, 0, 0.3f, 0 };



	// Set our maps Render Target
	_deviceContext->PSSetShaderResources(0, nrOfRenderTargets, clearRV);
	_deviceContext->OMSetRenderTargets(nrOfRenderTargets, renderTargetView, _depthStencilView);

	// Now clear the render target
	for (int n = 0; n < nrOfRenderTargets; n++)
		_deviceContext->ClearRenderTargetView(renderTargetView[n], clearColor);
}

void DeferredRendering::Render(ID3D11DeviceContext * _deviceContext, ID3D11DepthStencilView* _depthStencilView, ID3D11RenderTargetView* _backbufferRTV)
{

	
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	_deviceContext->GSSetShader(NULL, NULL, 0);
	_deviceContext->PSSetShader(deferredPixelShader, nullptr, 0);

	//_deviceContext->IASetInputLayout(deferredVertexLayout);

	//_deviceContext->VSSetShader(deferredVertexShader, nullptr, 0);

	_deviceContext->GSSetShader(NULL, NULL, 0);
	_deviceContext->PSSetShader(deferredPixelShader, nullptr, 0);
	_deviceContext->PSSetSamplers(0, 0, NULL);

	_deviceContext->OMSetRenderTargets(1, &_backbufferRTV, _depthStencilView);

	UINT32 offset = 0;

	_deviceContext->VSSetConstantBuffers(0, 1, &deferredCB);

	_deviceContext->IASetVertexBuffers(0, 1, &meshVertBuff, &vertexSize, &offset);

	_deviceContext->PSSetShaderResources(0, nrOfRenderTargets, shaderResourceView);

	_deviceContext->Draw(4, 0);
	
}

bool DeferredRendering::SetShaders(ID3D11DeviceContext* _deviceContext)
{
	_deviceContext->GSSetShader(NULL, NULL, 0);
	_deviceContext->PSSetShader(deferredPixelShader, nullptr, 0);

	return true;
}