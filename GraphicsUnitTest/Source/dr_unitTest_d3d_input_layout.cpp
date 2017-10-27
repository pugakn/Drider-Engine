#pragma once
#include <dr_d3d_input_layout.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DInputLayout, create) {
  driderSDK::D3DInputLayout obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrInputElementDesc desc;
  desc.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  desc.offset = 0;
  desc.inputSlot = 0;
  desc.semanticIndex = 0;
  desc.semanticName = "POSITION";
  //char init[10];
  EXPECT_TRUE(!obj.create(device,&desc, 1));
}

TEST(D3DInputLayout, set) {
  driderSDK::D3DInputLayout obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrInputElementDesc desc;
  desc.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  desc.offset = 0;
  desc.inputSlot = 0;
  desc.semanticIndex = 0;
  desc.semanticName = "POSITION";
  //char init[10];
  obj.create(device, &desc, 1);
  obj.set(context);
  EXPECT_TRUE(true);
}

TEST(D3DInputLayout, release) {
  driderSDK::D3DInputLayout obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  driderSDK::DrInputElementDesc desc;
  desc.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  desc.offset = 0;
  desc.inputSlot = 0;
  desc.semanticIndex = 0;
  desc.semanticName = "POSITION";
  //char init[10];
  obj.create(device, &desc, 1);
  obj.release();
  EXPECT_TRUE(obj.APILayout.Get());
}