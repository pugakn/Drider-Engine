#pragma once
#include <dr_d3d_render_target.h>
#include <dr_d3d_depth_stencil.h>
//#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

//TEST(D3DRenderTarget, create) {
//  driderSDK::D3DRenderTarget obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  driderSDK::DrTextureDesc desc;
//  desc.width = 1920;
//  desc.pitch = 1920 * 4;
//  desc.height = 1080;
//  desc.mipLevels = 1;
//  desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
//  desc.CPUAccessFlags = 0;
//
//  EXPECT_TRUE(!obj.create(device,desc,1));
//}
//
//TEST(D3DRenderTarget, set) {
//  driderSDK::D3DRenderTarget obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  driderSDK::DrTextureDesc desc;
//  desc.width = 1920;
//  desc.pitch = 1920 * 4;
//  desc.height = 1080;
//  desc.mipLevels = 1;
//  desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
//  desc.CPUAccessFlags = 0;
//  bool b = obj.create(device, desc, 1);
//
//  driderSDK::D3DDepthStencil obj2;
//  desc.width = 1920;
//  desc.pitch = 1920 * 4;
//  desc.height = 1080;
//  desc.mipLevels = 1;
//  desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
//  desc.CPUAccessFlags = 0;
//  bool b2 = obj2.create(device, desc);
//  
//  if (!b || !b2)
//  {
//    obj.set(context,obj2);
//    EXPECT_TRUE(true);
//  }
//  else
//    EXPECT_TRUE(false);
//}
//
//TEST(D3DRenderTarget, release) {
//  driderSDK::D3DRenderTarget obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  driderSDK::DrTextureDesc desc;
//  desc.width = 1920;
//  desc.pitch = 1920 * 4;
//  desc.height = 1080;
//  desc.mipLevels = 1;
//  desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
//  desc.CPUAccessFlags = 0;
//
//  EXPECT_TRUE(!obj.create(device, desc, 1));
//}