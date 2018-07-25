//#define QUALITY_BLUR
#define NORMAL_BLUR
//#define PERFORMANCE_BLUR

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
};

SamplerState SS : register(s0);

Texture2D inTex : register(t0);

RWTexture2D<float4> outTex : register(u0);

#if defined(QUALITY_BLUR)
  #define HALF_KERNEL_SIZE 15
  static const float BlurWeights[31] = { 0.01279f, 0.015149f, 0.017736f, 0.020523f, 0.023472f, 0.026533f, 0.029646f, 0.032739f, 0.035735f, 0.038553f, 0.04111f, 0.043328f, 0.045135f, 0.046472f, 0.047293f, 0.04757f, 0.047293f, 0.046472f, 0.045135f, 0.043328f, 0.04111f, 0.038553f, 0.035735f, 0.032739f, 0.029646f, 0.026533f, 0.023472f, 0.020523f, 0.017736f, 0.015149f, 0.01279f };
#elif defined(NORMAL_BLUR)
  #define HALF_KERNEL_SIZE 6
  static const float BlurWeights[13] =  { 0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f, 0.026995f, 0.008764f, 0.002216f };
#elif defined(PERFORMANCE_BLUR)
  #define HALF_KERNEL_SIZE 2
  static const float BlurWeights[5] =  { 0.197669f, 0.201161f, 0.202339f, 0.201161f, 0.197669f };
#endif

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
  const float2 uv = float2(dispatchID.x, dispatchID.y);
	
	const float2 uvScale = float2(dispatchID.x / fViewportDimensions.x,
																dispatchID.y / fViewportDimensions.y);

  static const float offset = 1.0f / fViewportDimensions.x;
  float4 shadowVal = float4(0.0f, 0.0f, 0.0f, 0.0f);
  
  float2 sampleCoords;
  
  [unroll]
  for (int i = -HALF_KERNEL_SIZE; i <= HALF_KERNEL_SIZE; ++i) {
    sampleCoords = float2(uvScale.x + (i * offset), uvScale.y);
    shadowVal += inTex.SampleLevel(SS, sampleCoords.xy, 0) * BlurWeights[i + HALF_KERNEL_SIZE];
  }
  
  outTex[uv] = shadowVal;
}