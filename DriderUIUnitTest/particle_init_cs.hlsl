#define MAX_ATTRACTORS 4
#define MAX_REPELLERS 4
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
  int m_bTimeColorUpdaterActive;
  int m_bTimeScaleUpdaterActive;
  int m_bEulerUpdaterActive;
  int m_bAttractorUpdaterActive;
  int m_bRepellerUpdaterActive;
  int m_bColliderUpdaterActive;
  int m_bBoxGeneratorActive;
  int m_bRandVelocityGeneratorActive;

  //Particle system
  float4 m_systemPosition;
  float4 m_globalAcceleration;
  //Time Color Updater
  float4 m_initialColor;
  float4 m_finalColor;
  //Box Generator
  float4 m_randomPosMin;
  float4 m_randomPosMax;
  //Random Velocity Generator
  float4 m_randomVelMin;
  float4 m_randomVelMax;
  //Attractors Updater
  float4 m_attractorPos[MAX_ATTRACTORS];
  float4 m_attractorForceX_radiusY[MAX_ATTRACTORS];
  //Repellers Updater
  float4 m_RepellerPos[MAX_REPELLERS];
  float4 m_RepellerForceX_radiuusY[MAX_REPELLERS];
  //Plane Collision Updater
  float4 m_planeNormal;
  float4 m_planePoint;

  //Time Scale Updater
  float m_initialScale;
  float m_finaleScale;

  float m_particleMaxLife;
  int m_particlesToEmit;
  int  aliveParticles;
  int m_maxParticles;
  int m_numAttractors;
  int m_numRepellers;
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

