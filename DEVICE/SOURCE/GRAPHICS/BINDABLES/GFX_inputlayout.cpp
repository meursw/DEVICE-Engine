#include "GFX_inputlayout.h"
#include "GFX_bindablecodex.h"
#include <vector>

InputLayout::InputLayout(
    D3DClass* d3d,
    DEVICE_VERTEX::VertexLayout layout,
    ID3D10Blob* vertexShaderBuffer)
    :
    m_layout(std::move(layout))
{
    HRESULT hr;

    const auto d3dLayout = m_layout.GetD3DLayout();

    D3D_THROW(d3d->GetDevice()->CreateInputLayout(
        d3dLayout.data(), (UINT)d3dLayout.size(),
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        m_inputLayout.GetAddressOf()
    ));

}

std::shared_ptr<InputLayout> InputLayout::Resolve(
    D3DClass* d3d,
    const DEVICE_VERTEX::VertexLayout& layout, 
    ID3DBlob* pVertexShaderByteCode)
{
    return BindableCodex::Resolve<InputLayout>(d3d, layout, pVertexShaderByteCode);
}

std::string InputLayout::GenerateUID(const DEVICE_VERTEX::VertexLayout& layout, ID3DBlob* pVertexShaderBytecode)
{
    using namespace std::string_literals;
    return typeid(InputLayout).name() + "#"s + layout.GetCode();
}

std::string InputLayout::GetUID() const
{
    return GenerateUID(m_layout);
}

void InputLayout::Bind(D3DClass* d3d)
{
    d3d->GetDeviceContext()->IASetInputLayout(m_inputLayout.Get());
}
