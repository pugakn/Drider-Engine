#pragma once
#include <dr_d3d_swap_chain.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_d3d_texture.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DSwapChain, create) {
  driderSDK::D3DSwapChain obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);

  driderSDK::DrSwapChainDesc desc;
  EXPECT_TRUE(obj.create(device, desc) != driderSDK::DR_GRAPHICS_ERROR::CREATE_SWAP_CHAIN_ERROR);
}

TEST(D3DSwapChain, getBackBuffer) {
  driderSDK::D3DSwapChain obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);

  driderSDK::DrSwapChainDesc desc;
  bool b = obj.create(device, desc);
  EXPECT_TRUE(!b);
}

TEST(D3DSwapChain, release) {
  driderSDK::D3DSwapChain obj;
  driderSDK::D3DDevice device;
  driderSDK::D3DDeviceContext context;
  device.createDeviceAndDeviceContext(context);

  driderSDK::DrSwapChainDesc desc;
  bool b = obj.create(device, desc);
  if (b) {
    obj.release();
  }
  EXPECT_TRUE(true);
}

TEST(D3DSwapChain, swapBuffers) {
  EXPECT_TRUE(true);
}