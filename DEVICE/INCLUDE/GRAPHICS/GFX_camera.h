#pragma once

#include <directxmath.h>

namespace DX = DirectX;

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

	// Imgui
	void SpawnControlWindow();
	void Reset();

private:
	void CreateProjectionAndOrthoMatrix(int, int, float, float);

private:
	DX::XMFLOAT3 m_position;
	DX::XMFLOAT3 m_rotation;
	DX::XMFLOAT2 m_positionAroundOrigin;

	DX::XMMATRIX m_viewMatrix;
	DX::XMMATRIX m_projectionMatrix;
	DX::XMMATRIX m_orthoMatrix;

};

