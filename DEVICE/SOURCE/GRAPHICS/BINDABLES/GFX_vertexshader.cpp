#include "GFX_vertexshader.h"

VertexShader::VertexShader(
	ShaderType shaderType,
	ID3D11Device* device,
	HWND hwnd,
	const wchar_t* shaderFilename,
	const char* shaderEntryPoint) 
	: 
	BaseShaderClass(shaderType, hwnd, shaderFilename, shaderEntryPoint)
{
	CreateShader(device);
}

void VertexShader::CreateShader(ID3D11Device* device)
{
	HRESULT hr;

	D3D_THROW(device->CreateVertexShader(
		m_shaderBuffer->GetBufferPointer(),
		m_shaderBuffer->GetBufferSize(),
		NULL,
		m_vertexShader.GetAddressOf()
	));
}

ID3D10Blob* VertexShader::GetBytecode() const
{
	return m_shaderBuffer.Get();
}

void VertexShader::Bind(D3DClass* d3d)
{
	d3d->GetDeviceContext()->VSSetShader(m_vertexShader.Get(), NULL, 0);
}