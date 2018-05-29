#define DR_PARTICLES_GPU
#define MATRIX_Identity float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
float4x4 MATRIX_scale(float4x4 m, float3 v)
{
  float x = v.x, y = v.y, z = v.z;

  m[0][0] *= x; m[1][0] *= y; m[2][0] *= z;
  m[0][1] *= x; m[1][1] *= y; m[2][1] *= z;
  m[0][2] *= x; m[1][2] *= y; m[2][2] *= z;
  m[0][3] *= x; m[1][3] *= y; m[2][3] *= z;

  return m;
}
float4x4 MATRIX_translate(float4x4 m, float3 v)
{
  float x = v.x, y = v.y, z = v.z;
  m[0][3] = x;
  m[1][3] = y;
  m[2][3] = z;
  return m;
}


cbuffer ConstantBuffer : register(b0) {
  float4x4 V;
  float4x4 P;
};
cbuffer ListCount2 : register(b1)
{
  uint	numActiveParticles;
  uint3	pad;
};

struct VS_INPUT {
  float4 Position     : POSITION;
  uint id : SV_InstanceID;
};
struct CPUStructureBuffer {
  float4 position;
  float4 color;
  float scale;
  float aa;
  float aaa;
  float aaaa;
};
struct VS_OUTPUT {
  float4 Position : SV_POSITION;
  float4 color : COLOR;
};
#ifdef DR_PARTICLES_GPU
struct PoolBuffer
{
  float4 position;
  float4 velocity;
  float4 acceleration;
  float4 color;

  float lifeTime;
  float scale;
  int isActive;
  float speedLimit;
};

StructuredBuffer<PoolBuffer> poolBuffer : register(t0);
StructuredBuffer<uint> aliveBuffer : register(t1);

#endif
StructuredBuffer<CPUStructureBuffer> renderBuffer : register(t2);
VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT output = (VS_OUTPUT)0;
  float3 position;
  float3 color;
  float sacale;
#ifdef DR_PARTICLES_GPU
  if (input.id > numActiveParticles) {
    discard;
  }
  uint pID = aliveBuffer[input.id];
  position = poolBuffer[pID].position.xyz;
  color = poolBuffer[pID].color.xyz;
  sacale = poolBuffer[pID].scale;
  sacale = 5;
#else
  position = renderBuffer[input.id].position.xyz;
  color = renderBuffer[input.id].color.xyz;
  sacale = renderBuffer[input.id].scale;
#endif
  float4x4 World = MATRIX_Identity;
  World[3][0] = position.x;
  World[3][1] = position.y;
  World[3][2] = position.z;
  float4x4 worldViewMatrix = mul(World, V);
  float3 positionVS = input.Position * float3(sacale, sacale, sacale);
  positionVS = positionVS + float3(worldViewMatrix._41, worldViewMatrix._42, worldViewMatrix._43);

  output.Position = mul(float4(positionVS, 1.0f), P);
  output.color = float4(color.xyz,1);
#ifdef DR_PARTICLES_GPU
  //}
#endif
  return output;
}