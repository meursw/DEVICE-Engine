cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    
    float4 worldPosition : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
};

PixelInputType PhongVertexEntry(VertexInputType input)
{
    PixelInputType output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;
    
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));
    
    output.tangent = normalize(mul(input.tangent, (float3x3) worldMatrix));
    
    output.bitangent = normalize(mul(input.bitangent, (float3x3) worldMatrix));
    
    float4 worldPosition;
    worldPosition = mul(input.position, worldMatrix);
    output.worldPosition = worldPosition;
    
    output.viewDirection = normalize(cameraPosition - worldPosition.xyz);
    
    return output;
}