#include "..\Include\dr_skeleton.h"

void driderSDK::Skeleton::setBones(BonesRefsList&& bones) {
  m_bonesRefByName = bones;
}
