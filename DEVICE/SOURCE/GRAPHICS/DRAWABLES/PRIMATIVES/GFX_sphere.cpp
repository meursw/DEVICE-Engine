#include "GFX_sphere.h"
#include "GFX_BindableInc.h"
#include "GFX_modelclass.h"

Sphere::Sphere(D3DClass* d3d, HWND hwnd, float radius)
{
	auto device = d3d->GetDevice();

	if (IsStaticInitialized())
	{
		SetIndexFromStatic();
		AddBind(std::make_unique<TransformCbuf>(device, *this));
		return;
	}

	ModelClass model(L"../DEVICE/ASSETS/MODELS/sphere.obj");

	AddStaticBind(std::make_unique<VertexBuffer>(device, model.m_vertices));
	AddStaticIndexBuffer(std::make_unique<IndexBuffer>(device, model.m_indices));

	auto vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		device,
		hwnd,
		L"SHADERS/flat.vs",
		"FlatVertexEntry"
	);

	auto vsByteCode = vertexShader->GetBytecode();

	AddStaticBind(std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		device,
		hwnd,
		L"SHADERS/flat.ps",
		"FlatPixelEntry")
	);

	AddStaticBind(std::move(vertexShader));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	AddStaticBind(std::make_unique<InputLayout>(device, polygonLayout, vsByteCode));

	AddBind(std::make_unique<TransformCbuf>(device, *this));

	PSColorBuf colorBuf;
	AddBind(std::make_unique<PixelConstantBuffer<PSColorBuf>>(device, colorBuf, 0));
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