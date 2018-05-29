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
  float speedLimit;
};
cbuffer ConstantBuffer {
  float4 m_globalAcceleration;
  float4 m_initialColor;
  float4 m_finalColor;
  float m_particleMaxLife;
  float dt;
  int m_particlesToEmit;
  int  aliveParticles;
  float m_initialScale;
  float m_finaleScale;
  float aa;
  float aaaa;
};
//StructuredBuffer<BufType> Buffer0 : register(t0);
//StructuredBuffer<BufType> Buffer1 : register(t1);
RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
AppendStructuredBuffer<uint> DeadBuffer : register(u1);
AppendStructuredBuffer<uint> AliveBuffer : register(u2);
[numthreads(256, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
  float _proportionMul = 1.0f / 2.0; // maxLife
  float _proportion = poolBuffer[id.x].lifeTime * _proportionMul;
  float m1Proportion = (1.0f - _proportion);
#ifdef TIME_COLOR
  poolBuffer[id.x].color = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
#endif
#ifdef TIME_SCALE
  poolBuffer[id.x].scale = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion); //* p->m_scaleFactor[i]
#endif
#ifdef EULER_UPDATER
  poolBuffer[id.x].velocity += (poolBuffer[id.x].acceleration /*+ m_globalAcceleration*/) * dt;
  poolBuffer[id.x].position += poolBuffer[id.x].velocity * dt;
#endif
  //poolBuffer[id.x].scale = 15;
  //poolBuffer[id.x].color = float4(1,0,0,1);

  if (poolBuffer[id.x].lifeTime < m_particleMaxLife) {
    poolBuffer[id.x].lifeTime += dt; //TODO:
    AliveBuffer.Append(id.x);
  }
  else {
    DeadBuffer.Append(id.x);
  }
}

