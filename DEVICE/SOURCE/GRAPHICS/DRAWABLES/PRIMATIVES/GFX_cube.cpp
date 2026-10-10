#include "GFX_cube.h"
#include "GFX_BindableInc.h"

#include "GFX_vertex.h"

#include "IMGUI/imgui.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Cube::Cube(D3DClass* d3d)
{
	Assimp::Importer imp;
	const auto pModel = imp.ReadFile("../DEVICE/ASSETS/MODELS/cube.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	using DEVICE_VERTEX::VertexLayout;
	DEVICE_VERTEX::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Texture2D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Tangent)
		.Append(VertexLayout::Bitangent)
	));

	const auto pMesh = pModel->mMeshes[0];

	for (UINT i = 0; i < pMesh->mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			*reinterpret_cast<XMFLOAT3*>(&pMesh->mVertices[i]),
			*reinterpret_cast<XMFLOAT2*>(&pMesh->mTextureCoords[0][i]),
			*reinterpret_cast<XMFLOAT3*>(&pMesh->mNormals[i]),
			*reinterpret_cast<XMFLOAT3*>(&pMesh->mTangents[i]),
			*reinterpret_cast<XMFLOAT3*>(&pMesh->mBitangents[i])
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(pMesh->mNumFaces * 3);
	for (UINT i = 0; i < pMesh->mNumFaces; i++)
	{
		const auto& face = pMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	// ADD BINDS
	AddBind(std::make_shared<TransformCbuf>(d3d, *this));

	const auto geometryTag = "Cube";

	AddBind(VertexBuffer::Resolve(d3d, geometryTag, vbuf));
	AddBind(IndexBuffer::Resolve(d3d, geometryTag, indices));

	auto vertexShader = VertexShader::Resolve(
		d3d,
		ShaderType::VERTEX_SHADER,
		nullptr,
		"SHADERS/phongNormalMap.vs",
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

	AddBind(InputLayout::Resolve(d3d, vbuf.GetLayout(), vBytecode));
	AddBind(Topology::Resolve(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(Texture::Resolve(d3d, "../DEVICE/ASSETS/TEXTURES/brickwall.jpg"));
	AddBind(Texture::Resolve(d3d, "../DEVICE/ASSETS/TEXTURES/brickwall_normal.jpg", nullptr, 1u));
	AddBind(Sampler::Resolve(d3d));

	AddBind(PixelConstantBuffer<NormalMapCbuf>::Resolve(d3d, cbData, 2u));

	AddBind(std::make_shared<TransformCbufDouble>(d3d, *this, 0u, 3u));
}

DirectX::XMMATRIX Cube::GetTransformXM() const
{
	return XMMatrixRotationRollPitchYaw(m_rot.pitch, m_rot.yaw, m_rot.roll) *
		XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Cube::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

void Cube::SpawnControlWindow(D3DClass* d3d)
{
	if (ImGui::Begin("Cube"))
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
