#include <gtest\gtest.h>
#include <dr_md5.h>
#include <dr_octree.h>
#include <dr_aabb.h>
#include <queue>

void chargePoints(std::queue<driderSDK::Face>* objects) {
  driderSDK::Face temp;
  temp.vertex[0] = driderSDK::Vector3D(734, 710, 855);
  temp.vertex[1] = driderSDK::Vector3D(733, 617, 920);
  temp.vertex[2] = driderSDK::Vector3D(766, 620, 826);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(271, 119, 1023);
  temp.vertex[1] = driderSDK::Vector3D(281, 151, 990);
  temp.vertex[2] = driderSDK::Vector3D(254, 215, 1000);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(139, 773, 53);
  temp.vertex[1] = driderSDK::Vector3D(143, 763, 76);
  temp.vertex[2] = driderSDK::Vector3D(169, 683, 22);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(265, 537, 1019);
  temp.vertex[1] = driderSDK::Vector3D(295, 532, 1023);
  temp.vertex[2] = driderSDK::Vector3D(293, 526, 924);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(564, 803, 573);
  temp.vertex[1] = driderSDK::Vector3D(610, 753, 607);
  temp.vertex[2] = driderSDK::Vector3D(622, 755, 508);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(30, 509, 749);
  temp.vertex[1] = driderSDK::Vector3D(59, 503, 774);
  temp.vertex[2] = driderSDK::Vector3D(106, 490, 687);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(200, 221, 1018);
  temp.vertex[1] = driderSDK::Vector3D(277, 227, 1009);
  temp.vertex[2] = driderSDK::Vector3D(213, 254, 938);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(185, 648, 288);
  temp.vertex[1] = driderSDK::Vector3D(204, 658, 290);
  temp.vertex[2] = driderSDK::Vector3D(195, 655, 273);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(973, 778, 361);
  temp.vertex[1] = driderSDK::Vector3D(979, 795, 355);
  temp.vertex[2] = driderSDK::Vector3D(974, 781, 342);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(887, 299, 256);
  temp.vertex[1] = driderSDK::Vector3D(879, 301, 249);
  temp.vertex[2] = driderSDK::Vector3D(892, 289, 240);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(120, 932, 30);
  temp.vertex[1] = driderSDK::Vector3D(132, 935, 30);
  temp.vertex[2] = driderSDK::Vector3D(125, 938, 12);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(174, 1022, 130);
  temp.vertex[1] = driderSDK::Vector3D(189, 1015, 136);
  temp.vertex[2] = driderSDK::Vector3D(181, 1021, 119);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(759, 257, 487);
  temp.vertex[1] = driderSDK::Vector3D(776, 239, 493);
  temp.vertex[2] = driderSDK::Vector3D(764, 240, 478);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(263, 527, 43);
  temp.vertex[1] = driderSDK::Vector3D(289, 537, 47);
  temp.vertex[2] = driderSDK::Vector3D(279, 529, 32);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(442, 13, 201);
  temp.vertex[1] = driderSDK::Vector3D(444, 2, 209);
  temp.vertex[2] = driderSDK::Vector3D(443, 3, 190);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(603, 416, 907);
  temp.vertex[1] = driderSDK::Vector3D(622, 402, 899);
  temp.vertex[2] = driderSDK::Vector3D(610, 414, 889);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(692, 563, 274);
  temp.vertex[1] = driderSDK::Vector3D(717, 557, 269);
  temp.vertex[2] = driderSDK::Vector3D(701, 556, 258);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(248, 710, 601);
  temp.vertex[1] = driderSDK::Vector3D(249, 711, 600);
  temp.vertex[2] = driderSDK::Vector3D(249, 706, 592);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(48, 142, 23);
  temp.vertex[1] = driderSDK::Vector3D(59, 139, 26);
  temp.vertex[2] = driderSDK::Vector3D(56, 142, 17);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(464, 74, 766);
  temp.vertex[1] = driderSDK::Vector3D(465, 84, 769);
  temp.vertex[2] = driderSDK::Vector3D(464, 82, 760);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(504, 929, 997);
  temp.vertex[1] = driderSDK::Vector3D(494, 928, 999);
  temp.vertex[2] = driderSDK::Vector3D(497, 929, 990);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(71, 761, 168);
  temp.vertex[1] = driderSDK::Vector3D(78, 760, 168);
  temp.vertex[2] = driderSDK::Vector3D(75, 762, 159);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(233, 986, 23);
  temp.vertex[1] = driderSDK::Vector3D(236, 978, 25);
  temp.vertex[2] = driderSDK::Vector3D(238, 981, 16);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(649, 345, 1017);
  temp.vertex[1] = driderSDK::Vector3D(649, 363, 1017);
  temp.vertex[2] = driderSDK::Vector3D(653, 354, 1016);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(551, 930, 564);
  temp.vertex[1] = driderSDK::Vector3D(552, 929, 564);
  temp.vertex[2] = driderSDK::Vector3D(551, 929, 564);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(934, 330, 903);
  temp.vertex[1] = driderSDK::Vector3D(941, 341, 901);
  temp.vertex[2] = driderSDK::Vector3D(940, 333, 896);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(526, 717, 882);
  temp.vertex[1] = driderSDK::Vector3D(525, 720, 883);
  temp.vertex[2] = driderSDK::Vector3D(531, 723, 876);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(710, 351, 660);
  temp.vertex[1] = driderSDK::Vector3D(717, 347, 661);
  temp.vertex[2] = driderSDK::Vector3D(714, 349, 658);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(510, 924, 158);
  temp.vertex[1] = driderSDK::Vector3D(509, 932, 158);
  temp.vertex[2] = driderSDK::Vector3D(511, 928, 156);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(1007, 882, 978);
  temp.vertex[1] = driderSDK::Vector3D(1012, 876, 978);
  temp.vertex[2] = driderSDK::Vector3D(1011, 880, 976);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(354, 365, 732);
  temp.vertex[1] = driderSDK::Vector3D(361, 362, 731);
  temp.vertex[2] = driderSDK::Vector3D(358, 365, 729);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(721, 339, 490);
  temp.vertex[1] = driderSDK::Vector3D(719, 342, 486);
  temp.vertex[2] = driderSDK::Vector3D(722, 338, 486);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(102, 956, 139);
  temp.vertex[1] = driderSDK::Vector3D(110, 959, 137);
  temp.vertex[2] = driderSDK::Vector3D(105, 959, 137);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(220, 552, 688);
  temp.vertex[1] = driderSDK::Vector3D(225, 551, 689);
  temp.vertex[2] = driderSDK::Vector3D(224, 554, 686);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(93, 76, 768);
  temp.vertex[1] = driderSDK::Vector3D(97, 76, 768);
  temp.vertex[2] = driderSDK::Vector3D(95, 74, 764);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(56, 558, 313);
  temp.vertex[1] = driderSDK::Vector3D(58, 565, 312);
  temp.vertex[2] = driderSDK::Vector3D(57, 561, 310);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(444, 672, 342);
  temp.vertex[1] = driderSDK::Vector3D(440, 669, 341);
  temp.vertex[2] = driderSDK::Vector3D(444, 668, 339);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(1000, 322, 572);
  temp.vertex[1] = driderSDK::Vector3D(1001, 321, 572);
  temp.vertex[2] = driderSDK::Vector3D(1000, 321, 571);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(789, 855, 521);
  temp.vertex[1] = driderSDK::Vector3D(790, 854, 520);
  temp.vertex[2] = driderSDK::Vector3D(790, 855, 521);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(450, 694, 301);
  temp.vertex[1] = driderSDK::Vector3D(450, 693, 302);
  temp.vertex[2] = driderSDK::Vector3D(449, 693, 301);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(815, 121, 91);
  temp.vertex[1] = driderSDK::Vector3D(815, 120, 92);
  temp.vertex[2] = driderSDK::Vector3D(814, 119, 91);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(273, 635, 950);
  temp.vertex[1] = driderSDK::Vector3D(272, 634, 951);
  temp.vertex[2] = driderSDK::Vector3D(272, 635, 950);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(991, 308, 746);
  temp.vertex[1] = driderSDK::Vector3D(992, 309, 745);
  temp.vertex[2] = driderSDK::Vector3D(992, 308, 746);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(161, 559, 619);
  temp.vertex[1] = driderSDK::Vector3D(161, 557, 620);
  temp.vertex[2] = driderSDK::Vector3D(161, 558, 619);
  objects->push(temp);
  temp.vertex[0] = driderSDK::Vector3D(1000, 492, 862);
  temp.vertex[1] = driderSDK::Vector3D(1001, 493, 862);
  temp.vertex[2] = driderSDK::Vector3D(1000, 493, 863);
  objects->push(temp);
}

void rutaObject(std::string ruta, driderSDK::Octree &octree, std::queue<std::string>* rutas) {
  for (size_t i = 0; i < octree.childs.size(); i++)
  {
    rutaObject(ruta + std::to_string(i), *octree.childs[i], rutas);
    
  }
  if (octree.containedObjects.size() > 0)
  {
    ruta += "size(" + std::to_string(octree.boundingRegion.width) + ")" +
      "(" + std::to_string(octree.boundingRegion.center.x) + ")" +
      "(" + std::to_string(octree.boundingRegion.center.y) + ")" +
      "(" + std::to_string(octree.boundingRegion.center.z) + ")";

    rutas->push(ruta);
  }
}

int main(int argc, char* argv[])
{
  std::queue<driderSDK::Face> objects;
  chargePoints(&objects);
  driderSDK::AABB space(1024,1024,1024, driderSDK::Vector3D(512,512,512));
  driderSDK::Octree octree(space, objects, 10);
  std::queue<std::string> rutas;
  octree.BuildTree();
  rutaObject("R", octree, &rutas);
  ::testing::GTEST_FLAG(output) = "xml";
  ::testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();

  return 0;
}