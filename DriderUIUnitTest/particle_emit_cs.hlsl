//float
//getRandom(in float2 uv, in float seed) {
//  float nx = frac(sin(dot(uv*(seed), float2(15.8989f, 76.133f) * 1.0f)) * 46736.23457f);
//  return nx;
//};
//
//float3
//getRandom3(in float2 uv, in float seed) {
//  float nx = frac(sin(dot(uv*(seed), float2(15.8989f, 76.133f) * 1.0f)) * 46736.23457f);
//  float ny = frac(sin(dot(uv*(seed), float2(11.9899f, 62.223f) * 1.0f)) * 34748.34744f);
//  float nz = frac(sin(dot(uv*(seed), float2(13.3238f, 63.122f) * 1.0f)) * 59998.47332f);
//
//  return normalize(float3(nx, ny, nz));
//};

static uint w_seed;
uint wang_hash()
{
  w_seed = (w_seed ^ 61) ^ (w_seed >> 16);
  w_seed *= 9;
  w_seed = w_seed ^ (w_seed >> 4);
  w_seed *= 0x27d4eb2d;
  w_seed = w_seed ^ (w_seed >> 15);
  return w_seed;
}

struct PoolBuffer
{
  float4 position;
  float4 velocity;
  float4 acceleration;
  float4 color;

  float lifeTime;
  float scale;
  int isActive;
  int padaaasdasdas;
};
cbuffer ConstantBuffer : register(b0) {
  float4 m_globalAcceleration;
  float4 m_initialColor;
  float4 m_finalColor;
  float m_particleMaxLife;
  float dt;
  int m_particlesToEmit;
  int  aliveParticles;
  float m_initialScale;
  float m_finaleScale;
  int m_maxParticles;
  float aaaa;
};

cbuffer ListCount1 : register(b1)
{
  uint	numDeadParticles;
  uint3	pad1;
};
cbuffer dtBuff : register(b2)
{
  float	DT;
  float3	pad2;
};

RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
ConsumeStructuredBuffer<uint> DeadBuffer : register(u1);
AppendStructuredBuffer<uint> AliveBuffer : register(u2);
[numthreads(1024, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{

  if (id.x < m_particlesToEmit && id.x < numDeadParticles) {
    const float div = (1.0 / 4294967296.0);
    w_seed = id.x * DT *100000;
    uint particleID = DeadBuffer.Consume();

    float3 randPosMin = float3(-900,0,-900);
    float3 randPosMax = float3(900, 200, 900);
    float3 randVelMin = float3(-0,200, -0);
    float3 randVelMax = float3(0, 500, 0);

    //Random Pos
    float r1 = float(wang_hash() * div);
    float r2 = float(wang_hash() * div);
    float r3 = float(wang_hash() * div);
    float3 rrr = float3(r1, r2, r3);
    float3 boxRand = randPosMin + rrr  * (randPosMax - randPosMin);
    poolBuffer[particleID].position = float4(boxRand.xyz, 1);

    //Random Vel
    r1 = float(wang_hash() * div);
    r2 = float(wang_hash() * div);
    r3 = float(wang_hash() * div);
    rrr = float3(r1, r2, r3);
    float3 velRandom = randVelMin + rrr  * (randVelMax - randVelMin);
    poolBuffer[particleID].velocity = float4(velRandom, 0);


    poolBuffer[particleID].acceleration = float4(0, 0, 0, 0);
    poolBuffer[particleID].color = m_initialColor;
    poolBuffer[particleID].lifeTime = 0;
    poolBuffer[particleID].scale = m_initialScale;
    poolBuffer[particleID].isActive = 1;
    AliveBuffer.Append(particleID);
  }
}

