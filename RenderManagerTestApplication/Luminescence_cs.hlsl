Texture2D<float4> BufferIn : register(t0);

cbuffer ListCount1 : register(b0)
{
  float	LuminiscenceDelta;
  uint  TextureWidth;
  uint  TextureHeight;
  float pad;
};

float AverageLuminescence : register(u0);

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
  float totalLuminescence = 0.0f;
  
  float4 actualPixel;
  for (int txWidth = 0; txWidth < TextureWidth; ++txWidth) {
    for (int txHeight = 0; txHeight < TextureHeight; ++txHeight) {
	    actualPixel = BufferIn.Load(uint3(txWidth, txHeight, 1));
	    totalLuminescence += luminescence(actualPixel.xyz);
	  }
  }
  totalLuminescence /= TextureWidth * TextureHeight;
  
  AverageLuminescence = totalLuminescence;
}