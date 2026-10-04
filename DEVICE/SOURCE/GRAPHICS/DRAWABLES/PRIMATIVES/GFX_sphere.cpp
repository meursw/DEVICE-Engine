#include "GFX_sphere.h"
#include "GFX_BindableInc.h"

#include "GFX_vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Sphere::Sphere(D3DClass* d3d, HWND hwnd, float radius)
{
	auto device = d3d->GetDevice();

	if (IsStaticInitialized())
	{
		SetIndexFromStatic();
		AddBind(std::make_unique<TransformCbuf>(device, *this));
		return;
	}

	Assimp::Importer imp;
	const auto pModel = imp.ReadFile("../DEVICE/ASSETS/MODELS/sphere.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	using DEVICE_VERTEX::VertexLayout;
	DEVICE_VERTEX::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
	));

	const auto pMesh = pModel->mMeshes[0];

	for (UINT i = 0; i < pMesh->mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			XMFLOAT3{ pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z }
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

	auto vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		device,
		hwnd,
		L"SHADERS/flat.vs",
		"FlatVertexEntry"
	);

	auto vsByteCode = vertexShader->GetBytecode();

	// ADD BINDS

	AddStaticBind(std::make_unique<VertexBuffer>(device, vbuf));
	AddStaticIndexBuffer(std::make_unique<IndexBuffer>(device, indices));
	AddStaticBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	
	AddStaticBind(std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		device,
		hwnd,
		L"SHADERS/flat.ps",
		"FlatPixelEntry")
	);

	AddStaticBind(std::move(vertexShader));

	AddStaticBind(std::make_unique<InputLayout>(device, vbuf.GetLayout().GetD3DLayout(), vsByteCode));

	AddBind(std::make_unique<TransformCbuf>(device, *this));

	AddBind(std::make_unique<PixelConstantBuffer<PSColorBuf>>(device, 0));
}

void Sphere::Update(D3DClass* d3d, DirectX::XMFLOAT4 color)
{
	auto pixelCbuf = QueryBindable<PixelConstantBuffer<PSColorBuf>>();
	assert(pixelCbuf != nullptr);
	pixelCbuf->Update(d3d->GetDeviceContext(), { color });
}

void Sphere::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}

XMMATRIX Sphere::GetTransformXM() const
{
	return DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}