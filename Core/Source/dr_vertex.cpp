#include "dr_vertex.h"

namespace driderSDK {


const Vertex::InputLayoutDesc&
Vertex::getInputDesc() {

  /***
  String semanticName;
  UInt32 semanticIndex;
  DR_FORMAT::E format;
  UInt32 inputSlot;
  UInt32 offset;
  **/
  static InputLayoutDesc inputDesc;

  if (inputDesc.empty()) {
    DrInputElementDesc ie;
    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
    ie.offset = 0;
    ie.semanticName = "POSITION";
    inputDesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
    ie.offset = 16;
    ie.semanticName = "NORMAL";
    inputDesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
    ie.offset = 32;
    ie.semanticName = "BINORMAL";
    inputDesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
    ie.offset = 48;
    ie.semanticName = "TANGENT";
    inputDesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_FLOAT;
    ie.offset = 64;
    ie.semanticName = "BONEWEIGHTS";
    inputDesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32B32A32_SINT;
    ie.offset = 80;
    ie.semanticName = "BONEIDS";
    inputDesc.push_back(ie);

    ie.format = DR_FORMAT::kDrFormat_R32G32_FLOAT;
    ie.offset = 96;
    ie.semanticName = "TEXCOORD";
    inputDesc.push_back(ie);

  }

  return inputDesc;
}

}