#pragma once

#include <dr_transform.h>
#include <vector>
#include <dr_memory.h>
#include <dr_model.h>
#include <dr_d3d_vertex_buffer.h>
#include <dr_d3d_index_buffer.h>
#include <dr_d3d_constant_buffer.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
//#include <dr_matrix4x4.h>
#include <dr_d3d_vertex_shader.h>
#include <dr_d3d_fragment_shader.h>
#include <dr_d3d_input_layout.h>
#include <dr_matrix4x4.h>
#include <dr_camera.h>
#include <dr_animator.h>
#include <dr_skeleton.h>

class Model3D 
{
 public:
  enum S 
	{
		Left_Bottom_Front,
		Left_Bottom_Back,
		Left_Top_Front,
		Left_Top_Back,
		Right_Bottom_Front,
		Right_Bottom_Back,
		Right_Top_Front,
		Right_Top_Back
	};

  Model3D();
  void init(const driderSDK::TString& filename);
  void destroy();
  void update();
  void draw(const driderSDK::Camera& wvp);

  driderSDK::Transform transform;

  float elapsedTime;
 private:
  struct CBuffer {
    driderSDK::Matrix4x4 WVP;
    driderSDK::Matrix4x4 World;
    driderSDK::Matrix4x4 Bones[200];
  };

  std::shared_ptr<driderSDK::Model> resource;

  CBuffer constBuff;

  struct MeshGFX {
    driderSDK::VertexBuffer* VB;
    driderSDK::IndexBuffer* IB;
  };

  std::vector<MeshGFX> meshesGFX;

  driderSDK::ConstantBuffer* CB;

  driderSDK::Shader* vs;
  driderSDK::Shader* fs;

  driderSDK::InputLayout* IL;

  std::vector<driderSDK::Vertex> vertices;
  std::vector<driderSDK::UInt32> indices;

  driderSDK::Animator animator;
  std::shared_ptr<driderSDK::Skeleton> pSkeleton;
};
