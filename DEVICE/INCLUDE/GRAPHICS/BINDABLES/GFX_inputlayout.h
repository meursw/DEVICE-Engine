#pragma once

#include "GFX_bindable.h"
#include "GFX_vertex.h"
#include <vector>

class InputLayout : public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	InputLayout(D3DClass* d3d, DEVICE_VERTEX::VertexLayout, ID3D10Blob*);
	~InputLayout() = default;

public:
	static std::shared_ptr<InputLayout> Resolve(
		D3DClass* d3d,
		const DEVICE_VERTEX::VertexLayout& layout, 
		ID3DBlob* pVertexShaderByteCode
	);

	static std::string GenerateUID(
		const DEVICE_VERTEX::VertexLayout& layout, 
		ID3DBlob* pVertexShaderBytecode = nullptr
	);

	std::string GetUID() const override;

private:
	DEVICE_VERTEX::VertexLayout m_layout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

