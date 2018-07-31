#define MAX_LIGHTS_PER_BLOCK 128

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
  float4 cameraUp; //X: Number of thread groups in x, Y: Number of thread groups in Y.
  float4 threadsGroups; //X: Number of thread groups in x, Y: Number of thread groups in Y.
  float4x4 VP;
  float4 kLightPosition[512];	//XYZ: Light Position, W: Range
};

struct lightsInBlock {
  int foo[MAX_LIGHTS_PER_BLOCK];
};

struct cacaInBlock {
  int foo[512];
};

RWStructuredBuffer<lightsInBlock> LightsIndex : register(u0);
RWStructuredBuffer<cacaInBlock> LightsIndexAux : register(u1);

bool
intersects(float2 circlePos, float circleRadius, float2 RectPos, float2 RectSize) {
	float DeltaX = circlePos.x - max(RectPos.x, min(circlePos.x, RectPos.x + RectSize.x));
  float DeltaY = circlePos.y - max(RectPos.y, min(circlePos.y, RectPos.y + RectSize.y));
  return ((DeltaX * DeltaX) + (DeltaY * DeltaY)) < (circleRadius * circleRadius);
}

//Max lights = 32 * 4 = 128
//Max lights = 32 * 8 = 256
//Max lights = 32 * 16 = 512
//Max lights = 32 * 32 = 1024
#define NUMTHREADS_X 32
#define NUMTHREADS_Y 16
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
   uint3 groupID				: SV_GroupID,
   uint3 dispatchID			: SV_DispatchThreadID,
   uint groupIndex			: SV_GroupIndex) {
  
  //Este indica en que index esta este grupo thread (como si fuese un array).
  const uint group = (groupID.y * threadsGroups.x) + groupID.x;
  
  //Este indica la luz que tiene que analizar este thread
  const uint lightIndex = groupIndex;
  
  LightsIndexAux[group].foo[lightIndex] = -1;
  
  static const float2 rectSize = float2(1.0f / threadsGroups.x, 1.0f / threadsGroups.y);
  const float2 rectPos = float2((rectSize.x * 0.5f) + (rectSize.x * groupID.x),
																(rectSize.y * 0.5f) + (rectSize.y * groupID.y));

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  float4 LightPos = float4(kLightPosition[lightIndex].xyz, 1.0f);
  float4 MaxLightPos = LightPos + (cameraUp * kLightPosition[lightIndex].w);

  const float4 transformedLight = mul(VP, LightPos);
  const float4 transformedMaxLight = mul(VP, MaxLightPos);

  float2 SSlightPos = transformedLight.xy / transformedLight.z;
  float2 SSmaxLightPos = transformedMaxLight.xy / transformedMaxLight.z;

  SSlightPos = (SSlightPos + 1.0f) * 0.5f;
  SSlightPos.y = 1.0f - SSlightPos.y;
  SSmaxLightPos = (SSmaxLightPos + 1.0f) * 0.5f;
  SSmaxLightPos.y = 1.0f - SSmaxLightPos.y;

  const float  SSlightRad = abs(length(SSlightPos - SSmaxLightPos));
  
  bool intersected = intersects(SSlightPos, SSlightRad, rectPos, rectSize);

  if (intersected) {
		//TODO: Comparar la profundidad de la luz con el min/max del depthbuffer,
		//			si no esta entre esa profundidad y el ojo, no agregarla
    
    //No agregar si ya hay MAX_LIGHTS_PER_BLOCK o mas luces
    //Agregar esta luz al array/vector de indices de luces
    LightsIndexAux[group].foo[lightIndex] = 1;
  }

  if (lightIndex > 0) {
    return;
  }
  
  //static const int maxLights = NUMTHREADS_X * NUMTHREADS_Y;
  uint counter = 0;
  [loop]
  for (int currentLight = 0; (currentLight < 512) && (counter < (MAX_LIGHTS_PER_BLOCK - 1)) ; ++currentLight) {
    if (LightsIndexAux[group].foo[currentLight] > -1) {
      LightsIndex[group].foo[counter] = currentLight;
      ++counter;
    }
  }

  LightsIndex[group].foo[MAX_LIGHTS_PER_BLOCK - 1] = counter;

  return;
}