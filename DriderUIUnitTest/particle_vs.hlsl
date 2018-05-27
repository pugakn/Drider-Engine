#define DR_PARTICLES_CPU
cbuffer ConstantBuffer {
  float4x4 V;
  float4x4 P;
};

struct VS_INPUT {
  //Vertex
  float4 Position     : POSITION;

  //Instance
  float4 InstancePos0 : TRANSFORM0;
  float4 InstancePos1 : TRANSFORM1;
  float4 InstancePos2 : TRANSFORM2;
  float4 InstancePos3 : TRANSFORM3;

  float4 color : COLOR;
};

struct VS_OUTPUT {
  float4 Position : SV_POSITION;
  float4 color : COLOR;
};
#ifdef DR_PARTICLES_GPU
struct BufType
{
  float4 color;
};

StructuredBuffer<BufType> BufferOut : register(t0);
#endif
VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT output;
  float4x4 World = float4x4(input.InstancePos0, input.InstancePos1, input.InstancePos2, input.InstancePos3);
  float4x4 worldViewMatrix = mul(World, V);
  float3 positionVS = input.Position * float3(World._11, World._22, World._33);
  positionVS = positionVS + float3(worldViewMatrix._41, worldViewMatrix._42, worldViewMatrix._43);

  output.Position = mul(float4(positionVS, 1.0f), P);
  output.color = input.color;
  return output;
}