#include "GFX_camera.h"

using namespace DirectX;

Camera::Camera()
{
	m_position = XMFLOAT3{};
	m_rotation = XMFLOAT3{};
	m_viewMatrix = XMMATRIX{};
}

void Camera::UpdateViewMatrix()
{
	// Construct a view matrix based on the position and rotation.
	
	XMFLOAT3 up{ 0.0f, 1.0f, 0.0 };
	XMFLOAT3 position = m_position;
	XMFLOAT3 lookAt{ 0.0f, 0.0f, 1.0f };

	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	float pitch = XMConvertToRadians(m_rotation.x);
	float yaw = XMConvertToRadians(m_rotation.y);
	float roll = XMConvertToRadians(m_rotation.z);

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vectors by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Transform the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void Camera::SetPosition(DX::XMFLOAT3 pos)
{
	m_position = pos;
}

void Camera::SetRotation(DX::XMFLOAT3 rot)
{
	m_rotation = rot;
}

XMFLOAT3 Camera::GetPosition() const
{
	return m_position;
}

XMFLOAT3 Camera::GetRotation() const
{
	return m_rotation;
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix) const
{
	viewMatrix = m_viewMatrix;
}
