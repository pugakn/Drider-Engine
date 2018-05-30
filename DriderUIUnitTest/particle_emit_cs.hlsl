
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

RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
ConsumeStructuredBuffer<uint> DeadBuffer : register(u1);
AppendStructuredBuffer<uint> AliveBuffer : register(u2);
[numthreads(1024, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
  if (id.x < m_particlesToEmit && id.x < numDeadParticles) {
    uint particleID = DeadBuffer.Consume();
    poolBuffer[particleID].position = float4(0, 0, 0, 1);
    poolBuffer[particleID].velocity = float4(0, 100, 0, 0);
    poolBuffer[particleID].acceleration = float4(0, 0, 0, 0);

    poolBuffer[particleID].color = m_initialColor;
    poolBuffer[particleID].lifeTime = 0;
    poolBuffer[particleID].scale = m_initialScale;
    poolBuffer[particleID].isActive = 1;
    //AliveBuffer.Append(particleID);
  }
}

