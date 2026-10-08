#include "GFX_sphere.h"
#include "GFX_BindableInc.h"

#include "GFX_vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Sphere::Sphere(D3DClass* d3d, HWND hwnd, float radius)
{
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

	// ADD BINDS
	AddBind(std::make_shared<TransformCbuf>(d3d, *this));

	auto vertexShader = VertexShader::Resolve(
		d3d,
		ShaderType::VERTEX_SHADER,
		hwnd,
		"SHADERS/flat.vs",
		"FlatVertexEntry"
	);

	auto vsByteCode = vertexShader->GetBytecode();
	AddBind(std::move(vertexShader));

	AddBind(PixelShader::Resolve(
		d3d,
		ShaderType::PIXEL_SHADER,
		hwnd,
		"SHADERS/flat.ps",
		"FlatPixelEntry")
	);

	const auto meshTag = "SPHERE";

	AddBind(VertexBuffer::Resolve(d3d, meshTag, vbuf));
	AddBind(IndexBuffer::Resolve(d3d, meshTag, indices));
	AddBind(Topology::Resolve(d3d, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(InputLayout::Resolve(d3d, vbuf.GetLayout(), vsByteCode));

	AddBind(PixelConstantBuffer<PSColorBuf>::Resolve(d3d, 0));

	AddBind(std::make_shared<TransformCbuf>(d3d, *this));
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