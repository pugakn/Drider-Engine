#define DR_PARTICLES_CPU

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float4 color : COLOR;
};

#ifdef DR_PARTICLES_GPU
struct BufType
{
  float4 color;
};

StructuredBuffer<BufType> BufferOut : register(t0);
#endif
float4
FS(PS_INPUT input) : SV_TARGET0 {
#ifdef DR_PARTICLES_CPU
  return float4(input.color.xyz, 1.0f);
#endif
}