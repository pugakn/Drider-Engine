cbuffer ConstantBuffer : register(b0) {
  //float4 fViewportDimensions;  
  //X: Number of thread groups in x
  //Y: Number of thread groups in Y.
  //Z: Vertical FOV.
  //W: Offset.
  float4 extraInfo;
};

Texture2D<float4> LightsTransformed : register(t0);
//Texture2D<float4> PositionLinearDepthTex : register(t1);

RWTexture2D<int> LightsIndex : register(u0); //Size: width = total tiles, height = Max lights per tile + 1
RWTexture2D<int> LightsIndexAux : register(u1); //Size: width = total tiles, height = Max lights

bool
intersects(in const float2 ellipsePos,
           in const float2 ellipseRadius,
           in const float2 RectPos,
           in const float2 RectSize) {
  const float DeltaX = max(RectPos.x - RectSize.x, min(ellipsePos.x, RectPos.x + RectSize.x));
  const float DeltaY = max(RectPos.y - RectSize.y, min(ellipsePos.y, RectPos.y + RectSize.y));
  
  float result = ((pow(DeltaX - ellipsePos.x, 2.0f) / pow(ellipseRadius.x, 2.0f)) +
                  (pow(DeltaY - ellipsePos.y, 2.0f) / pow(ellipseRadius.y, 2.0f)));

  return result < 1;
}

float2
getMinMaxDepth(in const uint2 uvOffset) {
  uint2 sampleUV = uvOffset;
  float2 minMax;
  //float2 minMax = PositionLinearDepthTex[uvOffset + uint2(x, y)].ww;

  /*
  float currentDepth;
  for (int x = 0; x < RM_TILE_LIGHTS_SZ; ++x) {
    for (int y = 0; y < RM_TILE_LIGHTS_SZ; ++y) {
      sampleUV = uvOffset + uint2(x, y);
      sampleUV.x = clamp(sampleUV.x, 0, fViewportDimensions.x - 1);
      sampleUV.y = clamp(sampleUV.y, 0, fViewportDimensions.y - 1);

      currentDepth = PositionLinearDepthTex[sampleUV].w;
      minMax.x = min(minMax.x, currentDepth);
      minMax.y = max(minMax.y, currentDepth);
    }
  }
  */

  return minMax;
}

//Max lights = 32 * 4 = 128
//Max lights = 32 * 8 = 256
//Max lights = 32 * 16 = 512
//Max lights = 32 * 32 = 1024
#define NUMTHREADS_X 512
#define NUMTHREADS_Y 1
#define NUMTHREADS_Z 1
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, NUMTHREADS_Z)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
   uint3 groupID				: SV_GroupID,
   uint3 dispatchID			: SV_DispatchThreadID,
   uint groupIndex			: SV_GroupIndex) {

  //Este indica en que index esta este thread group (como si fuese un array).
  const uint group = (groupID.y * extraInfo.x) + groupID.x;
  //const uint group = groupID.x;
  
  //Este indica la luz que tiene que analizar este thread
  const uint lightIndex = groupIndex + (NUMTHREADS_X * groupID.z);

  if (lightIndex >= RM_MAX_LIGHTS) {
    return;
  }
  
  LightsIndexAux[uint2(group, lightIndex)] = -1;
  LightsIndex[uint2(group, clamp(lightIndex, 0, RM_MAX_LIGHTS_PER_BLOCK - 1))] = -1;
  
  const float2 rectSize = float2(rcp(extraInfo.x), rcp(extraInfo.y));
  const float2 rectPos = float2(((rectSize.x * 0.5f) + (rectSize.x * groupID.x)),
																((rectSize.y * 0.5f) + (rectSize.y * groupID.y)));
  
  const float4 myLight = LightsTransformed[uint2(lightIndex, 0)];

  if (sign(myLight.w) < 1) {
    LightsIndexAux[uint2(group, lightIndex)] = -1;
  }
  else {
    const float2 SSlightPos = myLight.xy;
    //const float2 SSlightPos = float2(0.5f.xx);
    const float2 SSlightRad = float2(myLight.z * extraInfo.z,
                                     myLight.z);
    
    LightsIndexAux[uint2(group, lightIndex)] = intersects(SSlightPos, SSlightRad, rectPos, rectSize * 0.5f);
    //bool intersected = intersects(SSlightPos, SSlightRad, rectPos, rectSize * 0.5f);
    //if (intersected) {
      //TODO: Comparar la profundidad min/max de la luz con el min/max del depthbuffer,
      //			si no esta entre esa profundidad y el ojo, no agregarla
      
      //float2 depthMinMax = getMinMaxDepth(Wololo);
      //float2 lightMinMax = Wololo;
      //if ((lightMinMax.x < depthMinMax.y) &&
      //    (lightMinMax.y > depthMinMax.x) ) {
          //LightsIndexAux[uint2(group, lightIndex)] = 1;
      //
      //}
    //}
  }


  if (lightIndex > 0 || groupID.z > 0) {
    return;
  }
  
  int counter = 0;
  [loop]
  for (int currentLight = 0;
       (currentLight < RM_MAX_LIGHTS) && (counter < RM_MAX_LIGHTS_PER_BLOCK);
       ++currentLight) {
    if (LightsIndexAux[uint2(group, currentLight)] > 0) {
      LightsIndex[uint2(group, counter + 1)] = currentLight;
      ++counter;
    }
  }

  //LightsIndex[uint2(group, RM_MAX_LIGHTS_PER_BLOCK)] = counter;
  LightsIndex[uint2(group, 0)] = counter;

  return;
}