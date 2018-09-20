cbuffer ConstantBuffer : register(b0) {
  //XYZ: normalized camera vector, W = camera far plane
  float4 cameraUp;

  float4 cameraFront;
  float4x4 VP;

  //XYZ = LightPosition, W = Range
  //W Sign: Positive = light is active, Negative = light is inactive
  float4 kLight[RM_MAX_LIGHTS];
};

RWTexture2D<float4> LightsTransformed : register(u0);

#define NUMTHREADS_X 32
#define NUMTHREADS_Y 1
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
   uint3 groupID				: SV_GroupID,
   uint3 dispatchID			: SV_DispatchThreadID,
   uint groupIndex			: SV_GroupIndex) {
  
  const uint lightIndex = dispatchID.x;

  if (lightIndex >= RM_MAX_LIGHTS) {
    return;
  }
  //if sign returns -1, range is negative
  //if sign returns  0, range is 0
  //if sign returns  1, range is positive
  if (sign(kLight[lightIndex].w) <= 0) {
    LightsTransformed[uint2(lightIndex, 0)] = float4(-1.0f.xxxx);
    return;
  }

  const float4 LightPos         = float4(kLight[lightIndex].xyz, 1.0f);
  const float4 LightForwardPos  = LightPos + (float4(cameraFront.xyz, 0.0f) * kLight[lightIndex].w);
  const float4 LightUpPos       = LightPos + (float4(cameraUp.xyz, 0.0f) * kLight[lightIndex].w);

  const float4 transformedLightPos        = mul(VP, LightPos);
  const float4 transformedLightForwardPos = mul(VP, LightForwardPos);
  const float4 transformedLightUpPos      = mul(VP, LightUpPos);
  
  float2 SSlightPos = transformedLightPos.xy / transformedLightPos.w;
  SSlightPos = (SSlightPos + (1.0f).xx) * 0.5f;
  SSlightPos.y = 1.0f - SSlightPos.y;
  
  float2 SSLightUpPos = transformedLightUpPos.xyz / transformedLightUpPos.w;
  SSLightUpPos = (SSLightUpPos + (1.0f).xx) * 0.5f;
  SSLightUpPos.y = 1.0f - SSLightUpPos.y;
  
  const float SSlightRad = abs(length(SSlightPos - SSLightUpPos));

  const float lightLDepthPos = transformedLightPos.z * rcp(cameraUp.w);
  const float lightLDepthRad = abs(LightPos.z - LightForwardPos.z) * rcp(cameraUp.w);
  
  LightsTransformed[uint2(lightIndex, 0)] = float4(SSlightPos.xy, SSlightRad, 1.0f);
  LightsTransformed[uint2(lightIndex, 1)] = float4(lightLDepthPos, lightLDepthRad, 0.0f, 0.0f);

  return;
}