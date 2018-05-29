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
RWStructuredBuffer<PoolBuffer> poolBuffer : register(u0);
AppendStructuredBuffer<uint> DeadBuffer : register(u1);
[numthreads(1024, 1, 1)]

void CS( uint3 id : SV_DispatchThreadID )
{
  DeadBuffer.Append(id.x);
  poolBuffer[id.x].isActive = 0;
  poolBuffer[id.x].lifeTime = 0;
  poolBuffer[id.x].color = float4(1,0,1,1);
  poolBuffer[id.x].velocity = float4(0, 0, 0, 0);
  poolBuffer[id.x].acceleration = float4(0, 0, 0, 0);
  poolBuffer[id.x].position = float4(0, 0, 0, 1);
}

