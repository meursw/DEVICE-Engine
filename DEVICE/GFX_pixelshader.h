#pragma once

#include "GFX_baseshaderclass.h"

class PixelShader : public BaseShaderClass
{
public:
	void Bind(ID3D11DeviceContext*);

public:
	PixelShader(ShaderType);
	~PixelShader() = default;

private:
	bool CreateShader(ID3D11Device*) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};

