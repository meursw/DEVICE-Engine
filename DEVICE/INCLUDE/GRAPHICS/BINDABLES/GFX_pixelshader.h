#pragma once

#include "GFX_baseshaderclass.h"

class PixelShader : public BaseShaderClass
{
public:
	void Bind(ID3D11DeviceContext*);

public:
	PixelShader(ShaderType, ID3D11Device*, HWND, const wchar_t*, const char*);
	~PixelShader() = default;

private:
	void CreateShader(ID3D11Device*) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};

