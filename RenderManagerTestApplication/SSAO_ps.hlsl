Texture2D Position  : register(t0);
Texture2D Normal    : register(t1);

SamplerState SS;

//static const float SampleRadio = 1.08f;
//static const float Intensity = 2.0f;
//static const float Scale = 1.0f;
//static const float Bias = 0.002f;
static const float SampleRadio = 3.00000;
static const float Intensity = 3.00000;
static const float Scale = 1.00000;
static const float Bias = 0.00200;

float4
getPosition(float2 uv) {
  return Position.Sample(SS, uv);
};

float3
getNormal(float2 uv) {
  return Normal.Sample(SS, uv).xyz;
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
  float3 diff = getPosition(texCoord + uv).xyz - p;
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
	float4 SSAO      : SV_TARGET0;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
	
	float2 uv = input.Texcoord;

  float4 p = getPosition(uv);
  float3 n = getNormal(uv);
  float2 r = getRandom(uv).xy;
	float ao = 0.0f;
  
	float rad = SampleRadio / max(100.0f, abs(p.z));
	//float rad = SampleRadio;

	static const int iterations = 4;
	[unroll]
	for (int i = 0; i < iterations; ++i) {
		float2 coord1 = reflect(vec[i], r) * rad;
		float2 coord2 = float2(coord1.x * 0.707 - coord1.y * 0.707,
													 coord1.x * 0.707 + coord1.y * 0.707);
													 
		ao += doOclussion(uv, coord1 * 0.25f, p.xyz, n);
		ao += doOclussion(uv, coord2 * 0.50f, p.xyz, n);
		ao += doOclussion(uv, coord1 * 0.75f, p.xyz, n);
		ao += doOclussion(uv, coord2 * 1.00f, p.xyz, n);
	}
	
	ao *= 0.0625f;
	ao = 1.0f - ao;
	
	outRT.SSAO = float4(ao, ao, ao, 1.0f);
	//outRT.SSAO = float4(rad, rad, rad, 1.0f);
	//outRT.SSAO = float4(r.xxx, 1.0f);
	//outRT.SSAO = getPosition(uv);
	//outRT.SSAO = getPosition(uv);
	//outRT.SSAO = float4(getNormal(uv).xyz, 1.0f);
	//outRT.SSAO = float4(getRandom(uv), 1.0f);
	
	return outRT;
}