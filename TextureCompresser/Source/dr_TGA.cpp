#include "dr_TGA.h"
#include <fstream>

namespace driderSDK {

Tga::Tga() {
}

Tga::Tga(const char* FilePath) {
  std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
  
  if (!hFile.is_open()) {
    throw std::invalid_argument("File Not Found.");
  }
  
  UInt8 header[18] = { 0 };
  std::vector<PixelInfo> imageData;

  UInt8 isDecompressed[12] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
  UInt8 isCompressed[12] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

  hFile.read(reinterpret_cast<char*>(&header), sizeof(header));
  
  m_BitsPerPixel = header[16];

  if ((m_BitsPerPixel != 24) && (m_BitsPerPixel != 32)) {
    hFile.close();
    throw std::invalid_argument("Error. Required: 24 or 32 Bit Image.");
  }

  m_width  = header[13] * 256 + header[12];
  m_height = header[15] * 256 + header[14];
  m_numberOfPixels = m_width * m_height;
  m_size  = ((m_width * m_BitsPerPixel + 31) / 32) * 4 * m_height;

  if (!std::memcmp(isDecompressed, &header, sizeof(isDecompressed))) {
    m_ImageCompressed = false;

    imageData.resize(m_numberOfPixels);
    hFile.read(reinterpret_cast<char*>(imageData.data()), m_size);
  }
  else if (!std::memcmp(isCompressed, &header, sizeof(isCompressed))) {
    printf("Image is compressed, it may be loaded wrong.\n");
    m_ImageCompressed = true;

    PixelInfo pixel = { 0 };
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
    throw std::invalid_argument("Error. Required: 24 or 32 Bit TGA File.");
  }
  
  hFile.close();
  m_Pixels = imageData;
}

std::vector<PixelInfo>
Tga::GetPixels() {
  return m_Pixels;
}

UInt32
Tga::GetWidth() const {
  return m_width;
}

UInt32
Tga::GetHeight() const {
  return m_height;
}

bool
Tga::HasAlphaChannel() {
  return 32 == m_BitsPerPixel;
}


}