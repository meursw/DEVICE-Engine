#include "GFX_cube.h"
#include "GFX_BindableInc.h"
#include <random>

using namespace DirectX;

Cube::Cube(ID3D11Device* device,
	HWND hwnd,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& a,
	std::uniform_real_distribution<float>& b,
	std::uniform_real_distribution<float>& c,
	std::uniform_real_distribution<float>& d,
	std::uniform_real_distribution<float>& scale
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
	scale(scale(rng)),
	Drawable(hwnd)
{
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	std::vector<VertexType> vertices
	{
		{XMFLOAT3(-1,-1,-1),},
		{XMFLOAT3(1,-1,-1),	},
		{XMFLOAT3(1,1,-1),	},
		{XMFLOAT3(-1,1,-1),	},
		{XMFLOAT3(-1,-1,1),	},
		{XMFLOAT3(1, -1, 1),},
		{XMFLOAT3(1, 1, 1),	},
		{XMFLOAT3(-1, 1, 1),}
	};

	AddBind(std::make_unique<VertexBuffer>(device, vertices));

	std::vector<unsigned short> indices
	{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	AddIndexBuffer(std::make_unique<IndexBuffer>(device, indices));

	auto vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		device,
		m_hwnd,
		L"SHADERS/flatcube.vs",
		"FlatCubeVertexEntry"
	);

	auto vsByteCode = vertexShader->GetBytecode();

	AddBind(std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		device,
		m_hwnd,
		L"SHADERS/flatcube.ps",
		"FlatCubePixelEntry")
	);

	AddBind(std::move(vertexShader));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	AddBind(std::make_unique<InputLayout>(device, polygonLayout, vsByteCode));

	AddBind(std::make_unique<TransformCbuf>(device, *this));

	// Colors
	XMFLOAT4 Egg1{ 0.345, 0.537, 0.345,1.0f };
	XMFLOAT4 Egg2{ 0.592, 0.792, 0.537,1.0f };
	XMFLOAT4 Egg3{ 0.475, 0.659, 0.455,1.0f };
	XMFLOAT4 Egg4{ 0.525, 0.710, 0.494,1.0f };
	XMFLOAT4 Egg5{ 0.847, 0.937, 0.765,1.0f };
	XMFLOAT4 Egg6{ 0.741, 0.863, 0.663,1.0f };

	struct FaceColorsType
	{
		XMFLOAT4 face_colors[6];
	};

	FaceColorsType colorsBuffer
	{
		Egg1,Egg2,Egg3,Egg4,Egg5,Egg6
	};

	AddBind(std::make_unique<PixelConstantBuffer<FaceColorsType>>(device, colorsBuffer));
}

void Cube::Update(float delta)
{
	static float elapsedTime{};
	elapsedTime += delta;
	
	roll += droll * delta;
	pitch += dpitch * delta;
	yaw += dyaw * delta;
	theta += dtheta * delta;
	phi += dphi * delta;
	chi += dchi * delta;

	variableScale = (sin(elapsedTime * 0.01f) * 0.5 + 0.5)*scale+0.1;
}

XMMATRIX Cube::GetTransformXM() const
{
	return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		XMMatrixScaling(variableScale, variableScale, variableScale) *
		XMMatrixTranslation(r, 0.0f, 0.0f) *
		XMMatrixRotationRollPitchYaw(theta, phi, chi) 
	;
}
