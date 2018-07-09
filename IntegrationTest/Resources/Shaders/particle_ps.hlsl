struct PS_INPUT {
  float4 Position : SV_POSITION;
  float4 color : COLOR;
};
float4
FS(PS_INPUT input) : SV_TARGET0 {
  return float4(input.color.xyz, 1.0f);
}