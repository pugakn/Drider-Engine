
struct OutBuff
{
  float4 position;
  float4 velocity;
  float4 acceleration;
  float4 color;

  float lifeTime;
  float scale;
  float speedLimit;
  int isActive;
};
cbuffer ConstantBuffer {
  float4 m_globalAcceleration;
  float4 m_initialScale;
  float4 m_finaleScale;
  float4 m_initialColor;
  float4 m_finalColor;

  float dt;
  int  aliveParticles;
  float asdfg2;
  float asdfg3;
};
//StructuredBuffer<BufType> Buffer0 : register(t0);
//StructuredBuffer<BufType> Buffer1 : register(t1);
RWStructuredBuffer<OutBuff> BufferOut : register(u0);
RWStructuredBuffer<int> DeadBuffer : register(u1);
[numthreads(32, 1, 1)]

void CS( uint3 DTid : SV_DispatchThreadID )
{
  if (BufferOut[DTid.x].lifeTime) {
    DeadBuffer[aliveParticles] = DTid.x;
    aliveParticles = aliveParticles - 1;
  }
  BufferOut[DTid.x].lifeTime += dt;

  float _proportionMul = 1.0f / 2.0; // maxLife
  float _proportion = BufferOut[DTid.x].lifeTime * _proportionMul;
  float m1Proportion = (1.0f - _proportion);
#ifdef TIME_COLOR
  BufferOut[DTid.x].color = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
#endif
#ifdef TIME_SCALE
  BufferOut[DTid.x].scale = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion); //* p->m_scaleFactor[i]
#endif
#ifdef EULER_UPDATER
  BufferOut[DTid.x].velocity += (BufferOut[DTid.x].acceleration + m_globalAcceleration) * dt;
  BufferOut[DTid.x].position += BufferOut[DTid.x].velocity * dt;
#endif
  //BufferOut[DTid.x].color = float4(1,1,0,1);
}

