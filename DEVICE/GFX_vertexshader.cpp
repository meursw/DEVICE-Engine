#include "GFX_vertexshader.h"

VertexShader::VertexShader(ShaderType st) : BaseShaderClass(st) {}

bool VertexShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	hr = device->CreateVertexShader(
		m_shaderBuffer.Get()->GetBufferPointer(),
		m_shaderBuffer.Get()->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf()
	);

	if (FAILED(hr)) return false;

	return true;
}

ID3D10Blob* VertexShader::GetBytecode() const
{
	return m_shaderBuffer.Get();
}

void VertexShader::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
}