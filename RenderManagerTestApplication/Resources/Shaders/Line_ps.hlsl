cbuffer ConstantBuffer {
  float4x4 VP;
  float4   LineColor;
  float4   CameraInfo; //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
};

struct PS_INPUT {
  float4   Position : SV_POSITION;
};

struct PS_OUTPUT {
  float4 PositionLDepth	: SV_TARGET0;
  float4 NormCoC	      : SV_TARGET1;
	float4 Albedo_M	      : SV_TARGET2;
	float4 Emissive_R     : SV_TARGET3;
};

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT output;
  
  output.PositionLDepth  = float4(input.Position.xyz, 0.0f);
  output.NormCoC         = float4(0.0f.xxx, 1.0f);
  output.Albedo_M        = float4(LineColor.xyz, 0.0f);
  output.Emissive_R      = float4(LineColor.xyz, 1.0f);
  
  return output;
}