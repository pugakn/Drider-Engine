Texture2D ShadowSt : register(t0);
Texture2D ShadowNd : register(t1);
Texture2D ShadowRd : register(t2);
Texture2D ShadowLt : register(t3);

SamplerState SS : register(s0);

RWTexture2D<float4> ShadowTex : register(u0);

[numthreads(32, 32, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint groupIndex			: SV_GroupIndex) {

	float2 uv = float2(dispatchID.x, dispatchID.y);
	
	float2 wUVScale = float2(dispatchID.x / (float)TXWIDTH,
													 dispatchID.y / (float)TXHEIGHT);

	ShadowTex[uv] = float4(ShadowSt.SampleLevel(SS, wUVScale, 0).x,
												 ShadowNd.SampleLevel(SS, wUVScale, 0).x,
												 ShadowRd.SampleLevel(SS, wUVScale, 0).x,
												 ShadowLt.SampleLevel(SS, wUVScale, 0).x);

	return;
}