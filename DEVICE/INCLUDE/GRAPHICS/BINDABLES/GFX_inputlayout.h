#pragma once

#include "GFX_bindable.h"
#include "GFX_vertexlayout.h"

class InputLayout : public Bindable
{
public:
	void Bind(ID3D11DeviceContext*);

public:
	InputLayout(ID3D11Device*, VertexLayout&, ID3D10Blob*);
	~InputLayout() = default;

	void SetVertexLayout(const VertexLayout&);

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

