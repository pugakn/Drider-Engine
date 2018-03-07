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
  float4 Color			: SV_TARGET0;
	//float4 Position		: SV_TARGET1;
	//float4 Normal			: SV_TARGET2;
};

PS_OUTPUT FS(PS_INPUT input) {
	PS_OUTPUT outRT;
  
	float2 uv = input.Texcoord;

	float4 color = Texture0.Sample(SS, uv);
	float4 pos = input.Position;
	float3 n = input.Normal.xyz;

	outRT.Color 	 = float4(1, 1, 1, 1);
	//outRT.Position = float4(1, 0, 1, 1);
	//outRT.Normal   = float4(0, 1, 1, 1);

	return outRT;
}
