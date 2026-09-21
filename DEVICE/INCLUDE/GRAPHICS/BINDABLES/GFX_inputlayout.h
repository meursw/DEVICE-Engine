#pragma once

#include "GFX_bindable.h"
#include <vector>

class InputLayout : public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	InputLayout(ID3D11Device*, std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout, ID3D10Blob*);
	~InputLayout() = default;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

