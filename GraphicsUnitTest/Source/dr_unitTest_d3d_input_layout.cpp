#pragma once
#include <dr_d3d_input_layout.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>

TEST(D3DInputLayout, create) {
  driderSDK::D3DInputLayout obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  std::vector<driderSDK::DrInputElementDesc> desc;
  driderSDK::DrInputElementDesc n;
  n.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  n.offset = 0;
  n.inputSlot = 0;
  n.semanticIndex = 0;
  n.semanticName = "POSITION";
  desc.push_back(n);
  char init[10];
  EXPECT_TRUE(!obj.create(device,desc));
}

TEST(D3DInputLayout, set) {
  driderSDK::D3DInputLayout obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  std::vector<driderSDK::DrInputElementDesc> desc;
  driderSDK::DrInputElementDesc n;
  n.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  n.offset = 0;
  n.inputSlot = 0;
  n.semanticIndex = 0;
  n.semanticName = "POSITION";
  desc.push_back(n);
  char init[10];
  obj.create(device, desc);
  obj.set(context);
  EXPECT_TRUE(true);
}

TEST(D3DInputLayout, release) {
  driderSDK::D3DInputLayout obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);
  std::vector<driderSDK::DrInputElementDesc> desc;
  driderSDK::DrInputElementDesc n;
  n.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
  n.offset = 0;
  n.inputSlot = 0;
  n.semanticIndex = 0;
  n.semanticName = "POSITION";
  desc.push_back(n);
  char init[10];
  obj.create(device, desc);
  obj.release();
  EXPECT_TRUE(obj.APILayout);
}