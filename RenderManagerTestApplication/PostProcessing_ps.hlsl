Texture2D AlbedoTex    : register(t0); //1, 0, 0
Texture2D PositionTex  : register(t1); //0, 1, 0
Texture2D NormalTex    : register(t2); //0, 0, 1
Texture2D EmissiveTex  : register(t3); //1, 1, 0
Texture2D MetallicTex  : register(t4); //1, 0, 1
Texture2D RoughnessTex : register(t5); //0, 1, 1
Texture2D SSAOTex      : register(t6); //1, 0, 1
TextureCube Enviroment : register(t7);

SamplerState SS;

cbuffer ConstantBuffer {
	float4 EyePosition;
};

/*
Point Light {
  float4 Position;
  float4 Color;
  float Intensity;
}

Directional Light {
  float4 Direction;
  float4 Color;
  float Intensity;
}

Cone Light {
  float4 Direction;
  float4 Color;
  float Intensity;
}
*/

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float4 FS(PS_INPUT input) : SV_TARGET {
  float2 uv = input.Texcoord;

  float3 normal = NormalTex.Sample(SS, uv);
  float3 position = PositionTex.Sample(SS, uv);
  float3 LightPosition = float3(0.0f, 50.0f, 100.0f);

  float3 ViewDir = normalize(EyePosition.xyz - position);
  float3 LightDir = normalize(LightPosition - position);


  float  NdotV;
  float  NdotH;
  float  VdotH;
  float  NdotL;
  float3 H;
  float  LdotH;
  float  LdotV;

  NdotV = saturate(dot(normal, ViewDir));
  NdotH = saturate(dot(normal, H));
  VdotH = saturate(dot(ViewDir, H));
  //For every light
  NdotL = saturate(dot(normal, LightDir));
  H     = normalize(LightDir + ViewDir);
  LdotH = saturate(dot(LightDir, H));
  LdotV = saturate(dot(LightDir, ViewDir));

  float3 reflectVector = reflect(ViewDir, normal);

  //return Enviroment.Sample(SS, reflectVector);
	//float4 color = AlbedoTex.Sample(SS, uv);
  float4 color = Enviroment.Sample(SS, reflectVector);

  return color;
}