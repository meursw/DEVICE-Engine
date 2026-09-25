#include "GFX_camera.h"
#include "IMGUI/imgui.h"

using namespace DirectX;

Camera::Camera(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	m_position = XMFLOAT3{};
	m_rotation = XMFLOAT3{};
	m_positionAroundOrigin = XMFLOAT2{};

	m_viewMatrix = XMMATRIX{};

	CreateProjectionAndOrthoMatrix(screenWidth, screenHeight, screenDepth, screenNear);
}

void Camera::UpdateViewMatrix()
{
	// Construct a view matrix based on the position and rotation.
	
	// Initialize vectors.
	XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0, 0.0f);
	XMVECTOR lookAtVector = XMVectorSet(0.0f, 0.0f, 1.0, 0.0f);
	XMVECTOR positionVector = XMLoadFloat3(&m_position);

	// Calculate rotation matrix.
	float pitch = XMConvertToRadians(m_rotation.x);
	float yaw = XMConvertToRadians(m_rotation.y);
	float roll = XMConvertToRadians(m_rotation.z);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Calculate rotation around the origin matrix.
	/*float phi = XMConvertToRadians(m_positionAroundOrigin.x);
	float theta = XMConvertToRadians(m_positionAroundOrigin.y);
	XMMATRIX originRotationMatrix = XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f);*/

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

void Camera::GetViewMatrix(XMMATRIX& view) const
{
	view = m_viewMatrix;
}

void Camera::GetProjectionMatrix(XMMATRIX& proj) const
{
	proj = m_projectionMatrix;
}

void Camera::GetOrthoMatrix(XMMATRIX& ortho) const
{
	ortho = m_orthoMatrix;
}

void Camera::CreateProjectionAndOrthoMatrix(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	// The projection matrix is used to translate the 3D scene into the 2D viewport space that we previously created. 
	// We will need to keep a copy of this matrix so that we can pass it to our shaders that will be used to render our scenes.

	float fieldOfView = 3.141592654f / 3.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

// imgui
void Camera::SpawnControlWindow()
{
	if (ImGui::Begin("Camera Controller"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("Pos Z", &m_position.z, -100.0, 100.0);
		ImGui::SliderFloat("Phi", &m_positionAroundOrigin.x, -89.0f, 89.0f);
		ImGui::SliderFloat("Theta", &m_positionAroundOrigin.y, -180.0f, 180.0f);

		ImGui::Text("Orientation");
		ImGui::SliderFloat("Roll", &m_rotation.z, -180.0f, 180.0f);
		ImGui::SliderFloat("Pitch", &m_rotation.x, -180.0f, 180.0f);
		ImGui::SliderFloat("Yaw", &m_rotation.y, -180.0f, 180.0f);

		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void Camera::Reset()
{
	m_position = { 0.0f,0.0f,-10.0f };
	m_rotation = {};
	m_positionAroundOrigin = {};
}
