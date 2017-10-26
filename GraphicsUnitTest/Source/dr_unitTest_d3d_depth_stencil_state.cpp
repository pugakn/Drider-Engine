#pragma once
#include <dr_d3d_depth_stencil_state.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DDepthStencilState, create) {
 /* driderSDK::D3DDepthStencilState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrDepthStencilDesc desc;
  desc.depthEnable = true;
  desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;*/
  EXPECT_TRUE(/*!obj.create(device, desc)*/false);
}

TEST(D3DDepthStencilState, set) {
  //driderSDK::D3DDepthStencilState obj;
  //driderSDK::D3DDevice device;
  //driderSDK::D3DDeviceContext context;
  //device.createDeviceAndDeviceContext(context);
  //driderSDK::DrDepthStencilDesc desc;
  //desc.depthEnable = true;
  //desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;
  //obj.create(device, desc);
  //obj.set(context,1);
  EXPECT_TRUE(false);
}

TEST(D3DDepthStencilState, release) {
  //driderSDK::D3DDepthStencilState obj;
  //driderSDK::D3DDevice device;
  //driderSDK::D3DDeviceContext context;
  //device.createDeviceAndDeviceContext(context);
  //driderSDK::DrDepthStencilDesc desc;
  //desc.depthEnable = true;
  //desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;
  //obj.create(device, desc);
  //obj.release();
  EXPECT_TRUE(/*!obj.APIState.Get()*/false);
}
