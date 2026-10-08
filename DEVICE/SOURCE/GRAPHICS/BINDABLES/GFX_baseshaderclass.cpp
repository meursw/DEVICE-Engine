#include "GFX_baseshaderclass.h"

using namespace Microsoft::WRL;

BaseShaderClass::BaseShaderClass(
	ShaderType shaderType,
	HWND hwnd,
	const std::string& shaderFilename,
	const std::string& shaderEntryPoint
)
	:
	m_path(shaderFilename)
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

	CompileShader(hwnd, shaderFilename, shaderEntryPoint);
}

void BaseShaderClass::CompileShader(HWND hwnd, const std::string& shaderFilename, const std::string& shaderEntryPoint)
{
	HRESULT hr;
	ComPtr<ID3D10Blob> errorMessage;

	hr = D3DCompileFromFile
	(
		std::wstring{ shaderFilename.begin(), shaderFilename.end() }.c_str(),
		NULL, NULL,
		shaderEntryPoint.c_str(),
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
			MessageBoxA(hwnd, shaderFilename.c_str(), "Missing Shader File", MB_OK);

		D3D_THROW(hr);
	}
}


void BaseShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const std::string& shaderFilename)
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

	MessageBoxA(hwnd, "Error compiling shader.  Check shader-error.txt for message.", shaderFilename.c_str(), MB_OK);
}