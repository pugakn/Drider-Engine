SamplerState SS : register(s0);

Texture2D ShadowSt					: register(t0);
Texture2D ShadowNd					: register(t1);
Texture2D ShadowRd					: register(t2);
Texture2D ShadowLt 					: register(t3);

RWTexture2D<float4> CompressedShadowTex : register(u0);

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint groupIndex			: SV_GroupIndex) {

	float2 uv = float2(dispatchID.x, dispatchID.y);
	
	float2 uvScale = float2(dispatchID.x / (float)TXWIDTH,
													dispatchID.y / (float)TXHEIGHT);

	CompressedShadowTex[uv] = float4(ShadowSt.SampleLevel(SS, uvScale, 0).x,
												 					 ShadowNd.SampleLevel(SS, uvScale, 0).x,
												 					 ShadowRd.SampleLevel(SS, uvScale, 0).x,
												 					 ShadowLt.SampleLevel(SS, uvScale, 0).x);
	
	return;
}