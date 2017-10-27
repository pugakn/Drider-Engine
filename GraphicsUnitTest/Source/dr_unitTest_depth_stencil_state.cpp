#pragma once
#include <dr_graphics_defines.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dr_d3d_depth_stencil_state.h>
#include <gtest\gtest.h>

TEST(DepthStencilState, create) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DDepthStencilState state;
	driderSDK::DrDepthStencilDesc desc;
	desc.depthEnable = true;
	desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;
  EXPECT_TRUE(!state.create(device, desc));
}

TEST(DepthStencilState, set) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DDepthStencilState state;
	driderSDK::DrDepthStencilDesc desc;
	desc.depthEnable = true;
	desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;
	state.create(device, desc);
	state.set(context, 1);
  EXPECT_TRUE(true);
}

TEST(DepthStencilState, release) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DDepthStencilState state;
	driderSDK::DrDepthStencilDesc desc;
	desc.depthEnable = true;
	desc.depthFunc = driderSDK::DR_COMPARISON_FUNC::kGREATER;
	state.create(device, desc);
	state.release();
  EXPECT_TRUE(true);
}