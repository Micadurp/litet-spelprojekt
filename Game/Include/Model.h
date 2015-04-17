#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <fstream>
#include <istream>
#include <sstream>

#include <iostream>
#include <vector>


class Model
{
protected:
	struct Vertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;

		Vertex()
		{
			position.x = 0;
			position.y = 0;
			position.z = 0;
			position.w = 0;

			texture.x = 0;
			texture.y = 0;

			normal.x = 0;
			normal.y = 0;
			normal.z = 0;
		}
		Vertex(float x, float y, float z, float U, float V, float nX, float nY, float nZ)
		{
			position.x = x;
			position.y = y;
			position.z = z;
			position.w = 0;

			texture.x = U;
			texture.y = V;

			normal.x = nX;
			normal.y = nY;
			normal.z = nZ;
		}
	};
	
	ID3D11Buffer* meshVertexBuff;
	ID3D11Buffer* meshIndexBuff;
	int indexCount;

	ID3D11ShaderResourceView* textureShaderResource;
	std::wstring textureName;

	int nrOfVertices;

	//change
	UINT32 vertexSize = sizeof(Vertex);

	ID3D11Buffer*   geometryShaderCB;
	D3D11_BUFFER_DESC geometryShaderDesc;

	D3D11_SUBRESOURCE_DATA geometryShaderData;
public:
	Model();
	~Model();


	void Initialize(std::wstring modelName, ID3D11Device* device);
	void Shutdown();

	virtual void Update();
	void Render(ID3D11DeviceContext* deviceContext);

	bool LoadObj(std::wstring filename, ID3D11Device* device);

	int GetIndexCount();
};
#endif