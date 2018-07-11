#include "dr_Pixel.h"

namespace driderSDK {

Pixel::Pixel()
  : r(static_cast<UInt8>(0x00)),
    g(static_cast<UInt8>(0x00)),
    b(static_cast<UInt8>(0x00)),
    a(static_cast<UInt8>(0xFF)) {
}

Pixel::Pixel(UInt8 red, UInt8 green, UInt8 blue)
  : r(red), g(green), b(blue), a(static_cast<UInt8>(0xFF)) {
}

Pixel::Pixel(UInt8 red, UInt8 green, UInt8 blue, UInt8 alpha)
  : r(red), g(green), b(blue), a(alpha) {
}

Pixel::~Pixel() {
}

Pixel
Pixel::Red = Pixel(static_cast<UInt8>(0xFF), 0, 0);

Pixel
Pixel::Green = Pixel(0, static_cast<UInt8>(0xFF), 0);

Pixel
Pixel::Blue = Pixel(0, 0, static_cast<UInt8>(0xFF));

Pixel
Pixel::White = Pixel(static_cast<UInt8>(0xFF),
                     static_cast<UInt8>(0xFF),
                     static_cast<UInt8>(0xFF));

Pixel
Pixel::Black = Pixel(static_cast<UInt8>(0x0),
                     static_cast<UInt8>(0x0),
                     static_cast<UInt8>(0x0));

Pixel
Pixel::Transparent = Pixel(static_cast<UInt8>(0x0),
                           static_cast<UInt8>(0x0),
                           static_cast<UInt8>(0x0),
                           static_cast<UInt8>(0x0));

Pixel
Pixel::Orange = Pixel(static_cast<UInt8>(0xFF),
                      static_cast<UInt8>(0xA5),
                      static_cast<UInt8>(0x00));

Pixel
Pixel::Celeste = Pixel(static_cast<UInt8>(0xB2),
                       static_cast<UInt8>(0xFF),
                       static_cast<UInt8>(0xFF));

Pixel
Pixel::Purple = Pixel(static_cast<UInt8>(0x80),
                      static_cast<UInt8>(0x00),
                      static_cast<UInt8>(0x80));

Pixel
Pixel::Gray = Pixel(static_cast<UInt8>(0xD3),
                    static_cast<UInt8>(0xD3),
                    static_cast<UInt8>(0xD3));

Pixel
Pixel::Brown = Pixel(static_cast<UInt8>(0x96),
                     static_cast<UInt8>(0x4B),
                     static_cast<UInt8>(0x00));

Pixel
Pixel::Yellow = Pixel(static_cast<UInt8>(0xFF),
                      static_cast<UInt8>(0xFF),
                      static_cast<UInt8>(0x00));

Pixel
Pixel::Pink = Pixel(static_cast<UInt8>(0xFF),
                    static_cast<UInt8>(0x69),
                    static_cast<UInt8>(0xB4))

}