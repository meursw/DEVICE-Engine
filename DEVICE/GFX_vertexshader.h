#pragma once

#include "GFX_baseshaderclass.h"

class VertexShader : public BaseShaderClass
{
public:
	void Bind(ID3D11DeviceContext*);

public:
	VertexShader(ShaderType);
	~VertexShader() = default;

	ID3D10Blob* GetBytecode() const;

private:
	bool CreateShader(ID3D11Device*) override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};

