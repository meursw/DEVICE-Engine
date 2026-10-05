#pragma once
#include "GFX_bindable.h"

class Texture :public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	Texture(D3DClass*, const std::string&, HWND, UINT slot = 0);

private:
	UINT m_slot;
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
};
