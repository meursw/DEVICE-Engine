#include "GFX_pixelshader.h"

PixelShader::PixelShader(
	ShaderType shaderType,
	ID3D11Device* device,
	HWND hwnd,
	const wchar_t* shaderFilename,
	const char* shaderEntryPoint)
	:
	BaseShaderClass(shaderType, device, hwnd, shaderFilename, shaderEntryPoint)
{
	CreateShader(device);
}

void PixelShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	D3D_THROW(device->CreatePixelShader(
		m_shaderBuffer.Get()->GetBufferPointer(),
		m_shaderBuffer.Get()->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf()
	));
}

void PixelShader::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
}