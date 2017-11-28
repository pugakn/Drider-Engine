#pragma once
#include <dr_d3d_fragment_shader.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>

//TEST(D3DFragmentShader, set) {
//  driderSDK::D3DFragmentShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_OUTPUT {float4 hposition : SV_POSITION;};float4 FS(VS_OUTPUT input) : SV_TARGET{return float4(1,0,1,1);}";
//  bool b = obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  obj.set(context);
//  EXPECT_TRUE(!b);
//}
//
//TEST(D3DFragmentShader, release) {
//  driderSDK::D3DFragmentShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_OUTPUT {float4 hposition : SV_POSITION;};float4 FS(VS_OUTPUT input) : SV_TARGET{return float4(1,0,1,1);}";
//  bool b = obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  if (b) {
//    obj.release();
//  }
//  EXPECT_TRUE(!obj.APIShader.Get());
//}
//
//TEST(D3DFragmentShader, createFromMemory) {
//  driderSDK::D3DFragmentShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_OUTPUT {float4 hposition : SV_POSITION;};float4 FS(VS_OUTPUT input) : SV_TARGET{return float4(1,0,1,1);}";
//  bool b = obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  EXPECT_TRUE(!b);
//}
