#pragma once
#include <dr_d3d_vertex_buffer.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>
#include <gtest\gtest.h>

TEST(D3DVertexBuffer, constructor) {
}

TEST(D3DVertexBuffer, create) {
  driderSDK::D3DVertexBuffer vb;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrBufferDesc desc;
  desc.sizeInBytes = 10;
  desc.stride = 0;
  desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
  char init[10];
  EXPECT_TRUE(!vb.create(device, desc, init));
}

TEST(D3DVertexBuffer, set) {
  driderSDK::D3DVertexBuffer vb;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  EXPECT_TRUE(true);
}

TEST(D3DVertexBuffer, updateFromMemory) {
  EXPECT_TRUE(false);
}

TEST(D3DVertexBuffer, release) {
  driderSDK::D3DVertexBuffer vb;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrBufferDesc desc;
  desc.sizeInBytes = 10;
  desc.stride = 0;
  desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
  char init[10];
  vb.create(device, desc, init);
  vb.release();
  EXPECT_TRUE(!vb.VB.Get());
}