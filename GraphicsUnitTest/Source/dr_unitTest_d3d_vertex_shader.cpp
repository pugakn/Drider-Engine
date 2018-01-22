#pragma once
#include <dr_d3d_vertex_shader.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <d3d11.h>
#include <dxgi.h>
#include <gtest\gtest.h>

//TEST(D3DVertexShader, set) {
//  driderSDK::D3DVertexShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
//  bool b = obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  obj.set(context);
//  EXPECT_TRUE(!b);
//}
//
//TEST(D3DVertexShader, release) {
//  driderSDK::D3DVertexShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
//  bool b = obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  if (b) {
//    obj.release();
//  }
//  EXPECT_TRUE(!obj.APIShader.Get());
//}
//
//TEST(D3DVertexShader, createFromMemory) {
//  driderSDK::D3DVertexShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
//  bool b = obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  EXPECT_TRUE(!b);
//}