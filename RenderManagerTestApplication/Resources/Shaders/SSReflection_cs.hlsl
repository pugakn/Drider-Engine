#include "Resources\\Shaders\\PBR_Math.hlsl"

cbuffer ConstantBuffer : register(b0) {
  float4 fViewportDimensions;
  float4 kEyePosition;        //XYZ: EyePosition, W: Active Lights
  float4 CameraInfo; //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
  float4x4 CameraVP;
  float4 threadsInfo; //X: Number of thread groups in x, Y: Number of thread groups in Y, Z: Number of Tiles in X, W: Number of Tiles in Y
};

SamplerState SS : register(s0);

Texture2D PositionDepthTex     : register(t0);
Texture2D NormalCoCTex         : register(t1);
Texture2D ColorTex             : register(t2);

RWTexture2D<float4> Reflection  : register(u0);

float2
PositionToUV(const float4 inPos) {
  float4 transformedPos = mul(CameraVP, inPos);
  transformedPos.xy *= rcp(transformedPos.w);

  float2 uv = uv = 0.5f + (0.5f * transformedPos.xy);
  uv.y = 1.0 - uv.y;
  
  uv.x = saturate(uv.x);
  uv.y = saturate(uv.y);

  return uv;
}

float
getPositionDepth(const float4 inPos) {
  const float4 transformedPos = mul(CameraVP, inPos);

  return (transformedPos.z * rcp(CameraInfo[3]));
}

float
getCameraDepth(const float4 inPos) {
  float2 uv = PositionToUV(inPos);

  return PositionDepthTex.SampleLevel(SS, uv, 1).w;
}

float3
getColorFromPosition(const float4 posSample) {
  float2 uv = PositionToUV(posSample);

  return ColorTex.SampleLevel(SS, uv, 1).rgb;
}

float3
SSReflectDetail(const float4 startPos,
                const float4 finalPos,
                const int qualitySteps,
                const float baseDepth) {
  static const float reflectionBias = 0.005f;
  
  float4 currentPosition;
  float stepProportion = 0.0f;
  
  float positionDepth;
  float cameraDepth;
  
  const float proportionScale = 1.0f / qualitySteps;

  [loop]
  for (int i = 0; i < qualitySteps; ++i) {
    stepProportion = (i + 1.0f) * proportionScale;
    currentPosition = lerp(startPos, finalPos, stepProportion);

    //stepProportion = (i + 1.0f) * proportionScale;
    //currentPosition = lerp(startPos, finalPos, 1.0f - stepProportion);

    positionDepth = getPositionDepth(currentPosition);
    cameraDepth = getCameraDepth(currentPosition);
    
    //if (positionDepth > cameraDepth) {
    //if ((positionDepth - cameraDepth) < reflectionBias) {
    //if (positionDepth > baseDepth) {
    if ((positionDepth - baseDepth) < reflectionBias) {
      return getColorFromPosition(currentPosition);
    }
  }

  return (0.0f).xxx;
}

#define NUMTHREADS_X 8
#define NUMTHREADS_Y 4
[numthreads(NUMTHREADS_X, NUMTHREADS_Y, 1)]
void
CS(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID				: SV_GroupID,
	 uint3 dispatchID			: SV_DispatchThreadID,
	 uint  groupIndex			: SV_GroupIndex) {
  
  const float2 uvScale = float2(dispatchID.x, dispatchID.y);
	
	const float2 uv = float2(dispatchID.x * rcp(fViewportDimensions.x),
                           dispatchID.y * rcp(fViewportDimensions.y));

  const float4  position    = float4(PositionDepthTex.SampleLevel(SS, uv, 0).xyz, 1.0f);
  const float3  normal      = NormalCoCTex.SampleLevel(SS, uv, 0).xyz;

  const float3 ViewDir = normalize(position.xyz - kEyePosition.xyz);
  

  static const float reflectionBias = 1.0f;
  static const float stepSize = 4.0f;
  static const int nSteps = 125;

  const float3 reflectDir = normalize(reflect(ViewDir, normal));
  const float4 reflectStep = float4(reflectDir * stepSize, 0.0f);
  
  float4 currentPosition = position;
  float positionDepth = 0.0f;
  float cameraDepth = 0.0f;

  float reflectColor = (0.0f).xxxx;

  [loop]
  for (int i = 0; i < nSteps; ++i) {
    currentPosition += reflectStep;

    positionDepth = getPositionDepth(currentPosition);
    cameraDepth = getCameraDepth(currentPosition);
    
    if (positionDepth > cameraDepth) {
      if ((positionDepth - cameraDepth) < reflectionBias) {
        //reflectColor = getColorFromPosition(currentPosition);
        reflectColor = SSReflectDetail(currentPosition - reflectStep,
                                       currentPosition,
                                       10, cameraDepth);
        reflectColor *= 1.0f - ((i * rcp(nSteps)));
        break;
      }
    }
  }
  
  Reflection[uvScale] = reflectColor;
  return;
}