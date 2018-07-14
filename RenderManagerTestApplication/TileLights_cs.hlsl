#define MAX_LIGHTS_PER_BLOCK 64

cbuffer ConstantBuffer : register(b0)
{
  float2 threadsGroups; //X: Number of thread groups in x, Y: Number of thread groups in Y.
  float4x4 VP;
};

StructuredBuffer<float4> kLightPosition[128]	: register(u0);	//XYZ: Light Position, W: Range
RWStructuredBuffer<int> numberOfLights     		: register(u1);
RWStructuredBuffer<int[MAX_LIGHTS_PER_BLOCK]> LightsIndex     : register(u2);

bool
intersects(float2 circlePos, float circleRadius, float2 RectPos, float2 RectSize) {
	float DeltaX = circlePos.x - max(RectPos.x, min(circlePos.x, RectPos.x + RectSize.x));
  float DeltaY = circlePos.y - max(RectPos.y, min(circlePos.y, RectPos.y + RectSize.y));
  return ((DeltaX * DeltaX) + (DeltaY * DeltaY)) < (circleRadius * circleRadius);
}

//Max lights = 32 * 320 = 10,240
#define NUMTHREADS_X 32
#define NUMTHREADS_Y 320
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
   uint3 groupID				: SV_GroupID,
   uint3 dispatchID			: SV_DispatchThreadID,
   uint groupIndex			: SV_GroupIndex) {
  
  //Este indica en que index esta este grupo thread (como si fuese un array).
  const uint group = (groupID.y * threadsGroups.x) + groupID.x;

  numberOfLights[group] = 0;

	GroupMemoryBarrierWithGroupSync();
  
  //Este indica la luz que tiene que analizar este thread
  const uint lightIndex = groupIndex;
  
  //TODO: Borrar despues
  if (lightIndex >= 128) {
		return;
  }
  
  const float4 transformedLight = mul(VP, kLightPosition[lightIndex]);
  const float2 lightPos = transformedLight.xy;
  const float  lightRad = transformedLight.w;
  
  static const float2 rectSize = float2(1.0f / threadsGroups.x, 1.0f / threadsGroups.y);
  const float2 rectPos = float2((rectSize.x * 0.5f) + (rectSize.x * groupID.x),
																(rectSize.y * 0.5f) + (rectSize.y * groupID.y));

  if (intersects(lightPos, lightRad, rectPos, rectSize)) {
		//TODO: Comparar la profundidad de la luz con el min/max del depthbuffer,
		//			si no esta entre esa profundidad y el ojo, no agregarla
    
    //No agregar si ya hay MAX_LIGHTS_PER_BLOCK o mas luces
    if (numberOfLights[group] < MAX_LIGHTS_PER_BLOCK) {
		  //Agregar esta luz al array/vector de indices de luces
      LightsIndex[numberOfLights[group]].append(lightIndex);
      ++numberOfLights[group];
    }
  }

	return;
}