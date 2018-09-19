cbuffer ConstantBuffer : register(b0) {
  float4 threadsGroups; //X: Number of thread groups in x, Y: Number of thread groups in Y.
};

Texture2D<float4> LightsTransformed : register(t0);

RWTexture2D<int> LightsIndex : register(u0); //Size: width = total tiles, height = Max lights per tile + 1
RWTexture2D<int> LightsIndexAux : register(u1); //Size: width = total tiles, height = Max lights per tile

bool
intersects(in const float2 ellipsePos,
           in const float2 ellipseRadius,
           in const float2 RectPos,
           in const float2 RectSize) {
  /*
  //Circle case
	const float DeltaX = ellipsePos.x - max(RectPos.x, min(ellipsePos.x, RectPos.x + RectSize.x));
  const float DeltaY = ellipsePos.y - max(RectPos.y, min(ellipsePos.y, RectPos.y + RectSize.y));
  return ((DeltaX * DeltaX) + (DeltaY * DeltaY)) < (ellipseRadius * ellipseRadius); //Circle case
  */
  //Ellipse case
  const float DeltaX = max(RectPos.x - (RectSize.x * 0.5f), min(ellipsePos.x, RectPos.x + (RectSize.x * 0.5f)));
  const float DeltaY = max(RectPos.y - (RectSize.y * 0.5f), min(ellipsePos.y, RectPos.y + (RectSize.y * 0.5f)));
  return ((pow(DeltaX - ellipsePos.x, 2) / pow(ellipseRadius.x, 2)) + (pow(DeltaY - ellipsePos.y, 2) / pow(ellipseRadius.y, 2))) <= 1; 
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

  //Este indica en que index esta este thread group (como si fuese un array).
  const uint group = (groupID.y * threadsGroups.x) + groupID.x;
  
  //Este indica la luz que tiene que analizar este thread
  const uint lightIndex = groupIndex;

  if (lightIndex >= RM_MAX_LIGHTS) {
    return;
  }
  
  LightsIndexAux[uint2(group, lightIndex)] = 0;
  LightsIndex[uint2(group, lightIndex)] = 0;
  
  const float2 rectSize = float2(rcp(threadsGroups.x), rcp(threadsGroups.y));
  const float2 rectPos = float2((rectSize.x * 0.5f) + (rectSize.x * groupID.x),
																(rectSize.y * 0.5f) + (rectSize.y * groupID.y));
  
  const float4 myLight = LightsTransformed[uint2(lightIndex, 0)];

  if (sign(myLight.w) < 1) {
    LightsIndexAux[uint2(group, lightIndex)] = 0;
  }
  else {
    const float2 SSlightPos = myLight.xy;
    //const float2 SSlightPos = float2(0.5f.xx);
    const float2 SSlightRad = float2(myLight.z * threadsGroups.z,
                                     myLight.z);
    
    LightsIndexAux[uint2(group, lightIndex)] = intersects(SSlightPos, SSlightRad, rectPos, rectSize);
    //if (intersected) {
      //TODO: Comparar la profundidad de la luz con el min/max del depthbuffer,
      //			si no esta entre esa profundidad y el ojo, no agregarla
      
      //No agregar si ya hay RM_MAX_LIGHTS_PER_BLOCK o mas luces
      //Agregar esta luz al array/vector de indices de luces
      //LightsIndexAux[group].foo[lightIndex] = 1;
    //}
  }


  if (lightIndex > 0) {
    return;
  }
  
  int counter = 0;
  [loop]
  for (int currentLight = 0; (currentLight < RM_MAX_LIGHTS) && (counter < RM_MAX_LIGHTS_PER_BLOCK); ++currentLight) {
    if (LightsIndexAux[uint2(group, currentLight)].x > 0) {
      LightsIndex[uint2(group, counter)] = currentLight;
      ++counter;
    }
  }

  LightsIndex[uint2(group, RM_MAX_LIGHTS_PER_BLOCK)] = counter;

  return;
}