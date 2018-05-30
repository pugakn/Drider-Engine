#define EULER_UPDATER
#define TIME_COLOR
#define TIME_SCALE
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
  uint	numAliveParticles;
  uint3	pad1;
};
cbuffer dtBuff : register(b2)
{
  float	DT;
  float3	pad2;
};
RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
AppendStructuredBuffer<uint> DeadBuffer : register(u1);
AppendStructuredBuffer<uint> AliveBuffer : register(u2);
[numthreads(256, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
 /* if (id.x < numAliveParticles)
  {*/
  if (id.x < m_maxParticles) {
    uint pID = id.x;
    poolBuffer[pID].lifeTime += DT;
    float _proportionMul = 1.0f / m_particleMaxLife;
    float _proportion = poolBuffer[pID].lifeTime * _proportionMul;
    float m1Proportion = (1.0f - _proportion);
#ifdef TIME_COLOR
    poolBuffer[pID].color = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
#endif
#ifdef TIME_SCALE
    poolBuffer[pID].scale = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion); //* p->m_scaleFactor[i]
#endif
#ifdef EULER_UPDATER
  poolBuffer[pID].velocity += (poolBuffer[pID].acceleration + m_globalAcceleration) * DT;
  poolBuffer[pID].position += poolBuffer[pID].velocity * DT;
#endif
    if (poolBuffer[pID].lifeTime > m_particleMaxLife && poolBuffer[pID].isActive) {
      poolBuffer[pID].isActive = 0;
      DeadBuffer.Append(pID);
      //AliveBuffer.DecrementCounter();
    }
    else {
      AliveBuffer.Append(pID);
    }
  }
}

