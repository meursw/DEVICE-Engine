#include "GFX_pixelshader.h"

PixelShader::PixelShader(ShaderType st) : BaseShaderClass(st) {}

bool PixelShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	hr = device->CreatePixelShader(
		m_shaderBuffer.Get()->GetBufferPointer(),
		m_shaderBuffer.Get()->GetBufferSize(),
		NULL,
		m_pixelShader.GetAddressOf()
	);

	if (FAILED(hr)) return false;

	return true;
}

void PixelShader::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
}