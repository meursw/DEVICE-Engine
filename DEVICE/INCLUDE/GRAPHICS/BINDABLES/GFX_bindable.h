#pragma once

/// <summary>
/// Abstract class that binds "bindable" objects to the rendering pipeline.
/// Bindable objects include: shaders, input layouts, constant buffers, textures etc.
/// </summary>

#include <wrl/client.h> // Include for ComPtr
#include <d3d11.h>

#include "SYS_d3d_exception.h"

class Bindable
{
public:
	virtual void Bind(ID3D11DeviceContext*) = 0;
	virtual ~Bindable() = default;
};

