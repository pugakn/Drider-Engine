#include <dr_model_resource.h>
#include <gtest\gtest.h>

TEST(ModelResource, constructor) {
  EXPECT_TRUE(false);
}

TEST(ModelResource, destructor) {
  EXPECT_TRUE(false);
}

TEST(ModelResource, create) {
  std::shared_ptr<driderSDK::Resource> ptr;
  ptr = driderSDK::ModelResource::Create();
  EXPECT_TRUE(ptr != nullptr);
}