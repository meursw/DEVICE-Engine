#include "GFX_vertexshader.h"

VertexShader::VertexShader(
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

void VertexShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	D3D_THROW(device->CreateVertexShader(
		m_shaderBuffer.Get()->GetBufferPointer(),
		m_shaderBuffer.Get()->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf()
	));
}

ID3D10Blob* VertexShader::GetBytecode() const
{
	return m_shaderBuffer.Get();
}

void VertexShader::Bind(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
}