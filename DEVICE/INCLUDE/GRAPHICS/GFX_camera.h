#pragma once

#include <directxmath.h>

namespace DX = DirectX;

class Camera
{
public:
	Camera();
	~Camera() = default;

	void UpdateViewMatrix();

	void SetPosition(DX::XMFLOAT3);
	void SetRotation(DX::XMFLOAT3);

	DX::XMFLOAT3 GetPosition() const;
	DX::XMFLOAT3 GetRotation() const;
	void GetViewMatrix(DX::XMMATRIX&) const;

private:
	DX::XMFLOAT3 m_position;
	DX::XMFLOAT3 m_rotation;
	DX::XMMATRIX m_viewMatrix;
};

