#pragma once
#include <dr_d3d_index_buffer.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DIndexBuffer, constructor) {
  EXPECT_TRUE(true);
}

//TEST(D3DIndexBuffer, create) {
//  driderSDK::D3DIndexBuffer vb;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  driderSDK::DrBufferDesc desc;
//  desc.sizeInBytes = 10;
//  desc.stride = 0;
//  desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
//  char init[10];
//  EXPECT_TRUE(!vb.create(device, desc, init));
//}
//
//TEST(D3DIndexBuffer, set) {
//  driderSDK::D3DIndexBuffer vb;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  driderSDK::DrBufferDesc desc;
//  desc.sizeInBytes = 10;
//  desc.stride = 0;
//  desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
//  char init[10];
//  vb.create(device, desc, init);
//  vb.set(context, 0);
//  EXPECT_TRUE(true);
//}
//
//TEST(D3DIndexBuffer, updateFromMemory) {
//  EXPECT_TRUE(false);
//}

//TEST(D3DIndexBuffer, release) {
//  driderSDK::D3DIndexBuffer vb;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  driderSDK::DrBufferDesc desc;
//  desc.sizeInBytes = 10;
//  desc.stride = 0;
//  desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
//  char init[10];
//  vb.create(device, desc, init);
//  vb.release();
//  EXPECT_TRUE(!vb.IB);
//}