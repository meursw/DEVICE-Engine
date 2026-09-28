#pragma once

#include <directxmath.h>

namespace DX = DirectX;

class InputClass;

enum class CameraMovement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NONE
};

class Camera
{
public:
	Camera(int, int, float, float);
	~Camera() = default;

	void UpdateViewMatrix();

	void SetPosition(DX::XMFLOAT3);
	void SetRotation(DX::XMFLOAT3);

	DX::XMFLOAT3 GetPosition() const;
	DX::XMFLOAT3 GetRotation() const;
	void GetViewMatrix(DX::XMMATRIX&) const;
	void GetProjectionMatrix(DX::XMMATRIX&) const;
	void GetOrthoMatrix(DX::XMMATRIX&) const;

	void UpdateRotation(DX::XMINT2, float delta);
	void UpdatePosition(InputClass*, float delta);

	// Imgui
	void SpawnControlWindow();
	void Reset();

private:
	void UpdateVectors();

private:
	void CreateProjectionAndOrthoMatrix(int, int, float, float);

private:
	DX::XMFLOAT3 m_position;

	DX::XMVECTOR m_forward;
	DX::XMVECTOR m_right;
	DX::XMVECTOR m_up;

	DX::XMVECTOR worldUp{ 0.0f,1.0f,0.0f };
	
	struct CameraRotation
	{
		float pitch;
		float yaw;
		float roll;
	};

	CameraRotation m_rotation;

	DX::XMMATRIX m_viewMatrix;
	DX::XMMATRIX m_projectionMatrix;
	DX::XMMATRIX m_orthoMatrix;

private:
	float m_mouseSens = 400.0f;
	float m_movementSpeed = 400.0f;
	DX::XMINT2 m_prevMousePos;
};

