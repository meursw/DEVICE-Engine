cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
};

float4 FlatCubeVertexEntry(VertexInputType input) : SV_Position
{
    float4 output;
    
    input.position.w = 1.0f;
    
    output = mul(input.position, worldMatrix);
    output = mul(output, viewMatrix);
    output = mul(output, projectionMatrix);
    
    return output;
}