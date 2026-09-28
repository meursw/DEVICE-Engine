#include "GFX_camera.h"
#include "IMGUI/imgui.h"
#include "SYS_inputclass.h"

using namespace DirectX;

Camera::Camera(int screenWidth, int screenHeight, float screenDepth, float screenNear)
{
	m_position = XMFLOAT3{};
	m_rotation = {};
	m_prevMousePos = {};

	m_viewMatrix = XMMATRIX{};

	CreateProjectionAndOrthoMatrix(screenWidth, screenHeight, screenDepth, screenNear);

	m_mouseSens = 200.0f;
	m_movementSpeed = 20.0f;
}

void Camera::UpdateRotation(XMINT2 mousePos, float delta)
{
	XMINT2 offset{ m_prevMousePos.x - mousePos.x, m_prevMousePos.y - mousePos.y};
	
	m_prevMousePos = mousePos;

	m_rotation.yaw += offset.x * m_mouseSens * delta;
	m_rotation.pitch += offset.y * m_mouseSens * delta;

	if (m_rotation.pitch > 89.0f)
		m_rotation.pitch = 89.0f;
	if (m_rotation.pitch < -89.0f)
		m_rotation.pitch = -89.0f;

	UpdateVectors();
}

void Camera::UpdateVectors()
{
	// Update forward vector
	float pitch = XMConvertToRadians(m_rotation.pitch);
	float yaw = XMConvertToRadians(m_rotation.yaw);

	XMFLOAT3 forward = { 
		cosf(yaw) * cosf(pitch),
		sinf(pitch),
		sinf(yaw) * cosf(pitch)
	};

	m_forward = XMLoadFloat3(&forward);

	m_forward = XMVector3Normalize(m_forward);
	

	// Update right vector
	m_right = XMVector3Normalize(
		XMVector3Cross(m_forward, worldUp)
	);

	m_up = XMVector3Normalize(
		XMVector3Cross(m_right, m_forward)
	);
}

void Camera::UpdatePosition(InputClass* Input, float delta)
{
	XMVECTOR positionVector = XMLoadFloat3(&m_position);

	float velocity = m_movementSpeed * delta;

	if (Input->IsKeyPressed('W'))
		positionVector += m_forward * velocity;
	if (Input->IsKeyPressed('S'))
		positionVector -= m_forward * velocity;

	if (Input->IsKeyPressed('D'))
		positionVector -= m_right * velocity;
	if (Input->IsKeyPressed('A'))
		positionVector += m_right * velocity;

	if (Input->IsKeyPressed('E'))
		positionVector += m_up * velocity;
	if (Input->IsKeyPressed('Q'))
		positionVector -= m_up * velocity;


	XMStoreFloat3(&m_position, positionVector);
}

void Camera::UpdateViewMatrix()
{
	// Construct a view matrix based on the position and rotation.
	XMVECTOR positionVector = XMLoadFloat3(&m_position);

	m_viewMatrix = XMMatrixLookAtLH(positionVector, positionVector + m_forward, m_up);

	return;
}

void Camera::SetPosition(XMFLOAT3 pos)
{
	m_position = pos;
}

void Camera::SetRotation(XMFLOAT3 rot)
{
	m_rotation = { rot.x, rot.y, rot.z };
}

XMFLOAT3 Camera::GetPosition() const
{
	return m_position;
}

XMFLOAT3 Camera::GetRotation() const
{
	return { m_rotation.pitch, m_rotation.yaw, m_rotation.roll };
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
		ImGui::Text("Mouse");
		ImGui::SliderFloat("Sensitivity", &m_mouseSens, 1.0, 500.0);
		ImGui::SliderFloat("Speed", &m_movementSpeed, 1.0, 100.0);
		if (ImGui::Button("Reset"))
			Reset();
	}
	ImGui::End();
}

void Camera::Reset()
{
	m_position = { 0.0f,0.0f,-10.0f };
	m_rotation = { 0.0f, 0.0f, 0.0f };
	m_mouseSens = 200.0f;
	m_movementSpeed = 20.0f;
}


