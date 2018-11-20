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
  int m_bTimeColorUpdaterActive;
  int m_bTimeScaleUpdaterActive;
  int m_bEulerUpdaterActive;
  int m_bAttractorUpdaterActive;
  int m_bVortexActive;
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
  //Vortex Updater
  float4 m_VortexPos[MAX_VORTEX];
  float4 m_VortexForceX_radiuusY[MAX_VORTEX];
  float4 m_VortexUP[MAX_VORTEX];
  //Plane Collision Updater
  float4 m_planeNormal_k;
  float4 m_planePoint;

  //Time Scale Updater
  float m_initialScale;
  float m_finaleScale;

  float m_particleMaxLife;
  int m_particlesToEmit;
  int m_maxParticles;
  int m_numAttractors;
  int m_numRepellers;
  int m_numVortex;
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
[numthreads(DR_NUM_THREADS_PER_BLOCK, 1, 1)]
void CS( uint3 id : SV_DispatchThreadID )
{
  if (id.x < m_particlesToEmit && id.x < numDeadParticles) {
    const float div = (1.0 / 4294967296.0);
    w_seed = id.x * DT *100000;
    uint particleID = DeadBuffer.Consume();
    float r1;
    float r2;
    float r3;
    float3 rrr;
    poolBuffer[particleID].position = float4(0,0,0,1);
    if (m_bBoxGeneratorActive) {
      r1 = float(wang_hash() * div);
      r2 = float(wang_hash() * div);
      r3 = float(wang_hash() * div);
      rrr = float3(r1, r2, r3);
      float3 boxRand = m_randomPosMin + rrr  * (m_randomPosMax - m_randomPosMin);
      poolBuffer[particleID].position = float4(boxRand.xyz, 1);
    }
    poolBuffer[particleID].velocity = float4(0, 0, 0, 0);
    if (m_bRandVelocityGeneratorActive) {
      r1 = float(wang_hash() * div);
      r2 = float(wang_hash() * div);
      r3 = float(wang_hash() * div);
      rrr = float3(r1, r2, r3);
      float3 velRandom = m_randomVelMin + rrr  * (m_randomVelMax - m_randomVelMin);
      poolBuffer[particleID].velocity = float4(velRandom, 0);
    }


    poolBuffer[particleID].acceleration = float4(0, 0, 0, 0);
    poolBuffer[particleID].color = m_initialColor;
    poolBuffer[particleID].lifeTime = 0;
    poolBuffer[particleID].scale = m_initialScale;
    poolBuffer[particleID].isActive = 1;
    AliveBuffer.Append(particleID);
  }
}

