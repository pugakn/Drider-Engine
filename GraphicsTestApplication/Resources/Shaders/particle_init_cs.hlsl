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
RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
AppendStructuredBuffer<uint> DeadBuffer : register(u1);
[numthreads(1024, 1, 1)]

void CS( uint3 id : SV_DispatchThreadID )
{
  if (id.x < m_maxParticles) {
    DeadBuffer.Append(id.x);
    poolBuffer[id.x].isActive = 0;
    poolBuffer[id.x].lifeTime = 3.402823466e+38F;
    poolBuffer[id.x].color = float4(1, 0, 1, 1);
    poolBuffer[id.x].velocity = float4(0, 0, 0, 0);
    poolBuffer[id.x].acceleration = float4(0, 0, 0, 0);
    poolBuffer[id.x].position = float4(0, 0, 0, 1);
  }
}

