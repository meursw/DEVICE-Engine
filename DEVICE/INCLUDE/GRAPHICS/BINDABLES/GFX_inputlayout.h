#pragma once

#include "GFX_bindable.h"

class InputLayout : public Bindable
{
public:
	void Bind(ID3D11DeviceContext*);

public:
	InputLayout(ID3D11Device*, ID3D10Blob*);
	~InputLayout() = default;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

