#pragma once
#include <dr_d3d_domain_shader.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

driderSDK::Device *device = new driderSDK::D3DDevice();
driderSDK::DeviceContext *deviceContext = new driderSDK::D3DDeviceContext();
driderSDK::D3DDomainShader *domainShader = new driderSDK::D3DDomainShader();

TEST(D3DDomainShader, set) {
  device->createDeviceAndDeviceContext(*deviceContext);
  domainShader->set(*deviceContext);
  EXPECT_TRUE(deviceContext != nullptr);
}

TEST(D3DDomainShader, release) {
  domainShader->release();
  EXPECT_TRUE(domainShader == nullptr);
}

TEST(D3DDomainShader, createFromMemory) {
  char *shader = "";
  domainShader->createFromMemory(*device, shader, 5);
  EXPECT_TRUE(domainShader->APIShader != nullptr);
}
