#include "dr_TGA.h"
#include <dr_file_system.h>

namespace driderSDK {

TGA::TGA() {
}

TGA::TGA(TString FilePath) {
  load(FilePath);
}

bool
TGA::load(TString FilePath) {
  m_ImageCompressed = false;

  m_Pixels.clear();

  m_width = 0;
  m_height = 0;
  m_numberOfPixels = 0;
  m_size = 0;
  m_BitsPerPixel = 0;

  std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
  
  if (!hFile.is_open()) {
    return false;
  }

  UInt8 rawHeader[18];
  hFile.read(reinterpret_cast<char*>(&rawHeader[0]), sizeof(rawHeader));
  m_header.loadFromRawData(&rawHeader[0]);
  
  std::vector<Pixel> imageData;

  bool isDecompressed = (0x2 == m_header.ImageType);
  bool isCompressed = (0xA == m_header.ImageType);
  
  m_BitsPerPixel = m_header.PixelDepth;

  if ((m_BitsPerPixel != 24) && (m_BitsPerPixel != 32)) {
    hFile.close();
    return false;
  }

  m_width = m_header.ImageWidth;
  m_height = m_header.ImageHeight;

  m_numberOfPixels = m_width * m_height;
  m_size  = ((m_width * m_BitsPerPixel + 31) / 32) * 4 * m_height;

  if (isDecompressed) {
    m_ImageCompressed = false;

    imageData.resize(m_numberOfPixels);
    hFile.read(reinterpret_cast<char*>(imageData.data()), m_size);
  }
  else if (isCompressed) {
    printf("Image is compressed, it may be loaded wrong.\n");
    m_ImageCompressed = true;

    Pixel pixel;
    UInt8 chunkHeader = { 0 };
    SizeT currentPixel = 0;
    UInt32 bytesPerPixel = m_BitsPerPixel / 8;
    imageData.resize(m_numberOfPixels);

    do {
      hFile.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));
      
      if (chunkHeader < 128) {
        ++chunkHeader;
        for (int i = 0; i < chunkHeader; ++i, ++currentPixel) {
          hFile.read(reinterpret_cast<char*>(&imageData[currentPixel]),
                     bytesPerPixel);
        }
      }
      else {
        chunkHeader -= 127;
        hFile.read(reinterpret_cast<char*>(&pixel), bytesPerPixel);
        
        for (int i = 0; i < chunkHeader; ++i, ++currentPixel) {
          hFile.read(reinterpret_cast<char*>(&imageData[currentPixel]),
                     bytesPerPixel);
        }
      }
    } while (currentPixel < m_numberOfPixels);
  }
  else {
    hFile.close();
    return false;
  }
  
  hFile.close();
  m_Pixels = imageData;
}

void
TGA::save(TString FilePath) {
  FileSystem fs;
  File file;

  fs.CreateAndOpen(FilePath, file);

  UInt8 rawHeader[18];
  m_header.getRawData(&rawHeader[0]);

  file.Write(sizeof(rawHeader), reinterpret_cast<ANSIChar*>(&rawHeader[0]));

  file.Write(m_size, reinterpret_cast<ANSIChar*>(&m_Pixels[0]));

  file.Close();
}

void
TGA::create(SizeT width, SizeT height) {
  m_ImageCompressed = false;

  m_Pixels.clear();

  m_width = static_cast<UInt32>(width);
  m_height = static_cast<UInt32>(height);
  m_numberOfPixels = static_cast<UInt32>(width * height);
  m_BitsPerPixel = static_cast<UInt32>(0x20);
  m_size = ((m_width * m_BitsPerPixel + 31) / 32) * 4 * m_height;

  m_Pixels.resize(m_numberOfPixels);

  //Header stuff
  m_header.ID_Length = static_cast<UInt8>(0x0);
  m_header.ColorMapType = static_cast<UInt8>(0x0);
  m_header.ImageType = static_cast<UInt8>(0x2);

  m_header.FirstEntryIndex = static_cast<UInt16>(0x0);
  m_header.ColorMapLength = static_cast<UInt16>(0x0);
  m_header.ColorMapEntrySize = static_cast<UInt8>(0x0);

  m_header.X_origin = static_cast<UInt16>(0x0);
  m_header.Y_origin = static_cast<UInt16>(0x0);
  m_header.ImageWidth = static_cast<UInt16>(width);
  m_header.ImageHeight = static_cast<UInt16>(height);
  m_header.PixelDepth = static_cast<UInt8>(0x20);
  m_header.ImageDescriptor = static_cast<UInt16>(0x24);
}

std::vector<Pixel>
TGA::GetPixels() {
  return m_Pixels;
}

Pixel&
TGA::GetPixel(SizeT pixelIndex) {
  return m_Pixels[pixelIndex];
}

UInt32
TGA::GetWidth() const {
  return m_width;
}

UInt32
TGA::GetHeight() const {
  return m_height;
}

bool
TGA::HasAlphaChannel() {
  return (32 == m_BitsPerPixel);
}

}