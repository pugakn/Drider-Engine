#define PI 3.14152265f

cbuffer ConstantBuffer {
  float4   CameraInfo; //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
  float4x4 ViewProjectionInverse;
  float4x4 SkyboxRotation;
};

SamplerState SS;

Texture2D TexEnviroment : register(t0);
Texture2D TexIrradiance : register(t1);

struct PS_INPUT {
  float4 Position   : SV_POSITION;
  float3 Direction  : TEXCOORD0;
};

struct PS_OUTPUT {
  float4 PositionLDepth	: SV_TARGET0;
  float4 NormCoC	      : SV_TARGET1;
	float4 Albedo_M	      : SV_TARGET2;
	float4 Emissive_R     : SV_TARGET3;
};

float3
SkyboxColor(float3 direction) {
  direction = normalize(direction);
  direction = normalize(mul(SkyboxRotation, float4(direction, 1.0f)).xyz);

  float2 longlat = float2(atan2(direction.x , direction.z), acos(direction.y));
  
  float2 uv = longlat / float2(2.0f * PI, PI);
  uv.x += 0.5f;
  
  return TexEnviroment.Sample(SS, uv).xyz;
}

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT output = (PS_OUTPUT)0;

  output.PositionLDepth	= float4(0.0f, 0.0f, 0.0f, 1.0f);
  output.NormCoC	      = float4(0.0f, 0.0f, 0.0f, 1.0f);
	output.Albedo_M	      = float4(0.0f, 0.0f, 0.0f, 1.0f);
	output.Emissive_R     = float4(SkyboxColor(input.Direction), 1.0f);
  
  return output;
}