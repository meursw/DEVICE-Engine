#pragma once
#include "GFX_drawable.h"
#include "GFX_vertex.h"

class Plane : public Drawable
{
public:
	Plane(D3DClass* d3d, UINT subdiv_x, UINT subdiv_y, float size);
	
private:
	struct VertexPosTex
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

private:
	void AddBinds(D3DClass* d3d);
	void MakeVertices();
	void MakeIndices();
	void CalculateModelVectors();
	void CalculateTangentBinormal(VertexPosTex v1, VertexPosTex v2, VertexPosTex v3);
	
public:
	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(DirectX::XMFLOAT3 rot);
	DirectX::XMMATRIX GetTransformXM() const override;
	void Transform(DirectX::FXMMATRIX matrix);

public:
	// IMGUI
	void SpawnControlWindow(D3DClass* d3d);

private:
	DEVICE_VERTEX::VertexBuffer m_vertices;
	std::vector<unsigned short> m_indices;

private:
	UINT divisions_x, divisions_y;
	float m_size;

private:
	DirectX::XMFLOAT3 m_pos{};

	struct Rotation
	{
		float pitch;
		float yaw;
		float roll;
	};
	Rotation m_rot{};

private:
	struct NormalMapCbuf
	{
		bool normalMapEnabled = true;
		float padding[3] = {};
	} cbData;

};

