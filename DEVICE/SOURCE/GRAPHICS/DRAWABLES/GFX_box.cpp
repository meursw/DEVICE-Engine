#include <random>

#include "GFX_box.h"
#include "GFX_BindableInc.h"
#include "GFX_vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX;

Box::Box(D3DClass* d3d,
	HWND hwnd,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& a,
	std::uniform_real_distribution<float>& b,
	std::uniform_real_distribution<float>& c,
	std::uniform_real_distribution<float>& d,
	std::uniform_real_distribution<float>& scale,
	DirectX::XMFLOAT4 materialColor
)
	:
	r(d(rng)),
	droll(b(rng)),
	dyaw(b(rng)),
	dphi(c(rng)),
	dtheta(c(rng)),
	dchi(c(rng)),
	chi(a(rng)),
	theta(a(rng)),
	phi(a(rng)),
	scale(scale(rng))
{

	auto device = d3d->GetDevice();
	// If a cube and all its bindables have already been made,
	// They are stored as static and there is no need to create them again.
	// For the index buffer, since in AddIndexBuffer we retrieve the pointer
	// for the Drawable class member " IndexBuffer* m_indexBuffer "
	// we need to retrieve the pointer for the rest of the objects aswell.
	// We also need to set it's transform buffer.
	if (IsStaticInitialized())
	{
		SetIndexFromStatic();
		AddBind(std::make_unique<TransformCbuf>(device, *this));
		
		struct MaterialCbuf
		{
			DirectX::XMFLOAT4 color;
		} materialBuf;

		materialBuf.color = materialColor;

		AddBind(std::make_unique<PixelConstantBuffer<MaterialCbuf>>(device, materialBuf, 1));
		return;
	}

	Assimp::Importer imp;
	const auto pModel = imp.ReadFile("../DEVICE/ASSETS/MODELS/SLUGCAT.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices
	);

	using DEVICE_VERTEX::VertexLayout;
	DEVICE_VERTEX::VertexBuffer vbuf(std::move(
		VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Texture2D)
		.Append(VertexLayout::Normal)
	));

	const auto pMesh = pModel->mMeshes[0];

	for (UINT i = 0; i < pMesh->mNumVertices; i++)
	{
		vbuf.EmplaceBack(
			XMFLOAT3{pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z},
			*reinterpret_cast<XMFLOAT2*>(&pMesh->mTextureCoords[i]),
			*reinterpret_cast<XMFLOAT3*>(&pMesh->mNormals[i])
		);
	}

	AddStaticBind(std::make_unique<VertexBuffer>(device, vbuf));

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

	AddStaticIndexBuffer(std::make_unique<IndexBuffer>(device, indices));

	auto vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		device,
		hwnd,
		L"SHADERS/phong.vs",
		"PhongVertexEntry"
	);

	auto vsByteCode = vertexShader->GetBytecode();

	AddStaticBind(std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		device,
		hwnd,
		L"SHADERS/phong.ps",
		"PhongPixelEntry")
	);

	AddStaticBind(std::move(vertexShader));

	AddStaticBind(std::make_unique<InputLayout>(device, vbuf.GetLayout().GetD3DLayout(), vsByteCode));

	AddBind(std::make_unique<TransformCbuf>(device, *this));

	AddStaticBind(std::make_unique<Texture>(d3d, L"../DEVICE/ASSETS/TEXTURES/drybones.png", hwnd));
	AddStaticBind(std::make_unique<Sampler>(device));
}

void Box::Update(float delta)
{
	static float elapsedTime{};
	elapsedTime += delta;
	
	roll += droll * delta *0.5;
	pitch += dpitch * delta*0.51;
	yaw += dyaw * delta*0.5;

}

XMMATRIX Box::GetTransformXM() const
{
	return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		XMMatrixScaling(scale, scale, scale) *
		XMMatrixTranslation(r, 0.0f, 0.0f) *
		XMMatrixRotationRollPitchYaw(theta, phi, chi) 
	;
}
