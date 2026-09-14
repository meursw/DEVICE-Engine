#include "GFX_inputlayout.h"

InputLayout::InputLayout() {}

bool InputLayout::Initialize(ID3D11Device* device, ID3D10Blob* vertexShaderBuffer)
{
    const auto d3dLayout = m_layout.GetD3DLayout();
    if (!CreateInpLayout(device, d3dLayout, vertexShaderBuffer))
        return false;

    return true;
}

bool InputLayout::CreateInpLayout(
    ID3D11Device* device, 
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
    ID3D10Blob* vertexShaderBuffer
)
{
    HRESULT hr;

    hr = device->CreateInputLayout(
        layout.data(), (UINT)layout.size(),
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    );
    if (FAILED(hr)) return false;

    return true;
}

void InputLayout::SetVertexLayout(const VertexLayout& vl)
{
    m_layout = vl;
}

void InputLayout::Bind(ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetInputLayout(m_inputLayout.Get());
}


