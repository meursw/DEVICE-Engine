#include "GFX_inputlayout.h"

InputLayout::InputLayout(
    ID3D11Device* device,
    VertexLayout& layout,
    ID3D10Blob* vertexShaderBuffer
) 
{
    const auto d3dLayout = layout.GetD3DLayout();

    HRESULT hr;

    D3D_THROW(device->CreateInputLayout(
        d3dLayout.data(), (UINT)d3dLayout.size(),
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    ));

}

void InputLayout::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetInputLayout(m_inputLayout.Get());
}


