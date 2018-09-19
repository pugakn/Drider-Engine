cbuffer ConstantBuffer : register(b0) {
  float4 cameraUp; //X: Number of thread groups in x, Y: Number of thread groups in Y.
  float4x4 VP;
  float4 kLightPosition[RM_MAX_LIGHTS]; // [XYZ = LightPosition, W = Range] // W Sign: Positive = light is active, Negative = light is inactive
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

  //if (lightIndex >= RM_MAX_LIGHTS) { LightsTransformed[lightIndex] = float4(-1.0f.xxxx); return; }
  //if sign returns -1, range is negative
  //if sign returns  0, range is 0
  //if sign returns  1, range is positive
  //if (sign(kLightPosition[lightIndex].w) < 1) { LightsTransformed[lightIndex] = float4(-1.0f.xxxx); return; }

  const float4 LightPos = float4(kLightPosition[lightIndex].xyz, 1.0f);
  const float4 MaxLightPos = LightPos + (cameraUp * kLightPosition[lightIndex].w);
  
  const float4 transformedLight = mul(VP, LightPos);
  const float4 transformedMaxLight = mul(VP, MaxLightPos);
  
  float2 SSlightPos = transformedLight.xy / transformedLight.z;
  float2 SSmaxLightPos = transformedMaxLight.xy / transformedMaxLight.z;
  
  SSlightPos = (SSlightPos + 1.0f) * 0.5f;
  SSlightPos.y = 1.0f - SSlightPos.y;
  SSmaxLightPos = (SSmaxLightPos + 1.0f) * 0.5f;
  SSmaxLightPos.y = 1.0f - SSmaxLightPos.y;
  
  const float SSlightRad = abs(length(SSlightPos - SSmaxLightPos));

  LightsTransformed[uint2(lightIndex, 0)] = float4(SSlightPos, SSlightRad, 1.0f);

  return;
}