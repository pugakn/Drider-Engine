#pragma once
#include <dr_graphics_defines.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_d3d_texture.h>
#include <gtest\gtest.h>

TEST(Texture, destructor) {
  EXPECT_TRUE(true);
}

TEST(Texture, createFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	char data[10];
	EXPECT_TRUE(!tex.createFromMemory(device, desc, data));
}

TEST(Texture, createEmpty) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
  EXPECT_TRUE(!tex.createEmpty(device, desc));
}

TEST(Texture, map) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	tex.createEmpty(device, desc);
	char data[10];
  EXPECT_TRUE(!tex.map(context, data));
}

TEST(Texture, set) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	tex.createEmpty(device, desc);
	tex.set(context, 1);
  EXPECT_TRUE(true);
}

TEST(Texture, release) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	tex.createEmpty(device, desc);
	tex.release();
  EXPECT_TRUE(true);
}

TEST(Texture, unmap) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	tex.createEmpty(device, desc);
	tex.unmap(context);
  EXPECT_TRUE(true);
}

TEST(Texture, udpateFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	tex.createEmpty(device, desc);
	char data[10];
	tex.udpateFromMemory(context, data, sizeof(data));
	EXPECT_TRUE(true);
}

TEST(Texture, generateMipMaps) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	driderSDK::D3DTexture tex;
	tex.createEmpty(device, desc);
	tex.generateMipMaps(context);
  EXPECT_TRUE(true);
}