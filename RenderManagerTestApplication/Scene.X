xof 0303txt 0032
template KeyValuePair {
 <26e6b1c3-3d4d-4a1d-a437-b33668ffa1c2>
 STRING key;
 STRING value;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template ObjectMatrixComment {
 <95a48e28-7ef4-4419-a16a-ba9dbdf0d2bc>
 Matrix4x4 objectMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template VertexElement {
 <f752461c-1e23-48f6-b9f8-8350850f336f>
 DWORD Type;
 DWORD Method;
 DWORD Usage;
 DWORD UsageIndex;
}

template DeclData {
 <bf22e553-292c-4781-9fea-62bd554bdd93>
 DWORD nElements;
 array VertexElement Elements[nElements];
 DWORD nDWords;
 array DWORD data[nDWords];
}

template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template EffectInstance {
 <e331f7e4-0559-4cc2-8e99-1cec1657928f>
 STRING EffectFilename;
 [...]
}

template EffectParamDWord {
 <e13963bc-ae51-4c5d-b00f-cfa3a9d97ce5>
 STRING ParamName;
 DWORD Value;
}

template EffectParamFloats {
 <3014b9a0-62f5-478c-9b86-e4ac9f4e418b>
 STRING ParamName;
 DWORD nFloats;
 array FLOAT Floats[nFloats];
}

template EffectParamString {
 <1dbc4c88-94c1-46ee-9076-2c28818c9481>
 STRING ParamName;
 STRING Value;
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}


KeyValuePair {
 "Date";
 "2017-02-27 01:26:39";
}

KeyValuePair {
 "File";
 "C:\\Users\\Camus\\Documents\\3dsMax\\scenes\\sphere.max";
}

KeyValuePair {
 "User";
 "Camus";
}

KeyValuePair {
 "CoreTime";
 "0";
}

Frame Plane002 {
 

 FrameTransformMatrix relative {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 ObjectMatrixComment object {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh mesh_Plane002 {
  25;
  -400.000000;0.000000;400.000000;,
  -200.000000;0.000000;400.000000;,
  -200.000000;0.000000;200.000000;,
  -400.000000;0.000000;200.000000;,
  0.000000;0.000000;400.000000;,
  0.000000;0.000000;200.000000;,
  200.000000;0.000000;400.000000;,
  200.000000;0.000000;200.000000;,
  400.000000;0.000000;400.000000;,
  400.000000;0.000000;200.000000;,
  -200.000000;0.000000;0.000000;,
  -400.000000;0.000000;0.000000;,
  0.000000;0.000000;0.000000;,
  200.000000;0.000000;0.000000;,
  400.000000;0.000000;0.000000;,
  -200.000000;0.000000;-200.000000;,
  -400.000000;0.000000;-200.000000;,
  0.000000;0.000000;-200.000000;,
  200.000000;0.000000;-200.000000;,
  400.000000;0.000000;-200.000000;,
  -200.000000;0.000000;-400.000000;,
  -400.000000;0.000000;-400.000000;,
  0.000000;0.000000;-400.000000;,
  200.000000;0.000000;-400.000000;,
  400.000000;0.000000;-400.000000;;
  32;
  3;0,1,2;,
  3;2,3,0;,
  3;1,4,5;,
  3;5,2,1;,
  3;4,6,7;,
  3;7,5,4;,
  3;6,8,9;,
  3;9,7,6;,
  3;3,2,10;,
  3;10,11,3;,
  3;2,5,12;,
  3;12,10,2;,
  3;5,7,13;,
  3;13,12,5;,
  3;7,9,14;,
  3;14,13,7;,
  3;11,10,15;,
  3;15,16,11;,
  3;10,12,17;,
  3;17,15,10;,
  3;12,13,18;,
  3;18,17,12;,
  3;13,14,19;,
  3;19,18,13;,
  3;16,15,20;,
  3;20,21,16;,
  3;15,17,22;,
  3;22,20,15;,
  3;17,18,23;,
  3;23,22,17;,
  3;18,19,24;,
  3;24,23,18;;

  MeshNormals normals {
   25;
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;1.000000;0.000000;;
   32;
   3;0,1,2;,
   3;2,3,0;,
   3;1,4,5;,
   3;5,2,1;,
   3;4,6,7;,
   3;7,5,4;,
   3;6,8,9;,
   3;9,7,6;,
   3;3,2,10;,
   3;10,11,3;,
   3;2,5,12;,
   3;12,10,2;,
   3;5,7,13;,
   3;13,12,5;,
   3;7,9,14;,
   3;14,13,7;,
   3;11,10,15;,
   3;15,16,11;,
   3;10,12,17;,
   3;17,15,10;,
   3;12,13,18;,
   3;18,17,12;,
   3;13,14,19;,
   3;19,18,13;,
   3;16,15,20;,
   3;20,21,16;,
   3;15,17,22;,
   3;22,20,15;,
   3;17,18,23;,
   3;23,22,17;,
   3;18,19,24;,
   3;24,23,18;;
  }

  MeshTextureCoords tc0 {
   25;
   0.024974;0.975014;,
   12.512485;0.975017;,
   12.512487;-11.512493;,
   0.024977;-11.512496;,
   24.999994;0.975020;,
   24.999996;-11.512490;,
   37.487503;0.975023;,
   37.487507;-11.512487;,
   49.975014;0.975026;,
   49.975018;-11.512485;,
   12.512490;-24.000004;,
   0.024980;-24.000006;,
   25.000000;-24.000000;,
   37.487511;-23.999996;,
   49.975021;-23.999994;,
   12.512493;-36.487514;,
   0.024983;-36.487514;,
   25.000004;-36.487511;,
   37.487514;-36.487507;,
   49.975021;-36.487503;,
   12.512496;-48.975021;,
   0.024986;-48.975025;,
   25.000006;-48.975021;,
   37.487514;-48.975018;,
   49.975025;-48.975014;;
  }

  DeclData {
   2;
   2;0;7;0;,
   2;0;6;0;;
   150;
   3027968389,
   0,
   3212836864,
   1065353216,
   0,
   3027968389,
   3028080348,
   0,
   3212836864,
   1065353216,
   0,
   3028080348,
   3028463818,
   0,
   3212836864,
   1065353216,
   0,
   3028463818,
   3025841149,
   0,
   3212836864,
   1065353216,
   0,
   3025841149,
   3028080348,
   0,
   3212836864,
   1065353216,
   0,
   3028080348,
   3028463819,
   0,
   3212836864,
   1065353216,
   0,
   3028463819,
   3028080349,
   0,
   3212836864,
   1065353216,
   0,
   3028080349,
   3026848791,
   0,
   3212836864,
   1065353216,
   0,
   3026848791,
   3028295879,
   0,
   3212836864,
   1065353216,
   0,
   3028295879,
   3027856429,
   0,
   3212836864,
   1065353216,
   0,
   3027856429,
   3028407838,
   0,
   3212836864,
   1065353216,
   0,
   3028407838,
   3017452541,
   0,
   3212836864,
   1065353216,
   0,
   3017452541,
   3030199197,
   0,
   3212836864,
   1065353216,
   0,
   3030199197,
   3028407838,
   0,
   3212836864,
   1065353216,
   0,
   3028407838,
   3024049789,
   0,
   3212836864,
   1065353216,
   0,
   3024049789,
   3020467069,
   0,
   3212836864,
   1065353216,
   0,
   3020467069,
   3017452540,
   0,
   3212836864,
   1065353216,
   0,
   3017452540,
   3030647037,
   0,
   3212836864,
   1065353216,
   0,
   3030647037,
   3030647037,
   0,
   3212836864,
   1065353216,
   0,
   3030647037,
   3028855677,
   0,
   3212836864,
   1065353216,
   0,
   3028855677,
   3017452540,
   0,
   3212836864,
   1065353216,
   0,
   3017452540,
   3030647037,
   0,
   3212836864,
   1065353216,
   0,
   3030647037,
   3025841149,
   0,
   3212836864,
   1065353216,
   0,
   3025841149,
   3030647037,
   0,
   3212836864,
   1065353216,
   0,
   3030647037,
   3030647037,
   0,
   3212836864,
   1065353216,
   0,
   3030647037;
  }

  MeshMaterialList mtls {
   1;
   32;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;

   Material Dflt_Material {
    0.109804;0.584314;0.694118;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      1;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      0;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "grasss.tga";
     }
	 
	 EffectParamString {
      "specularMap";
      "Black.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      1;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "Grass.tga";
    }
   }
  }
 }
}

Frame Box001 {
 

 FrameTransformMatrix relative {
  0.414499,0.000000,0.000000,0.000000,0.000000,0.414499,0.000000,0.000000,0.000000,0.000000,0.414499,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 ObjectMatrixComment object {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1499.99988,0.000000,1.000000;;
 }

 Mesh mesh_Box001 {
  24;
  -1499.99988;-1499.99988;1499.99988;,
  1499.99988;-1499.99988;1499.99988;,
  1499.99988;-1499.99988;-1499.99988;,
  -1499.99988;-1499.99988;-1499.99988;,
  -1499.99988;1499.99988;1499.99988;,
  -1499.99988;1499.99988;-1499.99988;,
  1499.99988;1499.99988;-1499.99988;,
  1499.99988;1499.99988;1499.99988;,
  -1499.99988;-1499.99988;1499.99988;,
  -1499.99988;1499.99988;1499.99988;,
  1499.99988;1499.99988;1499.99988;,
  1499.99988;-1499.99988;1499.99988;,
  1499.99988;-1499.99988;1499.99988;,
  1499.99988;1499.99988;1499.99988;,
  1499.99988;1499.99988;-1499.99988;,
  1499.99988;-1499.99988;-1499.99988;,
  1499.99988;-1499.99988;-1499.99988;,
  1499.99988;1499.99988;-1499.99988;,
  -1499.99988;1499.99988;-1499.99988;,
  -1499.99988;-1499.99988;-1499.99988;,
  -1499.99988;-1499.99988;-1499.99988;,
  -1499.99988;1499.99988;-1499.99988;,
  -1499.99988;1499.99988;1499.99988;,
  -1499.99988;-1499.99988;1499.99988;;
  12;
  3;0,1,2;,
  3;2,3,0;,
  3;4,5,6;,
  3;6,7,4;,
  3;8,9,10;,
  3;10,11,8;,
  3;12,13,14;,
  3;14,15,12;,
  3;16,17,18;,
  3;18,19,16;,
  3;20,21,22;,
  3;22,23,20;;

  MeshNormals normals {
   24;
   0.000000;2.412550;0.000000;,
   0.000000;2.412550;0.000000;,
   0.000000;2.412550;0.000000;,
   0.000000;2.412550;0.000000;,
   0.000000;-2.412550;0.000000;,
   0.000000;-2.412550;0.000000;,
   0.000000;-2.412550;0.000000;,
   0.000000;-2.412550;0.000000;,
   0.000000;0.000000;-2.412550;,
   0.000000;0.000000;-2.412550;,
   0.000000;0.000000;-2.412550;,
   0.000000;0.000000;-2.412550;,
   -2.412550;0.000000;0.000000;,
   -2.412550;0.000000;0.000000;,
   -2.412550;0.000000;0.000000;,
   -2.412550;0.000000;0.000000;,
   0.000000;0.000000;2.412550;,
   0.000000;0.000000;2.412550;,
   0.000000;0.000000;2.412550;,
   0.000000;0.000000;2.412550;,
   2.412550;0.000000;0.000000;,
   2.412550;0.000000;0.000000;,
   2.412550;0.000000;0.000000;,
   2.412550;0.000000;0.000000;;
   12;
   3;0,1,2;,
   3;2,3,0;,
   3;4,5,6;,
   3;6,7,4;,
   3;8,9,10;,
   3;10,11,8;,
   3;12,13,14;,
   3;14,15,12;,
   3;16,17,18;,
   3;18,19,16;,
   3;20,21,22;,
   3;22,23,20;;
  }

  MeshTextureCoords tc0 {
   24;
   1.000000;1.000000;,
   0.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   0.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;;
  }

  DeclData {
   2;
   2;0;7;0;,
   2;0;6;0;;
   144;
   0,
   0,
   3222955831,
   3222955831,
   0,
   0,
   0,
   0,
   3222955831,
   3222955831,
   0,
   0,
   0,
   0,
   3222955831,
   3222955831,
   0,
   0,
   0,
   0,
   3222955831,
   3222955831,
   0,
   0,
   0,
   0,
   3222955831,
   1075472183,
   0,
   0,
   0,
   0,
   3222955831,
   1075472183,
   0,
   0,
   0,
   0,
   3222955831,
   1075472183,
   0,
   0,
   0,
   0,
   3222955831,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   0,
   0,
   3222955831,
   0,
   1075472183,
   0,
   0,
   0,
   3222955831,
   0,
   1075472183,
   0,
   0,
   0,
   3222955831,
   0,
   1075472183,
   0,
   0,
   0,
   3222955831,
   0,
   1075472183,
   0,
   3222955831,
   0,
   0,
   0,
   1075472183,
   0,
   3222955831,
   0,
   0,
   0,
   1075472183,
   0,
   3222955831,
   0,
   0,
   0,
   1075472183,
   0,
   3222955831,
   0,
   0,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183,
   0,
   1075472183,
   0,
   0,
   0,
   1075472183;
  }

  MeshMaterialList mtls {
   6;
   12;
   0,
   0,
   1,
   1,
   2,
   2,
   3,
   3,
   4,
   4,
   5,
   5;

   Material Dflt_Material {
    0.560784;0.878431;0.341176;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      0;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      1;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "4.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      1;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "4.tga";
    }
   }

   Material Dflt_Material {
    0.560784;0.878431;0.341176;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      0;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      1;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "3.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      1;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "3.tga";
    }
   }

   Material Dflt_Material {
    0.560784;0.878431;0.341176;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      0;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      1;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "6.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      1;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "6.tga";
    }
   }

   Material Dflt_Material {
    0.560784;0.878431;0.341176;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      0;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      1;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "2.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      0;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "2.tga";
    }
   }

   Material Dflt_Material {
    0.560784;0.878431;0.341176;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      0;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      1;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "5.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      1;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "5.tga";
    }
   }

   Material Dflt_Material {
    0.560784;0.878431;0.341176;1.000000;;
    16.000000;
    1.000000;1.000000;1.000000;;
    0.000000;0.000000;0.000000;;

    EffectInstance Effect {
     "Xoliulshader_1.6 beta.fx";

     EffectParamDWord {
      "Tiled";
      0;
     }

     EffectParamDWord {
      "AlphaMask";
      0;
     }

     EffectParamDWord {
      "AlphaGlass";
      0;
     }

     EffectParamDWord {
      "MirrorUV";
      0;
     }

     EffectParamDWord {
      "ShadowCaster";
      0;
     }

     EffectParamDWord {
      "ShadowRenderer";
      0;
     }

     EffectParamDWord {
      "IndependentLights";
      0;
     }

     EffectParamDWord {
      "NoLighting";
      1;
     }

     EffectParamDWord {
      "HDR";
      0;
     }

     EffectParamDWord {
      "UsePhysX";
      0;
     }

     EffectParamDWord {
      "ConvexMesh";
      0;
     }

     EffectParamDWord {
      "TriangleMesh";
      0;
     }

     EffectParamDWord {
      "Terrain";
      0;
     }

     EffectParamDWord {
      "Compound";
      0;
     }

     EffectParamDWord {
      "numberOfActiveLights";
      1;
     }

     EffectParamDWord {
      "light1_Position";
      0;
     }

     EffectParamDWord {
      "__PlaceHolder_255_shadowTerm1";
      0;
     }

     EffectParamDWord {
      "bUseShadow1";
      0;
     }

     EffectParamFloats {
      "Shadow1Soft";
      1;
      0.000000;
     }

     EffectParamFloats {
      "Shadow1Strength";
      1;
      1.000000;
     }

     EffectParamFloats {
      "DistanceBlur";
      1;
      3.000000;
     }

     EffectParamDWord {
      "light2_Position";
      0;
     }

     EffectParamDWord {
      "light3_Position";
      0;
     }

     EffectParamFloats {
      "HueAdjust";
      1;
      0.300000;
     }

     EffectParamDWord {
      "bUseHalfLambert";
      0;
     }

     EffectParamFloats {
      "HalfLambertPower";
      1;
      2.000000;
     }

     EffectParamFloats {
      "ambientcolor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseAmbientMap";
      0;
     }

     EffectParamDWord {
      "ambientMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bUseIBL";
      0;
     }

     EffectParamDWord {
      "bUseIBLCubeMap";
      0;
     }

     EffectParamFloats {
      "IBLBlur";
      1;
      7.600000;
     }

     EffectParamFloats {
      "IBLmultiplier";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseLightMap";
      0;
     }

     EffectParamDWord {
      "LightMapmapChannel";
      1;
     }

     EffectParamFloats {
      "LightMapStrength";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseDiffuseMap";
      1;
     }

     EffectParamFloats {
      "diffuseColor";
      4;
      0.500000,
      0.500000,
      0.500000,
      1.000000;
     }

     EffectParamString {
      "diffuseMap";
      "1.tga";
     }

     EffectParamDWord {
      "diffuseMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bColorDiffuse";
      0;
     }

     EffectParamDWord {
      "bUseAlpha";
      0;
     }

     EffectParamFloats {
      "GlobalOpacity";
      1;
      1.000000;
     }

     EffectParamDWord {
      "bUseSpecMap";
      0;
     }

     EffectParamFloats {
      "specularColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      1.000000;
     }

     EffectParamFloats {
      "speclevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseGlossMap";
      0;
     }

     EffectParamFloats {
      "glossiness";
      1;
      25.000000;
     }

     EffectParamFloats {
      "glossoffset";
      1;
      10.000000;
     }

     EffectParamDWord {
      "bUseNormalMap";
      0;
     }

     EffectParamDWord {
      "bUseObjectNormals";
      0;
     }

     EffectParamDWord {
      "normalMapmapChannel";
      1;
     }

     EffectParamDWord {
      "bFlipGreenChannel";
      1;
     }

     EffectParamDWord {
      "bUseReflMap";
      0;
     }

     EffectParamFloats {
      "ReflectThreshold";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseCubeMap";
      0;
     }

     EffectParamFloats {
      "CubeIntensityLevel";
      1;
      0.500000;
     }

     EffectParamDWord {
      "bUseUnimap";
      0;
     }

     EffectParamDWord {
      "bUseSIMap";
      0;
     }

     EffectParamFloats {
      "siMapMult";
      1;
      1.000000;
     }

     EffectParamFloats {
      "siLevel";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseEdge";
      0;
     }

     EffectParamFloats {
      "EdgeStep";
      1;
      0.800000;
     }

     EffectParamFloats {
      "EdgeColor";
      4;
      0.000000,
      0.000000,
      0.000000,
      0.000000;
     }

     EffectParamDWord {
      "bUseFresnel";
      0;
     }

     EffectParamDWord {
      "bUseFresnelMap";
      0;
     }

     EffectParamDWord {
      "bAlphaMasksFresnel";
      0;
     }

     EffectParamFloats {
      "FresnelPower";
      1;
      3.000000;
     }

     EffectParamFloats {
      "FresnelBias";
      1;
      0.000000;
     }

     EffectParamFloats {
      "FresnelMult";
      1;
      2.000000;
     }

     EffectParamFloats {
      "FresnelColor";
      4;
      1.000000,
      1.000000,
      1.000000,
      0.000000;
     }

     EffectParamFloats {
      "FresnelMaskHardness";
      1;
      4.000000;
     }

     EffectParamDWord {
      "bUseWorldMask";
      1;
     }

     EffectParamFloats {
      "CubeMapBlur";
      1;
      0.000000;
     }

     EffectParamDWord {
      "bUseReflGloss";
      0;
     }
    }

    TextureFilename Diffuse {
     "1.tga";
    }
   }
  }
 }
}