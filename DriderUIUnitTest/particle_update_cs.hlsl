#define MAX_ATTRACTORS 4
#define MAX_REPELLERS 4
#define MAX_VORTEX 4
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
    poolBuffer[pID].acceleration.xyz = 0;
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
          float3 desiredVelocity = normalize(v) * (m_attractorForceX_radiusY[i].x * (l / m_attractorForceX_radiusY[i].y));
          poolBuffer[pID].acceleration.xyz += (desiredVelocity - poolBuffer[pID].velocity.xyz);
        }
      }
    }

    if (m_bVortexActive) {
      for (int i = 0; i < m_numVortex; ++i) {
        float3 v = m_VortexPos[i].xyz - poolBuffer[pID].position.xyz;
        float l = length(v);
        if (l < m_VortexForceX_radiuusY[i].y) {
          v = cross(m_VortexUP[i], v) - poolBuffer[pID].position.xyz;
          float3 desiredVelocity = normalize(v) * (m_VortexForceX_radiuusY[i].x * (l / m_VortexForceX_radiuusY[i].y));
          poolBuffer[pID].acceleration.xyz += (desiredVelocity - poolBuffer[pID].velocity.xyz);
        }
      }
    }
    //Euler Updater
    if (m_bEulerUpdaterActive) {
      poolBuffer[pID].velocity += (poolBuffer[pID].acceleration + m_globalAcceleration) * DT;
      poolBuffer[pID].position += poolBuffer[pID].velocity * DT;
    }


    //Plane colider updater
    if (m_bColliderUpdaterActive) {
      if (dot(poolBuffer[pID].position.xyz - m_planePoint.xyz, m_planeNormal_k.xyz) < 0.0005) {
        float3 rayOrigin = poolBuffer[pID].position - poolBuffer[pID].velocity *DT;
        float3 rayDir = normalize(poolBuffer[pID].position.xyz - rayOrigin);
        float denom = dot(rayDir, m_planeNormal_k.xyz);
        float t = dot(m_planePoint.xyz - rayOrigin, m_planeNormal_k.xyz) / denom;
        poolBuffer[pID].position.xyz = rayOrigin + rayDir * t;
        if (dot(poolBuffer[pID].velocity.xyz, m_planeNormal_k.xyz) < 0.0005) {
          float d = dot(poolBuffer[pID].velocity.xyz, m_planeNormal_k.xyz);
          float j = max(-(1 + m_planeNormal_k.w) * d, 0);
          poolBuffer[pID].velocity.xyz += j * m_planeNormal_k.xyz;
          poolBuffer[pID].position += poolBuffer[pID].velocity * DT;
        }
      }
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

