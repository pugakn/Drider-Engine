//#define INTERVAL_BASED_SELECTION
#define MAP_BASED_SELECTION
//#define DR_SH_PCF_ENABLED
//#define CASCADE_BLUR

cbuffer ConstantBuffer : register(b0) {
  float4	 fViewportDimensions;
  float4x4 kShadowVP[4];
  float4   ShadowSplitDepth;
  float4   ShadowSizesProportion;
  float4   ShadowInfo; //X: Activated cascades, Y: TextureSize, Z: CascadeLerp
};

SamplerState SS : register(s0);

Texture2D PositionDepthTex    : register(t0);
Texture2D CompressedShadowTex : register(t1);

RWTexture2D<float4> ShadowTex : register(u0);

bool
insideBounds(float4 fromLightPos) {
  return (fromLightPos.x > -1.0f) *
         (fromLightPos.x <  1.0f) *
	       (fromLightPos.y > -1.0f) *
         (fromLightPos.y <  1.0f);
}

float
GetShadowValue(float4 fromLightPos, const int camIndex) {
  float shadowValue = 1.0f;

  const float CascadeBiasModifier = ShadowSizesProportion[camIndex] - (camIndex * 1.5f); //Dunno LOL
  
  float2 uv = 0.5f + (0.5f * fromLightPos.xy);
  uv.y = 1.0 - uv.y;

  const float depthValue = fromLightPos.z;

  #ifdef DR_SH_PCF_ENABLED
    static const float shadowBias = 0.0005f;

    //const float texelSize = 1.0f / ShadowInfo[1];
    const float texelSize = rcp(ShadowInfo[1]);
    const float sampleRadius = 3.0f;
    //const float modifier = 4.0f / (sampleRadius * sampleRadius * 2.0f);
    const float modifier = 4.0f * rcp(sampleRadius * sampleRadius * 2.0f);

    [unroll]
    for (float y = -sampleRadius; y <= sampleRadius; y += 1.0f) {
      [unroll]
      for (float x = -sampleRadius; x <= sampleRadius; x += 1.0f) {
        //Projected depth
        float depthSample = CompressedShadowTex.SampleLevel(SS, uv + (texelSize * float2(x, y)), 0)[camIndex];
                                              //Dunno LOL
        shadowValue = (modifier + (camIndex * 0.0004f)) * (depthValue > (depthSample + (shadowBias * CascadeBiasModifier)));
      }
    }

  #else //DR_SH_PCF_ENABLED
    //static const float shadowBias = 0.00005f; //Standard value
    static const float shadowBias = 0.000125f; //This value works well
    float depthSample = CompressedShadowTex.SampleLevel(SS, uv, 0)[camIndex];
    //shadowValue = (0.25f * (depthValue > (depthSample + (shadowBias * CascadeBiasModifier))));
    shadowValue = (0.25f * (depthValue > (depthSample + (shadowBias * CascadeBiasModifier))));
  #endif //DR_SH_PCF_ENABLED

  return shadowValue;
}

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint groupIndex			: SV_GroupIndex) {

	float2 uv = float2(dispatchID.x, dispatchID.y);
	
	float2 uvScale = float2(dispatchID.x * rcp(fViewportDimensions.x),
													dispatchID.y * rcp(fViewportDimensions.y));
  
  const float4 position = float4(PositionDepthTex.SampleLevel(SS, uvScale, 0).xyz, 1.0f);
  const float  depth		= PositionDepthTex.SampleLevel(SS, uvScale, 0).w;

  float ShadowValue = 1.0f;
  static const float CascadeLerp = ShadowInfo[2];
  
  int iCurrentCascadeIndex = 0;
  
  #if defined(INTERVAL_BASED_SELECTION)
    float4 fComparison;
  
    fComparison[0] = depth > ShadowSplitDepth[0];
    fComparison[1] = depth > ShadowSplitDepth[1];
    fComparison[2] = depth > ShadowSplitDepth[2];
    fComparison[3] = depth > ShadowSplitDepth[3];

    static const int activatedCacades = ShadowInfo[0];
    float fIndex = dot(float4(activatedCacades > 0,
                              activatedCacades > 1,
                              activatedCacades > 2,
                              activatedCacades > 3),
                       fComparison);
    iCurrentCascadeIndex = (int)min(fIndex, activatedCacades);

    #ifdef CASCADE_BLUR
      //const float pxProportion = depth / ShadowSplitDepth[iCurrentCascadeIndex];
      //const float ShadowLerp = saturate((pxProportion - CascadeLerp) / (1.0f - CascadeLerp));
      const float pxProportion = depth * rcp(ShadowSplitDepth[iCurrentCascadeIndex]);
      const float ShadowLerp = saturate((pxProportion - CascadeLerp) * rcp(1.0f - CascadeLerp));
    #endif //CASCADE_BLUR
  #elif defined(MAP_BASED_SELECTION)
    float4 fComparison;
    
    static const int activatedCacades = ShadowInfo[0];
    fComparison[0] = insideBounds(mul(kShadowVP[0], position)) * 4 * (0 < activatedCacades);
    fComparison[1] = insideBounds(mul(kShadowVP[1], position)) * 3 * (1 < activatedCacades);
    fComparison[2] = insideBounds(mul(kShadowVP[2], position)) * 2 * (2 < activatedCacades);
    fComparison[3] = insideBounds(mul(kShadowVP[3], position)) * 1 * (3 < activatedCacades);

    iCurrentCascadeIndex = fComparison[0];
    iCurrentCascadeIndex = max(fComparison[1], iCurrentCascadeIndex);
    iCurrentCascadeIndex = max(fComparison[2], iCurrentCascadeIndex);
    iCurrentCascadeIndex = max(fComparison[3], iCurrentCascadeIndex);

    iCurrentCascadeIndex = min(4 - iCurrentCascadeIndex, 3);

    #ifdef CASCADE_BLUR
      const float2 cascadeUV = abs(mul(kShadowVP[iCurrentCascadeIndex], position).xy);
      //const float ShadowLerp = saturate((max(cascadeUV.x, cascadeUV.y) - CascadeLerp) / (1.0f - CascadeLerp));
      const float ShadowLerp = saturate((max(cascadeUV.x, cascadeUV.y) - CascadeLerp) * rcp(1.0f - CascadeLerp));
    #endif //CASCADE_BLUR
  #endif //INTERVAL_BASED_SELECTION || MAP_BASED_SELECTION

  //Projects the position from the mainCam to what shadowCam sees
  const float4 fromMinLightPos = mul(kShadowVP[iCurrentCascadeIndex], position);

  #ifdef CASCADE_BLUR
    const float4 fromMaxLightPos = mul(kShadowVP[min(iCurrentCascadeIndex + 1, 3)], position);
    ShadowValue = lerp(GetShadowValue(fromMinLightPos, iCurrentCascadeIndex),
                        GetShadowValue(fromMaxLightPos, min(iCurrentCascadeIndex + 1, 3)),
                        ShadowLerp);
  #else
    ShadowValue = GetShadowValue(fromMinLightPos, iCurrentCascadeIndex);
  #endif //CASCADE_BLUR

	ShadowTex[uv] = max(0.0f, ShadowTex[uv] - ShadowValue);

  //if (iCurrentCascadeIndex == 0)
  //  ShadowTex[uv] = 1.0f;
  //if (iCurrentCascadeIndex == 1)
  //  ShadowTex[uv] = 0.6f;
  //if (iCurrentCascadeIndex == 2)
  //  ShadowTex[uv] = 0.3f;
  //if (iCurrentCascadeIndex == 3)
  //  ShadowTex[uv] = 0.0f;

	return;
}