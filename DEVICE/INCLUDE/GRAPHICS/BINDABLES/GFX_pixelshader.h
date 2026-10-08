#pragma once

#include "GFX_baseshaderclass.h"

class PixelShader : public BaseShaderClass
{
public:
	void Bind(D3DClass*) override;

public:
	PixelShader(D3DClass* d3d, ShaderType, HWND, const std::string&, const std::string&);
	~PixelShader() = default;

private:
	void CreateShader(ID3D11Device*) override;

public:
	static std::shared_ptr<PixelShader> Resolve(
		D3DClass* d3d, ShaderType, HWND, const std::string&, const std::string&
	);

	static std::string GenerateUID(ShaderType type, HWND hwnd, const std::string& path, const std::string& entry)
	{
		return GenerateUID_(path);
	}
	std::string GetUID() const override;

private:
	static std::string GenerateUID_(const std::string& path);

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};

