#pragma once
#include <dr_d3d_compute_shader.h>
#include <gtest\gtest.h>
#include <dr_d3d_device.h>
#include <dr_d3d_device_context.h>
#include <dxgi.h>

//TEST(D3DComputeShader, set) {
//  driderSDK::D3DComputeShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
//  obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  obj.set(context);
//  EXPECT_TRUE(false);
//}
//
//TEST(D3DComputeShader, release) {
//  driderSDK::D3DComputeShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
//  obj.createFromMemory(device, source, GTEST_ARRAY_SIZE_(source));
//  EXPECT_TRUE(!obj.APIShader);
//}
//
//TEST(D3DComputeShader, createFromMemory) {
//  driderSDK::D3DComputeShader obj;
//  driderSDK::D3DDevice device;
//  driderSDK::D3DDeviceContext context;
//  device.createDeviceAndDeviceContext(context);
//  char *source = "cbuffer ConstantBuffer{float4x4 WVP;}struct VS_INPUT {float4 position : POSITION;};struct VS_OUTPUT {float4 hposition : SV_POSITION;};VS_OUTPUT VS(VS_INPUT input) {VS_OUTPUT OUT;OUT.hposition = mul(WVP, input.position);return OUT;}";
//  EXPECT_TRUE(!obj.createFromMemory(device, source,GTEST_ARRAY_SIZE_(source)));
//}