#pragma once

#include "SYS_WinFilter.h"
#include "SYS_DEVICE_exception.h"

#include <DirectXMath.h>
#include <vector>

class ModelClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 normal;
	};

public:
	ModelClass(const std::wstring&);
	~ModelClass() = default;

private:
	void ParseObjFileAndLoadModel(const std::wstring&);

private:
	UINT m_vertexCount;

public:
	std::vector<VertexType> m_vertices;
	std::vector<unsigned short> m_indices;
};

