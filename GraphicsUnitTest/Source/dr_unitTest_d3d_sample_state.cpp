#pragma once
#include <dr_d3d_sample_state.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3D11SamplerState, create) {
  driderSDK::D3D11SamplerState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);

  driderSDK::DrSampleDesc desc;
  EXPECT_TRUE(!obj.create(device, desc));

}

TEST(D3D11SamplerState, set) {
  driderSDK::D3D11SamplerState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);

  driderSDK::DrSampleDesc desc;
  bool b = obj.create(device, desc);
  obj.set(context, driderSDK::DR_SHADER_TYPE_FLAG::kVertex);
  EXPECT_TRUE(!b);
}

TEST(D3D11SamplerState, release) {
  driderSDK::D3D11SamplerState obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);

  driderSDK::DrSampleDesc desc;
  bool b = obj.create(device, desc);
  obj.release();
  EXPECT_TRUE(obj.APIState);
}