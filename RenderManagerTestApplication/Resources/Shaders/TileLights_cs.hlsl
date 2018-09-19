cbuffer ConstantBuffer : register(b0) {
  float4 threadsGroups; //X: Number of thread groups in x, Y: Number of thread groups in Y.
};

struct lightsInBlock {
  int foo[RM_MAX_LIGHTS_PER_BLOCK]; //64
};

struct cacaInBlock {
  int foo[RM_MAX_LIGHTS];
};

RWTexture2D<float4> LightsTransformed : register(u0);

RWStructuredBuffer<lightsInBlock> LightsIndex : register(u0);
RWStructuredBuffer<cacaInBlock> LightsIndexAux : register(u1);

bool
intersects(in const float2 circlePos,
           in const float circleRadius,
           in const float2 RectPos,
           in const float2 RectSize) {
	const float DeltaX = circlePos.x - max(RectPos.x, min(circlePos.x, RectPos.x + RectSize.x));
  const float DeltaY = circlePos.y - max(RectPos.y, min(circlePos.y, RectPos.y + RectSize.y));
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
  
  ///////////////////////////////////////////////////
  //////////Informacion que todos necesitan//////////
  ///////////////////////////////////////////////////

  //Este indica en que index esta este thread gruop (como si fuese un array).
  const uint group = (groupID.y * threadsGroups.x) + groupID.x;
  
  //Este indica la luz que tiene que analizar este thread
  const uint lightIndex = groupIndex;
  
  LightsIndexAux[group].foo[lightIndex] = -1;
  
  const float2 rectSize = float2(rcp(threadsGroups.x), rcp(threadsGroups.y));
  const float2 rectPos = float2((rectSize.x * 0.5f) + (rectSize.x * groupID.x),
																(rectSize.y * 0.5f) + (rectSize.y * groupID.y));
  
  const float2 SSlightPos = LightsTransformed[lightIndex].xy;
  const float SSlightRad = LightsTransformed[lightIndex].z;

  //bool intersected = intersects(SSlightPos, SSlightRad, rectPos, rectSize);
  LightsIndexAux[group].foo[lightIndex] = (intersects(SSlightPos, SSlightRad, rectPos, rectSize) * 2) - 1;

  //if (intersected) {
		//TODO: Comparar la profundidad de la luz con el min/max del depthbuffer,
		//			si no esta entre esa profundidad y el ojo, no agregarla
    
    //No agregar si ya hay RM_MAX_LIGHTS_PER_BLOCK o mas luces
    //Agregar esta luz al array/vector de indices de luces
    //LightsIndexAux[group].foo[lightIndex] = 1;
  //}

  if (lightIndex > 0) {
    return;
  }
  
  //static const int maxLights = NUMTHREADS_X * NUMTHREADS_Y;
  uint counter = 0;
  [loop]
  for (int currentLight = 0; (currentLight < RM_MAX_LIGHTS) && (counter < (RM_MAX_LIGHTS_PER_BLOCK - 1)) ; ++currentLight) {
    if (LightsIndexAux[group].foo[currentLight] > -1) {
      LightsIndex[group].foo[counter] = currentLight;
      ++counter;
    }
  }

  LightsIndex[group].foo[RM_MAX_LIGHTS_PER_BLOCK - 1] = counter;

  return;
}