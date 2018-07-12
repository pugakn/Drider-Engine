#include "dr_TGAHeader.h"

namespace driderSDK {

TGAHeader::TGAHeader() {
}

TGAHeader::~TGAHeader() {
}

void
TGAHeader::loadFromRawData(UInt8* data) {
  ID_Length = reinterpret_cast<UInt8&>(data[0]);
  ColorMapType = reinterpret_cast<UInt8&>(data[1]);
  ImageType = reinterpret_cast<UInt8&>(data[2]);

  FirstEntryIndex = reinterpret_cast<UInt16&>(data[3]);
  ColorMapLength = reinterpret_cast<UInt16&>(data[5]);
  ColorMapEntrySize = reinterpret_cast<UInt8&>(data[7]);

  X_origin = reinterpret_cast<UInt16&>(data[8]);
  Y_origin = reinterpret_cast<UInt16&>(data[10]);
  ImageWidth = reinterpret_cast<UInt16&>(data[12]);
  ImageHeight = reinterpret_cast<UInt16&>(data[14]);
  PixelDepth = reinterpret_cast<UInt8&>(data[16]);
  ImageDescriptor = reinterpret_cast<UInt8&>(data[17]);
}

void
TGAHeader::getRawData(UInt8* data) {
  reinterpret_cast<UInt8&>(data[0]) = ID_Length;
  reinterpret_cast<UInt8&>(data[1]) = ColorMapType;
  reinterpret_cast<UInt16&>(data[2]) = ImageType;

  reinterpret_cast<UInt16&>(data[3]) = FirstEntryIndex;
  reinterpret_cast<UInt16&>(data[5]) = ColorMapLength;
  reinterpret_cast<UInt8&>(data[7]) = ColorMapEntrySize;

  reinterpret_cast<UInt16&>(data[8]) = X_origin;
  reinterpret_cast<UInt16&>(data[10]) = Y_origin;
  reinterpret_cast<UInt16&>(data[12]) = ImageWidth;
  reinterpret_cast<UInt16&>(data[14]) = ImageHeight;
  reinterpret_cast<UInt16&>(data[16]) = PixelDepth;
  reinterpret_cast<UInt8&>(data[17]) = ImageDescriptor;
}

}