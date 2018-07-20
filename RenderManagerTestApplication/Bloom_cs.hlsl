cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
  float4 BloomThresholdLuminiscenceDelta;
};

SamplerState SS : register(s0);

Texture2D ColorTex : register(t0);

RWTexture2D<float4> Brightness : register(u0);

float
luminescence(float3 Color) {
  static const float delta = BloomThresholdLuminiscenceDelta.w;
  static const float3 LuminanceFactor = float3(0.3f, 0.59f, 0.03f);
  //Case LDR
  return (dot(Color, LuminanceFactor) + delta);
  //Case HDR
  return log(dot(Color, LuminanceFactor) + delta);
}

float3
brightness(float3 LinearColor) {
  static const float3 BloomThreshold = BloomThresholdLuminiscenceDelta.xyz;
  return luminescence(LinearColor) * saturate(LinearColor - BloomThreshold.xxx);
}

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  const float2 uvScale = float2(dispatchID.x, dispatchID.y);
	
	const float2 uv = float2(dispatchID.x / fViewportDimensions.x,
                           dispatchID.y / fViewportDimensions.y);

  const float3 color = ColorTex.SampleLevel(SS, uv, 0).xyz;
  
  Brightness[uvScale] = float4(brightness(color), 1.0f);
 
  return;
}