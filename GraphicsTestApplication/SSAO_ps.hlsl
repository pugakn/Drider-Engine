Texture2D PositionLDepthTex : register(t0);
Texture2D NormalCoC         : register(t1);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 View;
  float4x4 ViewInv;
  float4x4 Projection;
  float4x4 ProjectionInv;
  float4x4 VP;
  float4x4 VPInv;
};

static const float SampleRadio = 0.0008f;
static const float Intensity = 1.0f;
static const float Scale = 1.0f;
static const float Bias = 0.0002f;

float3
getPosition(in float2 uv) {
  return PositionLDepthTex.Sample(SS, uv).xyz;
}

float3
getNormal(float2 uv) {
  return NormalCoC.Sample(SS, uv).xyz;
};

float3
getRandom(float2 uv) {
  float nx = frac(sin(dot(uv, float2(15.8989f, 76.133f) * 1.0f)) * 46736.23457f);
  float ny = frac(sin(dot(uv, float2(11.9899f, 62.223f) * 1.0f)) * 34748.34744f);
  float nz = frac(sin(dot(uv, float2(13.3238f, 63.122f) * 1.0f)) * 59998.47332f);
  
  return normalize(float3(nx, ny, nz));
};

float
doOclussion(float2 texCoord, float2 uv, float3 p, float3 cnorm) {
  float3 diff = getPosition(texCoord + uv) - p;
  float3 v = normalize(diff);
  float  d = length(diff) * Scale;
  
	return max(0.0f, dot(cnorm, v) - Bias) * (1.0f / (1.0f + d)) * Intensity;
};

static const float2 vec[4] = {
  float2(-1.0f,  0.0f),
  float2( 1.0f,  0.0f),
  float2( 0.0f, -1.0f),
  float2( 0.0f,  1.0f)
};

struct PS_INPUT {
	float4 Position : SV_POSITION;
	float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
	float4 SSAO : SV_TARGET0;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT outRT;

	float2 uv = input.Texcoord;

  float3 p = getPosition(uv);
  float3 n = getNormal(uv);
  float2 r = getRandom(uv).xy;
	float ao = 0.0f;
	
	//float rad = SampleRadio / p.z;
  float rad = SampleRadio;
	//float rad = SampleRadio / abs(max(100, p.z));

	static const int iterations = 4;
	[unroll]
	for (int i = 0; i < iterations; ++i) {
		float2 coord1 = reflect(vec[i], r) * rad;
		float2 coord2 = float2(coord1.x * 0.707f - coord1.y * 0.707f,
													 coord1.x * 0.707f + coord1.y * 0.707f);
													 
		ao += doOclussion(uv, coord1 * 0.25f, p, n);
		ao += doOclussion(uv, coord2 * 0.50f, p, n);
		ao += doOclussion(uv, coord1 * 0.75f, p, n);
		ao += doOclussion(uv, coord2 * 1.00f, p, n);
	}
	
	ao *= 0.0625f;
	ao  = 1.0f - ao;
	
	outRT.SSAO = float4(ao.rrr, 1.0f);
	
	return outRT;
}