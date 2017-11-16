#include <dr_memory.h>
#include <gtest\gtest.h>
#include <dr_engine_prerequisites.h>
#include <dr_codec_model.h>
#include <dr_model_info.h>

using driderSDK::Codec;
using driderSDK::CodecModel;
using CodecPtr = std::unique_ptr<Codec>;

TEST(CodecModel, decode) {
  CodecPtr modelCodec = driderSDK::dr_make_unique<CodecModel>();

  Codec::UniqueVoidPtr pData = modelCodec->decode(_T("Cube.fbx"));

  driderSDK::ModelInfo* pInfo = static_cast<driderSDK::ModelInfo*>(pData.get());

  EXPECT_TRUE(pInfo->meshes.size() == 1);
  EXPECT_TRUE(pInfo->meshes[0].vertices.size() == 24);
}

TEST(CodecModel, isCompatible) {

  CodecPtr modelCodec = driderSDK::dr_make_unique<CodecModel>();

  EXPECT_TRUE(modelCodec->isCompatible(_T("frutsi.obj")));
  EXPECT_TRUE(modelCodec->isCompatible(_T("frutsi.fbx")));
  EXPECT_FALSE(modelCodec->isCompatible(_T("frutsi.pepe")));

}
