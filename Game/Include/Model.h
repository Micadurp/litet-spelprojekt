#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "DDSTextureLoader.h"
#include"Camera.h"

#include <fstream>
#include <istream>
#include <sstream>

#include <iostream>
#include <vector>

using namespace DirectX;

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
		Vertex(float _x, float _y, float _z, float _U, float _V, float _nX, float _nY, float _nZ)
		{
			position.x = _x;
			position.y = _y;
			position.z = _z;
			position.w = 0;

			texture.x = _U;
			texture.y = _V;

			normal.x = _nX;
			normal.y = _nY;
			normal.z = _nZ;
		}
	};

	struct Material
	{
		std::wstring matname = L"";
		ID3D11ShaderResourceView* texture = nullptr;
		float Ns; //Specular Power
		float Ni; //Optical Density, 0-10, jobbar med transparanta objekt
		float d; // Transparacy
		float Tr; // Transparacy igen anv�nds utav ett annat filformat
		DirectX::XMFLOAT3 Tf; //Transmission Filter, filtrerar ljus som passerar igenom transparanta objekt
		int illum; // Ljuss�ttnings tjofr�s , 0-10
		DirectX::XMFLOAT3 Ka; //Ambient Color, detta �r f�rgen p� ett objekt n�r inget ljus �r p� den
		DirectX::XMFLOAT3 Kd; //Diffuse map, kolla denna om objekt inte har texturer
		DirectX::XMFLOAT3 Ks; //Specular Color �r f�rgen som objektet f�r n�r ljus reflekteras emot den
		DirectX::XMFLOAT3 Ke;//Emissive color is the color of a surface when it "lights up". For example a lightbulb.
	} material;


	struct ModelMaterial
	{
		DWORD indexAmount;
		Material material;
	};
	
	ID3D11Buffer* meshVertexBuff;
	ID3D11Buffer* meshIndexBuff;
	int indexCount;

	ID3D11ShaderResourceView* textureShaderResource;
	ID3D11ShaderResourceView* normalShaderResource;

	std::vector<ModelMaterial> modelMats;


	int nrOfVertices;

	DirectX::XMFLOAT4X4 objMatrix;

	//change
	UINT32 vertexSize = sizeof(Vertex);

	ID3D11Buffer* pixelShaderMaterialCB;


	std::vector<XMFLOAT3> pickingPoints;
	std::vector<DWORD> pickingIndices;
public:
	Model();
	virtual ~Model();


	bool Initialize(std::wstring _modelName, ID3D11Device* _device, std::vector<XMFLOAT3> *collidableGeometryPositions, std::vector<DWORD> *collidableGeometryIndices);
	bool Initialize(std::wstring _modelName, ID3D11Device* _device, std::vector<XMFLOAT3> *collidableGeometryPositions, std::vector<DWORD> *collidableGeometryIndices, bool pickable);
	bool Initialize(std::wstring _modelName, ID3D11Device* _device);
	virtual void Shutdown();

	virtual void Update(Camera*);
	virtual void Render(ID3D11DeviceContext* _deviceContext, ID3D11DepthStencilState * _depthState);
	virtual void Model::Render(ID3D11DeviceContext* _deviceContext);

	bool LoadObj(std::wstring _filename, ID3D11Device* _device, std::vector<XMFLOAT3> *collidableGeometryPositions = nullptr, std::vector<DWORD> *collidableGeometryIndices = nullptr, bool pickable = false);

	bool CreateShaders(ID3D11Device* _device);

	int GetIndexCount();

	DirectX::XMMATRIX GetObjMatrix();
	void SetObjMatrix(const DirectX::XMMATRIX& _newMatrix);


	std::vector<XMFLOAT3>* GetPickingPoints();
	std::vector<DWORD>* GetPickingIndicies();
};
#endif