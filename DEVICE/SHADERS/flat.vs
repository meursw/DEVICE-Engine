cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 FlatVertexEntry(VertexInputType input) : SV_Position
{
    float4 output;
    
    input.position.w = 1.0f;
    
    output = mul(input.position, worldMatrix);
    output = mul(output, viewMatrix);
    output = mul(output, projectionMatrix);
    
    return output;
}