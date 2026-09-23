#pragma once
#include "GFX_bindable.h"

class Texture :public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	Texture(D3DClass*, const std::wstring&, HWND);

protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
};
