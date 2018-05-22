cbuffer ConstantBuffer {
  float4x4 WVP;
  float4 color;
};

struct VS_INPUT {
  float4 Position     : POSITION;
  float4 Normal       : NORMAL;
  float4 Binormal     : BINORMAL;
  float4 Tangent      : TANGENT;
  float4 BonesWeights : BONEWEIGHTS;
  int4   BonesIDs     : BONEIDS;
  float2 Texcoord     : TEXCOORD;
};

struct VS_OUTPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT output;
  
  //output.Position   =  input.Position;
  //output.Position.x = -output.Position.x;

  //float4x4 worldViewMatrix = mul(world, view);
  //float4x4 vp = mul(view, projection);
  //float3 positionVS = psOut.Position + float3(worldViewMatrix._41, worldViewMatrix._42, worldViewMatrix._43);
  //float3 positionVS = mul((float3x3)worldViewMatrix,psOut.Position.xyz);
  //psOut.Position = mul(projection, float4(positionVS, 1.0f));
  //psOut.Position = mul(WVP, psOut.Position);
  //psOut.Position = mul(vp,psOut.Position.xyzw);
  //psOut.Position.w = 1;

  output.Position = mul(WVP, float4(input.Position.xyz,1));
  output.Texcoord  = 1.0f - input.Texcoord;
  
  return output;
}