
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
cbuffer ConstantBuffer : register(b0) {
  float4 m_globalAcceleration;
  float4 m_initialScale;
  float4 m_finaleScale;
  float4 m_initialColor;
  float4 m_finalColor;
  float m_particleMaxLife;
  float dt;
  int m_particlesToEmit;
  int  aliveParticles;
};

cbuffer ListCount1 : register(b1)
{
  uint	numDeadParticles;
  uint3	pad1;
};


RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
ConsumeStructuredBuffer<uint> DeadBuffer : register(u1);
[numthreads(1024, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
  if (id.x < m_particlesToEmit && id.x < numDeadParticles) {
    uint particleID = DeadBuffer.Consume();
    poolBuffer[particleID].position = float4(0, 0, 0, 1);
    poolBuffer[particleID].velocity = float4(0, 100, 0, 0);
    poolBuffer[particleID].acceleration = float4(0, 0, 0, 0);

    poolBuffer[particleID].color = float4(1, 1, 0, 1);
    poolBuffer[particleID].lifeTime = 0.0f;
    poolBuffer[particleID].scale = 1.0f;
    poolBuffer[particleID].isActive = 1;
  }
}

