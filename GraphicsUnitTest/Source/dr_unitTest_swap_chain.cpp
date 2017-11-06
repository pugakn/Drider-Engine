#pragma once
#include <dr_graphics_defines.h>
#include <dr_d3d_swap_chain.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_d3d_texture.h>
#include <gtest\gtest.h>

TEST(SwapChain, destructor) {
  EXPECT_TRUE(true);
}

TEST(SwapChain, create) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DSwapChain swapchain;
	driderSDK::DrSwapChainDesc desc;
	desc.bufferCount = 2;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
	desc.height = 1080;
	desc.refreshRate.numerator = 0;
	desc.refreshRate.denominator = 1;
	desc.width = 1920;
	desc.windowed = true;
  EXPECT_TRUE(!swapchain.create(device, desc));
}

TEST(SwapChain, getBackBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DSwapChain swapchain;
	driderSDK::DrSwapChainDesc desc;
	desc.bufferCount = 2;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
	desc.height = 1080;
	desc.refreshRate.numerator = 0;
	desc.refreshRate.denominator = 1;
	desc.width = 1920;
	desc.windowed = true;
	swapchain.create(device, desc);
	driderSDK::D3DTexture tex;
	driderSDK::DrTextureDesc tdesc;
	tdesc.width = 1920;
	tdesc.pitch = 1920 * 4;
	tdesc.height = 1080;
	tdesc.mipLevels = 1;
	tdesc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	tdesc.CPUAccessFlags = 0;
	tex.createEmpty(device, tdesc);
  EXPECT_TRUE(!swapchain.getBackBuffer(tex));
}

TEST(SwapChain, release) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DSwapChain swapchain;
	driderSDK::DrSwapChainDesc desc;
	desc.bufferCount = 2;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
	desc.height = 1080;
	desc.refreshRate.numerator = 0;
	desc.refreshRate.denominator = 1;
	desc.width = 1920;
	desc.windowed = true;
	swapchain.create(device, desc);
	swapchain.release();
  EXPECT_TRUE(true);
}

TEST(SwapChain, swapBuffers) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DSwapChain swapchain;
	driderSDK::DrSwapChainDesc desc;
	desc.bufferCount = 2;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
	desc.height = 1080;
	desc.refreshRate.numerator = 0;
	desc.refreshRate.denominator = 1;
	desc.width = 1920;
	desc.windowed = true;
	swapchain.create(device, desc);
	swapchain.swapBuffers();
  EXPECT_TRUE(true);
}
