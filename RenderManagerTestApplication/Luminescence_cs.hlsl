Texture2D TextureIn : register(t0);

SamplerState SS : register(s0);

cbuffer ConstantBuffer : register(b0)
{
  float	LuminiscenceDelta;
  float TextureWidth;
  float TextureHeight;
  float pad;
};

RWStructuredBuffer<float4> AverageLuminescence : register(u0);

float
luminescence(float3 Color) {
  static const float3 LuminanceFactor = float3(0.3f, 0.59f, 0.03f);
  //Case LDR
  return (dot(Color, LuminanceFactor) + LuminiscenceDelta);
  //Case HDR
  return log(dot(Color, LuminanceFactor) + LuminiscenceDelta);
}

[numthreads(1, 1, 1)]
void
CS(uint3 id : SV_DispatchThreadID) {
  static const float widthOffset = 1.0f / TextureWidth;
  static const float heightOffset = 1.0f / TextureHeight;
  
  float3 actualPixel = float3(0.0f, 0.0f, 0.0f);
  float totalLuminescence = 0.0f;
  
  float2 uv = float2(0.0f, 0.0f);
  
  for (int txHeight = 0; txHeight < TextureHeight; ++txHeight) {
    uv.x = 0.0f;
    for (int txWidth = 0; txWidth < TextureWidth; ++txWidth) {
      actualPixel = TextureIn.SampleLevel(SS, uv, 0).xyz;
      totalLuminescence += luminescence(actualPixel);
      uv.x += widthOffset;
    }
    uv.y += heightOffset;
  }
  
  totalLuminescence /= TextureWidth * TextureHeight;
  
  AverageLuminescence[0].x = 0.5f;
}