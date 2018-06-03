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
  //Plane Collition Updater

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
  if (id.x < m_maxParticles && poolBuffer[id.x].isActive) {
    uint pID = id.x;
    poolBuffer[pID].lifeTime += DT;
    float _proportionMul = 1.0f / m_particleMaxLife;
    float _proportion = poolBuffer[pID].lifeTime * _proportionMul;
    float m1Proportion = (1.0f - _proportion);
    //Time color updater
    if (m_bTimeColorUpdaterActive) {
      poolBuffer[pID].color = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
    }
    //Time scale updater
    if (m_bTimeScaleUpdaterActive) {
      poolBuffer[pID].scale = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion);
    }
    //Attractor updater
    if (m_bAttractorUpdaterActive) {
      for (int i = 0; i < m_numAttractors; ++i) {
        float3 v = m_attractorPos[i] - poolBuffer[pID].position.xyz;
        float l = length(v);
        if (l < m_attractorForceX_radiusY[i].y) {
          poolBuffer[pID].acceleration.xyz += normalize(v) * (m_attractorForceX_radiusY[i].x * (m_attractorForceX_radiusY[i].y/l + 0.00000001));
        }
      }
    }
    //Repeller updater
    if (m_bRepellerUpdaterActive) {
      for (int i = 0; i < m_numRepellers; ++i) {
        float3 v = poolBuffer[pID].position.xyz - m_RepellerPos[i];
        float l = length(v);
        if (l < m_RepellerForceX_radiuusY[i].y) {
          poolBuffer[pID].acceleration.xyz += normalize(v) * (m_RepellerForceX_radiuusY[i].x * (m_RepellerForceX_radiuusY[i].y / l + 0.00000001));
        }
      }
    }
    //Plane colider updater
    if (m_bColliderUpdaterActive) {

    }

    //Always at the end
    if (m_bEulerUpdaterActive) {
      poolBuffer[pID].velocity += (poolBuffer[pID].acceleration + m_globalAcceleration) * DT;
      poolBuffer[pID].position += poolBuffer[pID].velocity * DT;
    }
    if (poolBuffer[pID].lifeTime > m_particleMaxLife) {
      poolBuffer[pID].isActive = 0;
      DeadBuffer.Append(pID);
      //AliveBuffer.DecrementCounter();
    }
    else {
      AliveBuffer.Append(pID);
    }
  }
}

