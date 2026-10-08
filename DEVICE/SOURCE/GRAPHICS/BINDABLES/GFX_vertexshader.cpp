#include "GFX_vertexshader.h"
#include "GFX_bindablecodex.h"
#include <typeinfo>

VertexShader::VertexShader(
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

std::shared_ptr<VertexShader> VertexShader::Resolve(
	D3DClass* d3d,
	ShaderType shaderType,
	HWND hwnd,
	const std::string& shaderFilename,
	const std::string& shaderEntryPoint)
{
	return BindableCodex::Resolve<VertexShader>(d3d, shaderType, hwnd, shaderFilename, shaderEntryPoint);
}

std::string VertexShader::GenerateUID_(const std::string& path)
{
	using namespace std::string_literals;
	return typeid(VertexShader).name() + "#"s + path;
}

std::string VertexShader::GetUID() const
{
	return GenerateUID_(m_path);
}