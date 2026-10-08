#pragma once

#include "GFX_baseshaderclass.h"

class VertexShader : public BaseShaderClass
{
public:
	void Bind(D3DClass*) override;

public:
	VertexShader(D3DClass* d3d, ShaderType, HWND, const std::string&, const std::string&);
	~VertexShader() = default;

	ID3D10Blob* GetBytecode() const;

private:
	void CreateShader(ID3D11Device*) override;

public:
	static std::shared_ptr<VertexShader> Resolve(
		D3DClass* d3d, ShaderType, HWND hwnd, const std::string&, const std::string&
	);

	static std::string GenerateUID(ShaderType type, HWND hwnd, const std::string& path, const std::string& entry)
	{
		return GenerateUID_(path);
	}
	std::string GetUID() const override;

private:
	static std::string GenerateUID_(const std::string& path);

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

};

