#pragma once

#include "GFX_bindable.h"
#include "GFX_vertexlayout.h"

class InputLayout : public Bindable
{
public:
	void Bind(ID3D11DeviceContext*);

public:
	InputLayout();
	~InputLayout() = default;

	bool Initialize(ID3D11Device*, ID3D10Blob*);
	void SetVertexLayout(const VertexLayout&);

private:
	bool CreateInpLayout(ID3D11Device*, const std::vector<D3D11_INPUT_ELEMENT_DESC>&, ID3D10Blob*);

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	VertexLayout m_layout;
};

