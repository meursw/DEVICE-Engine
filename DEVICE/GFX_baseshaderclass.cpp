#include "GFX_baseshaderclass.h"

using namespace Microsoft::WRL;

BaseShaderClass::BaseShaderClass(ShaderType shaderType) 
{
	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		m_shaderTarget = "vs_5_0";
		break;

	case ShaderType::PIXEL_SHADER:
		m_shaderTarget = "ps_5_0";
		break;

	case ShaderType::COMPUTE_SHADER:
		m_shaderTarget = "cs_5_0";
		break;
		
	}
}

bool BaseShaderClass::InitializeShader(
		ID3D11Device* device,
		HWND hwnd, 
		const wchar_t* shaderFilename, 
		const char* shaderEntryPoint
)
{
	if (!CompileShader(hwnd, shaderFilename, shaderEntryPoint))
		return false;

	if (!CreateShader(device))
		return false;

	return true;
}

bool BaseShaderClass::CompileShader(HWND hwnd, const wchar_t* shaderFilename,const char* shaderEntryPoint)
{
	HRESULT hr;
	ComPtr<ID3D10Blob> errorMessage;

	hr = D3DCompileFromFile
	(
		shaderFilename,
		NULL, NULL,
		shaderEntryPoint,
		m_shaderTarget,
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		m_shaderBuffer.GetAddressOf(),
		errorMessage.GetAddressOf()
	);

	if (FAILED(hr)) {
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage.Get(), hwnd, shaderFilename);
		else
			MessageBox(hwnd, shaderFilename, L"Missing Shader File", MB_OK);

		return false;
	}

	return true;
}


void BaseShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const wchar_t* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize;
	std::ofstream fout;

	bufferSize = errorMessage->GetBufferSize();
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	fout.open("shader-error.txt");

	for (int i = 0; i < bufferSize; i++)
		fout << compileErrors[i];
	
	fout.close();

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);
}