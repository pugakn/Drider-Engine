#include <dr_face.h>
#include <gtest\gtest.h>

TEST(Face, faseConstructor) {
  driderSDK::Face face;
  face.ID = 1;
  EXPECT_TRUE(face.ID == 1);
}

TEST(Face, faceDestructor) {
  EXPECT_TRUE(true);
}