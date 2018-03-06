Texture2D Texture0 : register(t0);

SamplerState SS;

cbuffer ConstantBuffer {
	float4x4 WVP;
	float4x4 World;
  float4x4 Bones[200];
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float3 Normal   : NORMAL0;
  float3 Binormal : BINORMAL0;
  float3 Tangent  : TANGENT0;
  float2 Texcoord : TEXCOORD0;
};

struct PS_OUTPUT {
  float4 Color			: COLOR0;
	float4 Position		: COLOR1;
	float4 Normal			: COLOR2;
};

PS_OUTPUT FS(PS_INPUT input) : SV_TARGET  {
	PS_OUTPUT outRT;
  
	float4 pos = input.Position;
  float2 uv = input.Texcoord;
	float3 n = input.Normal.xyz;
	float4 color = Texture0.Sample(SS, uv);

	outRT.Color = color;
	outRT.Position = float4(pos.xyz, 1.0f);
	outRT.Normal = float4(n, 1.0f);

	return outRT;
  //return float4(n, 1);
}
