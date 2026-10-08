#include "GFX_pixelshader.h"
#include "GFX_bindablecodex.h"


PixelShader::PixelShader(
	D3DClass* d3d,
	ShaderType shaderType,
	HWND hwnd,
	const std::string& shaderFilename,
	const std::string& shaderEntryPoint)
	:
	BaseShaderClass(shaderType, hwnd, shaderFilename, shaderEntryPoint)
{
	CreateShader(d3d->GetDevice());
}

void PixelShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	D3D_THROW(device->CreatePixelShader(
		m_shaderBuffer->GetBufferPointer(),
		m_shaderBuffer->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf()
	));
}

std::shared_ptr<PixelShader> PixelShader::Resolve(
	D3DClass* d3d,
	ShaderType shaderType,
	HWND hwnd,
	const std::string& shaderFilename,
	const std::string& shaderEntryPoint)
{
	return BindableCodex::Resolve<PixelShader>(d3d, shaderType, hwnd, shaderFilename, shaderEntryPoint);
}

std::string PixelShader::GenerateUID_(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(PixelShader).name() + "#"s + path;
}

std::string PixelShader::GetUID() const
{
	return GenerateUID_(m_path);
}

void PixelShader::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->PSSetShader(m_pixelShader.Get(), NULL, 0);
}