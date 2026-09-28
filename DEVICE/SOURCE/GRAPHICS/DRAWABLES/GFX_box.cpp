#include <random>

#include "GFX_box.h"
#include "GFX_BindableInc.h"
#include "GFX_modelclass.h"


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

	ModelClass model(L"../DEVICE/ASSETS/MODELS/drybones.obj");

	AddStaticBind(std::make_unique<VertexBuffer>(device, model.m_vertices));

	AddStaticIndexBuffer(std::make_unique<IndexBuffer>(device, model.m_indices));

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

	const std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	AddStaticBind(std::make_unique<InputLayout>(device, polygonLayout, vsByteCode));

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
