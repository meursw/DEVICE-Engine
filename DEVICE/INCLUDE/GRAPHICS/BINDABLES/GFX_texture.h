#pragma once
#include "GFX_bindable.h"

class Texture :public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	Texture(D3DClass*, const std::string&, HWND, UINT slot = 0);

public:
	static std::shared_ptr<Bindable> Resolve(D3DClass* d3d, const std::string& path, HWND hwnd = nullptr, UINT slot = 0);
	static std::string GenerateUID(const std::string& path, HWND hwnd, UINT slot);
	std::string GetUID() const override;

private:
	UINT m_slot;

protected:
	std::string m_path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
};
