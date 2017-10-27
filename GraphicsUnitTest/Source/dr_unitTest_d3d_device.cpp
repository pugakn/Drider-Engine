#pragma once
#include <dr_device.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_vertex_buffer.h>
#include <dr_d3d_index_buffer.h>
#include <dr_d3d_device_context.h>
//#include <dr_d3d_constant_buffer.h>
#include <dr_d3d_texture.h>
#include <dr_d3d_vertex_shader.h>
#include <dr_d3d_render_target.h>
#include <dr_d3d_depth_stencil.h>
#include <dr_d3d_sample_state.h>
#include <dr_d3d_rasterizer_state.h>
#include <dr_d3d_depth_stencil_state.h>
#include <dr_d3d_input_layout.h>
#include <dr_d3d_swap_chain.h>
#include <d3d11.h>
#include <dxgi.h>


TEST(Device, createVertexBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DVertexBuffer vb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	char initData[10];
	vb.create(device, desc, initData);
	EXPECT_TRUE(!device.createVertexBuffer(desc, initData, vb));
}

TEST(Device, createIndexBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	driderSDK::D3DIndexBuffer ib;
	char initData[10];
	ib.create(device, desc, initData);
	EXPECT_TRUE(!device.createIndexBuffer(desc, initData, ib));
}

TEST(Device, createConstantBuffer) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	//driderSDK::D3DConstantBuffer;  <-????

	char initData[10];

  EXPECT_TRUE(false);
}

TEST(Device, createShaderFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	char initData[10], shaderdata[10];
	driderSDK::D3DVertexShader shader;
	shader.createFromMemory(device, initData, sizeof(initData));
	EXPECT_TRUE(!device.createShaderFromMemory(shaderdata, sizeof(shaderdata), shader));
}

TEST(Device, createTextureFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	driderSDK::D3DTexture tex;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	tex.createEmpty(device, desc);
	char initData[10];
	EXPECT_TRUE(!device.createTextureFromMemory(initData, desc, tex));
}

TEST(Device, createEmptyTexture) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	driderSDK::D3DTexture tex;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrTextureDesc desc;
	desc.width = 1920;
	desc.pitch = 1920 * 4;
	desc.height = 1080;
	desc.mipLevels = 1;
	desc.Format = driderSDK::DR_FORMAT::kDrFormat_R8G8B8A8_UNORM;
	desc.CPUAccessFlags = 0;
	EXPECT_TRUE(!device.createEmptyTexture(desc, tex));
}

TEST(Device, createRenderTarget) {
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
	EXPECT_TRUE(!device.createRenderTarget(desc, rt, 1));
}

TEST(Device, createDepthStencil) {
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
	EXPECT_TRUE(!device.createDepthStencil(desc, ds));
}

TEST(Device, createSamplerState) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::DrSampleDesc desc;
	
	desc.Filter = driderSDK::DR_TEXTURE_FILTER::E::kCOMPARISON_MIN_MAG_MIP_LINEAR;
	desc.comparisonFunc = driderSDK::DR_COMPARISON_FUNC::E::kNEVER;
	desc.maxLOD = 0;
	desc.minLOD = 0;
	desc.addressU = driderSDK::DR_TEXTURE_ADDRESS::E::kClamp;
	desc.addressV = driderSDK::DR_TEXTURE_ADDRESS::E::kClamp;
	desc.addressW = driderSDK::DR_TEXTURE_ADDRESS::E::kClamp;

	driderSDK::D3D11SamplerState state;
	state.create(device, desc);
	EXPECT_TRUE(!device.createSamplerState(desc, state));
}

TEST(Device, createRasteizerState) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DRasterizerState state;
	driderSDK::DrRasterizerDesc desc;
	desc.fillMode = driderSDK::DR_FILL_MODE::kSolid;
	desc.cullMode = driderSDK::DR_CULL_MODE::kBack;
	desc.frontCounterClockwise = true;
	desc.depthBias = 0;
	desc.depthBiasClamp = 0.0f;
	desc.slopeScaledDepthBias = 0.0f;
	desc.depthClipEnable = TRUE;
	desc.scissorEnable = FALSE;
	desc.multisampleEnable = FALSE;
	desc.antialiasedLineEnable = FALSE;
	state.create(device, desc);
	EXPECT_TRUE(!device.createRasteizerState(desc, state));
}

TEST(Device, createDepthStencilState) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DDepthStencilState state;
	driderSDK::DrDepthStencilDesc desc;
	desc.depthEnable = true;
	desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;
	state.create(device, desc);
	device.createDepthStencilState(desc, state);
	//EXPECT_TRUE(!);
}

TEST(Device, createInputLayout) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DInputLayout layout;
	driderSDK::DrInputElementDesc desc;
	desc.format = driderSDK::DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
	desc.offset = 0;
	desc.inputSlot = 0;
	desc.semanticIndex = 0;
	desc.semanticName = "POSITION";
	layout.create(device, &desc, 1);
	EXPECT_TRUE(!device.createInputLayout(&desc, 1, layout));
}

TEST(Device, createSwapChain) {
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
	//desc.windowHandler;
	swapchain.create(device, desc);
	EXPECT_TRUE(!device.createSwapChain(desc, swapchain));
}

