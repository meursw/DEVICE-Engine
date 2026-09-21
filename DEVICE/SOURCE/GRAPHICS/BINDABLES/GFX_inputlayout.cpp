#include "GFX_inputlayout.h"
#include <vector>

InputLayout::InputLayout(
    ID3D11Device* device,
    std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout,
    ID3D10Blob* vertexShaderBuffer)
{
    HRESULT hr;

    D3D_THROW(device->CreateInputLayout(
        polygonLayout.data(), polygonLayout.size(),
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    ));

}

void InputLayout::Bind(D3DClass* d3d)
{
    d3d->GetDeviceContext()->IASetInputLayout(m_inputLayout.Get());
}
