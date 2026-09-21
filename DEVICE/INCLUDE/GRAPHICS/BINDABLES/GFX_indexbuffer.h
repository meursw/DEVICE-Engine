#pragma once
#include "GFX_bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	void Bind(D3DClass*) override;

public:
	IndexBuffer(ID3D11Device* device, const std::vector<unsigned short>&);
	~IndexBuffer() = default;

	UINT GetIndexCount() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexCount;
};

