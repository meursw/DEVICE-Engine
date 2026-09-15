#pragma once

// This generic class provides functionality for compiling and creating shaders. 
// Other more specific shader classes will inherit from this to reduce code duplication.

#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <string>

#include "GFX_bindable.h"

enum class ShaderType
{
	VERTEX_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
};

class BaseShaderClass : public Bindable
{
public:
	virtual void Bind(ID3D11DeviceContext*) = 0;

public:
	BaseShaderClass(ShaderType, ID3D11Device*, HWND, const wchar_t*, const char*);
	virtual ~BaseShaderClass() = default;

	void CompileShader(HWND, const wchar_t*, const char*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const wchar_t*);

protected:
	virtual void CreateShader(ID3D11Device*) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D10Blob> m_shaderBuffer;
	const char* m_shaderTarget = nullptr;
};

