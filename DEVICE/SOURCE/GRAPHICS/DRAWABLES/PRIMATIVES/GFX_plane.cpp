#include "GFX_plane.h"
#include "GFX_BindableInc.h"
#include <array>

#include "IMGUI/imgui.h"

using DEVICE_VERTEX::VertexLayout;
using namespace DirectX;

// The constructor takes in the subdivisions for x and y and creates a plane from triangles.
// It uses the DEVICE_VERTEX vertexbuffer to store the vertex data.
Plane::Plane(D3DClass* d3d, UINT div_x, UINT div_y, float size)
	:
	divisions_x(div_x),
	divisions_y(div_y),
	m_size(size),
	m_vertices(VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Texture2D)
		.Append(VertexLayout::Normal))
{
	MakeVertices();
	MakeIndices();
	AddBinds(d3d);

	m_rot = { 0.0f,0.0f,0.0f };
}

void Plane::AddBinds(D3DClass* d3d)
{
	// Scale the plane.
	Transform(XMMatrixScaling(m_size, m_size, 1.0f));

	const auto geometryTag = "Plane:" + std::to_string(divisions_x) + "," + std::to_string(divisions_y);
	
	AddBind(VertexBuffer::Resolve(d3d, geometryTag, m_vertices));
	AddBind(IndexBuffer::Resolve(d3d, geometryTag, m_indices));


	auto vertexShader = VertexShader::Resolve(
		d3d,
		ShaderType::VERTEX_SHADER,
		nullptr,
		"SHADERS/phong.vs",
		"PhongVertexEntry"
	);

	auto vBytecode = vertexShader->GetBytecode();
	AddBind(std::move(vertexShader));

	AddBind(PixelShader::Resolve(
		d3d, 
		ShaderType::PIXEL_SHADER, 
		nullptr,
		"SHADERS/phongNormalMap.ps",
		"PhongPixelEntry"
	));

	AddBind(InputLayout::Resolve(d3d, m_vertices.GetLayout(), vBytecode));
	AddBind(Topology::Resolve(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(Texture::Resolve(d3d, "../DEVICE/ASSETS/TEXTURES/brickwall.jpg"));
	AddBind(Texture::Resolve(d3d, "../DEVICE/ASSETS/TEXTURES/brickwall_normal.jpg", nullptr, 1u));
	AddBind(Sampler::Resolve(d3d));

	AddBind(PixelConstantBuffer<NormalMapCbuf>::Resolve(d3d, cbData, 2u));
	
	AddBind(std::make_shared<TransformCbuf>(d3d, *this));

}

void Plane::MakeVertices()
{
	// Ensure that the dimensions of the plane are atleast 1x1.
	assert(divisions_x >= 1); assert(divisions_y >= 1);

	// Amount of total vertices is div + 1.
	UINT vertices_x = divisions_x + 1;
	UINT vertices_y = divisions_y + 1;

	// Controls width and height of plane.
	constexpr float width = 2.0f;
	constexpr float height = 2.0f;

	float side_x = width / 2.0f;
	float side_y = height / 2.0f;

	// Calculate where inside the width/height each vertex should be placed based on its subdivisions.
	float divisionSize_x = width / float(divisions_x);
	float divisionSize_y = height / float(divisions_y);

	// UV coordinates range from 0 to 1 so the numerator it 1.
	float divisionSize_x_tc = 1.0f / float(divisions_x);
	float divisionSize_y_tc = 1.0f / float(divisions_y);

	for (UINT y = 0; y < vertices_y; y++)
	{
		// Calculate the y position of the plane.
		float y_pos = float(y) * divisionSize_y - 1.0f;
		float y_pos_tc = 1.0f - float(y) * divisionSize_y;
		for (UINT x = 0; x < vertices_x; x++)
		{
			float x_pos = float(x) * divisionSize_x - 1.0f;
			float x_pos_tc = 1.0f - float(x) * divisionSize_x;
			m_vertices.EmplaceBack(
				XMFLOAT3{ x_pos, y_pos, 0.0f },
				XMFLOAT2{ x_pos_tc, y_pos_tc },
				XMFLOAT3{ 0.0f, 0.0f, -1.0f }
			);
		}
	}
}

void Plane::MakeIndices()
{
	m_indices.reserve(divisions_x * divisions_y * divisions_x * divisions_y * 6);

	UINT verticesX = divisions_x + 1;

	const auto vxy2i = [verticesX](size_t x, size_t y)
		{
			return (unsigned short)(y * verticesX + x);
		};

	for (size_t y = 0; y < divisions_y; y++)
	{
		for (size_t x = 0; x < divisions_x; x++)
		{
			const std::array<unsigned short, 4> indexArray =
			{ vxy2i(x,y),vxy2i(x + 1,y),vxy2i(x,y + 1),vxy2i(x + 1,y + 1) };

			m_indices.push_back(indexArray[0]);
			m_indices.push_back(indexArray[2]);
			m_indices.push_back(indexArray[1]);
			m_indices.push_back(indexArray[1]);
			m_indices.push_back(indexArray[2]);
			m_indices.push_back(indexArray[3]);
		}
	}
}
 
void Plane::SetPosition(XMFLOAT3 pos)
{
	m_pos = pos;
}

void Plane::SetRotation(XMFLOAT3 rot)
{
	m_rot = { rot.x, rot.y, rot.z };
}

XMMATRIX Plane::GetTransformXM() const
{
	return XMMatrixRotationRollPitchYaw(m_rot.pitch, m_rot.yaw, m_rot.roll) *
		XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Plane::Transform(FXMMATRIX matrix)
{
	using Elements = DEVICE_VERTEX::VertexLayout::ElementType;
	for (int i = 0; i < m_vertices.Size(); i++)
	{
		auto& pos = m_vertices[i].Attr<Elements::Position3D>();
		DirectX::XMStoreFloat3(
			&pos,
			DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&pos), matrix)
		);
	}
}

void Plane::SpawnControlWindow(D3DClass* d3d)
{
	if (ImGui::Begin("Plane"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -10.0f, 10.0f, "%.1f");
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &m_rot.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &m_rot.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &m_rot.yaw, -180.0f, 180.0f);
		ImGui::Text("Normal Mapping");
		bool checkState = cbData.normalMapEnabled;
		bool changed = ImGui::Checkbox("Enable Normal Map", &checkState);
		cbData.normalMapEnabled = checkState ? true : false;

		if (changed)
		{
			QueryBindable<PixelConstantBuffer<NormalMapCbuf>>()->Update(d3d->GetDeviceContext(), cbData);
		}

	}
	ImGui::End();
}
