cbuffer ConstantBuffer {
	float4x4 World;
  float4x4 WVP;
  float4   CameraPosition;
  float4   vertice[8];
};

SamplerState SS;

Texture2D FrontWallTex  : register(t0);
Texture2D LeftWallTex   : register(t1);
Texture2D RightWallTex  : register(t2);
Texture2D CeilTex       : register(t3);
Texture2D FloorTex      : register(t4);

struct PS_INPUT {
  float4   Position : SV_POSITION;
  float2   Texcoord : TEXCOORD0;
  float4   RealPos  : TEXCOORD1;
  float3x3 TBN      : TEXCOORD2;
};

struct PS_OUTPUT {
  float4 Color	: SV_TARGET0;
};

#define EPSILON 1e-6f

bool
LineVSSquare(const in float3 rayOrigin,
             const in float3 rayDir,
             const in float3 vertex1,
             const in float3 vertex2,
             const in float3 vertex3,
             out float2 uvOut) {
  //1: Calculate the plane normal
  float3 vertex1to2 = vertex2 - vertex1;
  float3 vertex1to3 = vertex3 - vertex1;
  float3 planeNormal = cross(vertex1to2, vertex1to3);
  
  //2: Check if ray intersects the plane
  float rayDotPlane = dot(planeNormal, rayDir);
  
  if (abs(rayDotPlane) < EPSILON) {
    //The ray it's parallel to the plane
    return false;
  }
  
  //3: Get point in plane.
  float t = -dot(planeNormal, rayOrigin - vertex1) * rcp(rayDotPlane);
  float3 pointInPlane = rayOrigin + (rayDir * t);
  
  //4: Project the point in plane on quad edges to get the UV value.
  float3 vecVertex1ToPointInPlane = pointInPlane - vertex1;
  uvOut.x = dot(vecVertex1ToPointInPlane, vertex1to2) * rcp(dot(vertex1to2, vertex1to2));
  uvOut.y = dot(vecVertex1ToPointInPlane, vertex1to3) * rcp(dot(vertex1to3, vertex1to3));
  
  //5: Chek if the point is inside the quad.
  return (uvOut.x >= 0.0f && uvOut.x <= 1.0f) &&
         (uvOut.y >= 0.0f && uvOut.y <= 1.0f);
}

PS_OUTPUT
FS(PS_INPUT input) {
	PS_OUTPUT output = (PS_OUTPUT)0;
  
  float3 viewDir = normalize(input.RealPos - CameraPosition).xyz;
  float3 startRay = input.RealPos.xyz;
  
  float2 uv = float2(0.0f, 0.0f);

  bool fake_left  = LineVSSquare(startRay, viewDir, vertice[0].xyz, vertice[4].xyz, vertice[2].xyz, uv);
  //if (fake_left)  { output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f); output.Color = float4(uv, 0.0f, 1.0f); return output; }
  if (fake_left) { output.Color = LeftWallTex.Sample(SS, uv); return output; }

  bool fake_front = LineVSSquare(startRay, viewDir, vertice[4].xyz, vertice[5].xyz, vertice[6].xyz, uv);
  //if (fake_front) { output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f); output.Color = float4(uv, 0.0f, 1.0f); return output; }
  if (fake_front)  { output.Color = FrontWallTex.Sample(SS, uv); return output; }

  bool fake_right = LineVSSquare(startRay, viewDir, vertice[5].xyz, vertice[1].xyz, vertice[7].xyz, uv);
  //if (fake_right) { output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f); output.Color = float4(uv, 0.0f, 1.0f); return output; }
  if (fake_right) { output.Color = RightWallTex.Sample(SS, uv); return output; }

  bool fake_ceil  = LineVSSquare(startRay, viewDir, vertice[0].xyz, vertice[1].xyz, vertice[4].xyz, uv);
  //if (fake_ceil)  { output.Color = float4(1.0f, 1.0f, 1.0f, 1.0f); output.Color = float4(uv, 0.0f, 1.0f); return output; }
  if (fake_ceil)  { output.Color = CeilTex.Sample(SS, uv); return output; }

  bool fake_floor = LineVSSquare(startRay, viewDir, vertice[6].xyz, vertice[7].xyz, vertice[2].xyz, uv);
  //if (fake_floor) { output.Color = float4(0.5f, 0.5f, 0.5f, 1.0f); output.Color = float4(uv, 0.0f, 1.0f); return output; }
  if (fake_floor) { output.Color = FloorTex.Sample(SS, uv); return output; }

  //output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
  
  return output;
}