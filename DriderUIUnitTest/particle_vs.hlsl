
struct VS_INPUT {
  //Vertex
  float4 Position     : POSITION;

  //Instance
  float4 InstancePos0 : TRANSFORM0;
  float4 InstancePos1 : TRANSFORM1;
  float4 InstancePos2 : TRANSFORM2;
  float4 InstancePos3 : TRANSFORM3;

  float4 color : COLOR;
};

struct VS_OUTPUT {
  float4 Position : SV_POSITION;
  float4 color : COLOR;
};

VS_OUTPUT
VS(VS_INPUT input) {
  VS_OUTPUT output;
  float4x4 WVP = float4x4(input.InstancePos0, input.InstancePos1, input.InstancePos2, input.InstancePos3);
  //output.Position   =  input.Position;
  //output.Position.x = -output.Position.x;

  output.Position = mul(float4(input.Position.xyz,1), WVP);
  output.color = input.color;
  return output;
}