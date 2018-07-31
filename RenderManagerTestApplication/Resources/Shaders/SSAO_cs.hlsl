cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
	float4 SSAO_Options; //X: SampleRadio Y: Intensity Z: Scale X: Bias
};

SamplerState SS : register(s0);

Texture2D PositionLDepthTex : register(t0);
Texture2D NormalCoC         : register(t1);

RWTexture2D<float4> SSAOTex : register(u0);

float3
getPosition(in float2 uv) {
  return PositionLDepthTex.SampleLevel(SS, uv, 0).xyz;
}

float3
getNormal(float2 uv) {
  return NormalCoC.SampleLevel(SS, uv, 0).xyz;
};

float3
getRandom(float2 uv) {
	float3 randonNormal;
  randonNormal.x = frac(sin(dot(uv, float2(15.8989f, 76.133f) * 1.0f)) * 46736.23457f);
  randonNormal.y = frac(sin(dot(uv, float2(11.9899f, 62.223f) * 1.0f)) * 34748.34744f);
  randonNormal.z = frac(sin(dot(uv, float2(13.3238f, 63.122f) * 1.0f)) * 59998.47332f);
  
  return normalize(randonNormal);
};

float
doOclussion(float2 texCoord, float2 uv, float3 p, float3 cnorm) {
  float3 diff = getPosition(texCoord + uv) - p;
  float3 v = normalize(diff);
  float  d = length(diff) * SSAO_Options[2];
  
	return max(0.0f, dot(cnorm, v) - SSAO_Options[3]) * (1.0f / (1.0f + d)) * SSAO_Options[1];
};

static const float2 vec[4] = {
  float2(-1.0f,  0.0f),
  float2( 1.0f,  0.0f),
  float2( 0.0f, -1.0f),
  float2( 0.0f,  1.0f)
};

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint groupIndex			: SV_GroupIndex) {
  
	const float2 uv = float2(dispatchID.x, dispatchID.y);
	
	const float2 wUVScale = float2(dispatchID.x / fViewportDimensions.x,
																 dispatchID.y / fViewportDimensions.y);
	//const float2 wUVScale = float2((dispatchID.x * 1280) / (float)TXWIDTH,
	//												 			 (dispatchID.y * 720) / (float)TXHEIGHT);
	//const float2 wUVScale = float2(dispatchID.x / (float)1280*2,
	//												 			 dispatchID.y / (float)720*2);

  const float3 p = getPosition(wUVScale);
  const float3 n = getNormal(wUVScale);
  const float2 r = getRandom(wUVScale).xy;
	float ao = 0.0f;
	
	static const float rad = SSAO_Options[0];

	static const int iterations = 4;
	float2 coord1;
	float2 coord2;

	[unroll]
	for (int i = 0; i < iterations; ++i) {
		coord1 = reflect(vec[i], r) * rad;
		coord2 = float2((coord1.x * 0.707f) - (coord1.y * 0.707f),
										(coord1.x * 0.707f) + (coord1.y * 0.707f));
		
		ao += doOclussion(wUVScale, coord1 * 0.25f, p, n);
		ao += doOclussion(wUVScale, coord2 * 0.50f, p, n);
		ao += doOclussion(wUVScale, coord1 * 0.75f, p, n);
		ao += doOclussion(wUVScale, coord2 * 1.00f, p, n);
	}
	
	ao *= 0.0625f;
	ao  = 1.0f - ao;
	
	SSAOTex[uv] = float4(ao.rrr, 1.0f);
	return;
}