SamplerState SS : register(s0);

Texture2D TextureIn : register(t0);

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
};

RWStructuredBuffer<float4> AverageLuminescence : register(u0);

float
luminescence(float3 Color, float LuminiscenceDelta) {
  static const float3 LuminanceFactor = float3(0.3f, 0.59f, 0.03f);
  //Case LDR
  return (dot(Color, LuminanceFactor) + LuminiscenceDelta);
  //Case HDR
  return log(dot(Color, LuminanceFactor) + LuminiscenceDelta);
}

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
//[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
[numthreads(1, 1, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
	const float2 wUVScale = float2(dispatchID.x / fViewportDimensions.x,
																 dispatchID.y / fViewportDimensions.y);
  AverageLuminescence[0].x = 0.5;
  /*

  AllMemoryBarrierWithGroupSync();

  const float3 actualPixel = TextureIn.SampleLevel(SS, wUVScale, 0).xyz;

  AverageLuminescence[0].x += luminescence(actualPixel, fViewportDimensions.z);
  
  AllMemoryBarrierWithGroupSync();

  float totalLuminescence = AverageLuminescence[0].x / (fViewportDimensions.x * fViewportDimensions.y);
  
  AverageLuminescence[0].x = totalLuminescence;
  */
  return;
}