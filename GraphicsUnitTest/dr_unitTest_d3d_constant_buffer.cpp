#pragma once
#include <dr_d3d_constant_buffer.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

TEST(D3DConstantBuffer, constructor) {
	EXPECT_TRUE(true);
}

TEST(D3DConstantBuffer, create) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DConstantBuffer cb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;

	char initData[10];
	cb.create(device, desc, initData);
	EXPECT_TRUE(true);
}

TEST(D3DConstantBuffer, set) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DConstantBuffer cb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;

	char initData[10];
	cb.create(device, desc, initData);
	cb.set(context, driderSDK::DR_SHADER_TYPE_FLAG::E::kCompute);
	EXPECT_TRUE(true);
}

TEST(D3DConstantBuffer, updateFromMemory) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DConstantBuffer cb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;

	char initData[10];
	cb.create(device, desc, initData);
	cb.updateFromMemory(context, initData, sizeof(initData));
	EXPECT_TRUE(true);
}

TEST(D3DConstantBuffer, release) {
	driderSDK::D3DDevice device;
	driderSDK::D3DDeviceContext context;
	device.createDeviceAndDeviceContext(context);
	driderSDK::D3DConstantBuffer cb;
	driderSDK::DrBufferDesc desc;
	desc.sizeInBytes = 10;
	desc.stride = 0;
	desc.usage = driderSDK::DR_BUFFER_USAGE::kDefault;
	char initData[10];
	cb.create(device, desc, initData);
	cb.release();
	EXPECT_TRUE(true);
}
