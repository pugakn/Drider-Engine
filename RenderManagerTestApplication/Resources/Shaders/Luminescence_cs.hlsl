SamplerState SS : register(s0);

Texture2D TextureIn : register(t0);

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions; //X: TextureIn width; Y: TextureIn height; Z: LuminiscenceDelta;
  float4 threadsInfo; //X: Number of groups in X, Y: Number of groups in Y, Z: width divisions, Y: Height divisions
};

RWStructuredBuffer<float> GroupLuminescence : register(u0);
//RWStructuredBuffer<float4> AverageLuminescence  : register(u1);

float
luminescence(float3 Color, float LuminiscenceDelta) {
  static const float3 LuminanceFactor = float3(0.3f, 0.59f, 0.03f);
  //Case LDR
  return (dot(Color, LuminanceFactor) + LuminiscenceDelta);
  //Case HDR
  return log(dot(Color, LuminanceFactor) + LuminiscenceDelta);
}

[numthreads(1, 1, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
	float2 wUVScale;
  const uint group = (groupID.y * threadsInfo.x) + groupID.x;
  
  //Todos los grupos deben de hacer este calculo.
  //Hacer la sumatoria de luminiscencia de cada pixel dentro de este grupo,
  
  float thisGroupLuminance = 0.0f;
  float3 actualPixel;
  
  const float offsettedX = dispatchID.x * threadsInfo.z;
  const float offsettedY = dispatchID.y * threadsInfo.w;

  [loop]
  for (int pxGroup_X = 0; pxGroup_X < threadsInfo.z; ++pxGroup_X) {
    [loop]
    for (int pxGroup_Y = 0; pxGroup_Y < threadsInfo.w; ++pxGroup_Y) {
      wUVScale = float2((offsettedX + pxGroup_X) * rcp(fViewportDimensions.x),
                        (offsettedY + pxGroup_Y) * rcp(fViewportDimensions.y));
      actualPixel = TextureIn.SampleLevel(SS, wUVScale, 0).xyz;
      thisGroupLuminance += luminescence(actualPixel, fViewportDimensions.z);
    }
  }
  
  GroupLuminescence[group] = thisGroupLuminance;
  
  //Todos los grupos de cualquier id.y, y con id.x == 0 deben de hacer este calculo.
  //Hacer la sumatoria de aportacion de todos los grupos en X
  
  if (groupID.x != 0) {
    return;
  }

  AllMemoryBarrierWithGroupSync();

  float thisRowLuminance = 0.0f;
  float rowID = groupID.y * threadsInfo.x;
  
  [loop]
  for (int group_x = 0; group_x < threadsInfo.x; ++group_x) {
    thisRowLuminance += GroupLuminescence[rowID + group_x];
  }

  GroupLuminescence[group] = thisRowLuminance;

  //Solo el grupo id.x == 0, id.y == 0 debe hacer este calculo.
  //Hacer la sumatoria de aportacion de todos los grupos en Y
  
  if (groupID.y != 0) {
    return;
  }

  AllMemoryBarrierWithGroupSync();

  float thisColumnLuminance = 0.0f;

  [loop]
  for (int group_y = 0; group_y < threadsInfo.y; ++group_y) {
    thisColumnLuminance += GroupLuminescence[group_y * threadsInfo.x];
  }

  float totalAverageLuminescence = thisColumnLuminance *  rcp(fViewportDimensions.x * fViewportDimensions.y);
  
  GroupLuminescence[0] = totalAverageLuminescence;

  return;
}