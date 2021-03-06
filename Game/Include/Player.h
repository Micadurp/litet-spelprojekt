#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "PlayerInputs.h"
#include "Model.h"
#include <cmath>
#include "DirectXCollision.h"
#include "HookString.h"

#include "Structs\HookShot.h"
#include "Structs\Triangle.h"
#include "Structs\Ray.h"
#include "Structs\CollisionPacket.h"

using namespace std;

class Player : public Camera
{
	private:
		//float m_jumpVelocity;
		//bool m_isJumping;

		XMFLOAT4 m_jumpPosition;

		Model* m_crosshair;
		Model* m_crosshair2;

		PlayerInputs* m_input;

		KeyboardStateStruct m_keyboard;
		KeyboardStateStruct m_lastKeyboard;
		MouseStateStruct m_mouse;

		XMFLOAT2 m_position;

		XMFLOAT4 m_defaultForward;
		XMFLOAT4 m_defaultRight;
		XMFLOAT4 m_currentForward;
		XMFLOAT4 m_currentRight;

		XMFLOAT3 m_gravity;
		XMFLOAT3 m_velocity;
		HookShot* m_hookshot;

		bool m_lastpick;
		bool m_onGround;
		bool m_lookAtCrystal;

		HookString * hookString;

	public:
		Player();
		virtual ~Player();

		bool Initialize(HWND &_wndHandle, HINSTANCE &_hInstance, ID3D11Device* _device);
		void Shutdown();

		int Update(double time, vector<XMFLOAT3> collidableGeometryPositions, std::vector<uint32_t> collidableGeometryIndices, vector<Model*> models);
		void Move(double time, vector<XMFLOAT3> collidableGeometryPositions, vector<uint32_t> collidableGeometryIndices);

		void Render(Direct3D * _direct3D);
		void RenderRope(Direct3D * _direct3D);

		XMFLOAT4X4 GetCrosshairMatrix();

		void ChangeHookState(vector<Model*> models, vector<XMFLOAT3> collidableGeometryPositions, std::vector<uint32_t> collidableGeometryIndices);

		bool IsDead();
		bool Win(Model * winZone);
	private:

		// Collision Detection and Response Function Prototypes
		XMVECTOR Collision(vector<XMFLOAT3>& vertPos,		// An array holding the polygon soup vertex positions
			vector<uint32_t>& indices,							// An array holding the polygon soup indices (triangles)
			const XMVECTOR &velocity);
			

		XMVECTOR CollideWithWorld(CollisionPacket& cP,			// Same arguments as the above function
			vector<XMFLOAT3>& vertPos,
			vector<uint32_t>& indices);

		bool SphereCollidingWithTriangle(CollisionPacket& cP,	// Pointer to a CollisionPacket object	
			XMVECTOR &p0,										// First vertex position of triangle
			XMVECTOR &p1,										// Second vertex position of triangle
			XMVECTOR &p2,										// Third vertex position of triangle 
			XMVECTOR &triNormal);								// Triangle's Normal

		// Checks if a point (inside the triangle's plane) is inside the triangle
		bool CheckPointInTriangle(const XMVECTOR& point, const XMVECTOR& triV1, const XMVECTOR& triV2, const XMVECTOR& triV3);

		// Solves the quadratic eqation, and returns the lowest root if equation is solvable, returns false if not solvable
		bool GetLowestRoot(float a, float b, float c, float maxR, float* root); 

		void HookToObj(const XMVECTOR &point);
		void GrappleToObj(const XMVECTOR &point, XMVECTOR &velocity);
		void TurnOffHookShot();
		bool CheckHookState();

		bool TestIntersection(const Triangle & tri, XMVECTOR * point, const XMMATRIX & objMatrix);


};

#endif