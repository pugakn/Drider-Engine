Texture2D ShadowSt : register(t0);
Texture2D ShadowNd : register(t1);
Texture2D ShadowRd : register(t2);
Texture2D ShadowLt : register(t3);

SamplerState SS : register(s0);

RWTexture2D<float4> ShadowTex : register(u0);

[numthreads(1, TXHEIGHT, 1)]
void
CS(uint3 id : SV_DispatchThreadID) {

	float2 uv = float2(0.0f, id.y / TXHEIGHT);
	const float wJump = 1.0f / TXWIDTH;
	
	for (int i = 0; i < TXWIDTH; ++i) {
		ShadowTex[uv] = float4(1.0f,
													 uv.y * TXHEIGHT,
													 0.0f,
													 1.0f);
													 /*
		ShadowTex[uv] = float4(ShadowSt.Sample(SS, uv).x,
													 ShadowNd.Sample(SS, uv).x,
													 ShadowRd.Sample(SS, uv).x,
													 ShadowLt.Sample(SS, uv).x);
													 */
		uv.x += wJump;
	}
}