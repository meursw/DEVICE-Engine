#pragma once

#include "GFX_baseshaderclass.h"

class VertexShader : public BaseShaderClass
{
public:
	void Bind(D3DClass*) override;

public:
	VertexShader(ShaderType, ID3D11Device*, HWND, const wchar_t*, const char*);
	~VertexShader() = default;

	ID3D10Blob* GetBytecode() const;

private:
	void CreateShader(ID3D11Device*) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};

