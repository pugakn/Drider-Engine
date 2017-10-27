#pragma once
#include <dr_device_context.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_d3d_texture.h>
#include <dr_d3d_render_target.h>
#include <dr_d3d_depth_stencil.h>
#include <dr_d3d_vertex_buffer.h>
#include <dr_d3d_index_buffer.h>
#include <dr_d3d_vertex_shader.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DDeviceContext, release) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	context.release();
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, generateMipMaps) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DTexture tex;
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	tex.createEmpty(device, desc);
	context.generateMipMaps(tex);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, updateTextureFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DTexture tex;
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	tex.createEmpty(device, desc);
	char data[10];
	context.updateTextureFromMemory(tex, data, sizeof(data));
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, updateBufferFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DVertexBuffer vb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	char data[10];
	vb.create(device, desc, data);
	context.updateBufferFromMemory(vb, data, sizeof(data));
	EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setRenderTargets) {
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
	driderSDK::D3DRenderTarget rt;
	rt.create(device, desc, 0);
	driderSDK::D3DDepthStencil ds;
	ds.create(device, desc);
	context.setRenderTarget(rt, ds);
	EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setTexture) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DTexture tex;
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	tex.createEmpty(device, desc);
	context.setTexture(tex, 1);
	EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setShader) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	char data[10];
	driderSDK::D3DVertexShader shader;
	shader.createFromMemory(device, data, sizeof(data));
	context.setShader(shader);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setIndexBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DIndexBuffer ib;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	char data[10];
	ib.create(device, desc, data);
	context.setIndexBuffer(ib, 0);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setVertexBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DVertexBuffer vb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	char data[10];
	vb.create(device, desc, data);
	context.setVertexBuffer(vb, 2, 0);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setConstantBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	//need constant buffer

  //EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, clearDepthStencilView) {
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
	driderSDK::D3DDepthStencil ds;
	ds.create(device, desc);
	context.clearDepthStencilView(ds, 
																driderSDK::DR_DEPTH_STENCIL_CLEAR_TYPE::E::kClearDepthStencil, 
																1.f, 
																1);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, clearRenderTargetView) {
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
	driderSDK::D3DRenderTarget rt;
	rt.create(device, desc, 0);
	float color[4] = { 0.f,0.f,0.f,0.f };
	context.clearRenderTargetView(rt, color);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, setPrimitiveTopology) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	context.setPrimitiveTopology(driderSDK::DR_PRIMITIVE_TOPOLOGY::E::kTriangles);
  EXPECT_TRUE(true);
}

TEST(D3DDeviceContext, draw) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	context.draw(3, 3, 0);
  EXPECT_TRUE(true);
}