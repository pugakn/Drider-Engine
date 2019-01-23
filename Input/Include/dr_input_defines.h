#pragma once
namespace driderSDK {

namespace JOYSTICK_BUTTON
{
  enum E
  {
    kA,
    kB,
    kX,
    kY,
    kLB,
    kRB,
    kLS,
    kRS,
    kBack,
    kStart
  };
}

namespace JOYSTICK_AXIS
{
  enum E
  {
    //Left Trigger
    kLT = JOYSTICK_BUTTON::kStart + 1,
    //Right Trigger
    kRT,
    //Left Stick Vertical
    kLSVer, 
    //Right Stick Vertical
    kRSVer, 
    //Left Stick Horizontal
    kLSHor,  
    //Right Stick Horizontal
    kRSHor   
  };
}

namespace MOUSE_BUTTON {
enum E
{
  kLeft = 0, 
  kRight, 
  kMiddle,
  kButton3, 
  kButton4, 
  kButton5,
  kButton6, 
  kButton7
};
}

namespace JOYSTICK_EVENT {
enum E
{
  kButtonPressed,
  kButtonnReleased
};
}

namespace MOUSE_INPUT_EVENT {
enum E
{
  kButtonPressed = 0,
  kButtonReleased = 1
};
}

namespace KEYBOARD_EVENT {
enum E
{
  kKeyPressed,
  kKeyReleased
};
}

namespace KEYBOARD_MOD {
enum E
{
  kShift = 0x0000001,
  kCtrl = 0x0000010,
  kAlt =  0x0000100,
  kCapsLock = 0x0001000,
  kNumLock  = 0x0010000
};
}

namespace KEY_CODE {
enum E
{
  kUNASSIGNED = 0x00,
  kESCAPE = 0x01,
  k1 = 0x02,
  k2 = 0x03,
  k3 = 0x04,
  k4 = 0x05,
  k5 = 0x06,
  k6 = 0x07,
  k7 = 0x08,
  k8 = 0x09,
  k9 = 0x0A,
  k0 = 0x0B,
  kMINUS = 0x0C,    // - on main keyboard
  kEQUALS = 0x0D,
  kBACK = 0x0E,    // backspace
  kTAB = 0x0F,
  kQ = 0x10,
  kW = 0x11,
  kE = 0x12,
  kR = 0x13,
  kT = 0x14,
  kY = 0x15,
  kU = 0x16,
  kI = 0x17,
  kO = 0x18,
  kP = 0x19,
  kLBRACKET = 0x1A,
  kRBRACKET = 0x1B,
  kRETURN = 0x1C,    // Enter on main keyboard
  kLCONTROL = 0x1D,
  kA = 0x1E,
  kS = 0x1F,
  kD = 0x20,
  kF = 0x21,
  kG = 0x22,
  kH = 0x23,
  kJ = 0x24,
  kK = 0x25,
  kL = 0x26,
  kSEMICOLON = 0x27,
  kAPOSTROPHE = 0x28,
  kGRAVE = 0x29,    // accent
  kLSHIFT = 0x2A,
  kBACKSLASH = 0x2B,
  kZ = 0x2C,
  kX = 0x2D,
  kC = 0x2E,
  kV = 0x2F,
  kB = 0x30,
  kN = 0x31,
  kM = 0x32,
  kCOMMA = 0x33,
  kPERIOD = 0x34,    // . on main keyboard
  kSLASH = 0x35,    // / on main keyboard
  kRSHIFT = 0x36,
  kMULTIPLY = 0x37,    // * on numeric keypad
  kLMENU = 0x38,    // left Alt
  kSPACE = 0x39,
  kCAPITAL = 0x3A,
  kF1 = 0x3B,
  kF2 = 0x3C,
  kF3 = 0x3D,
  kF4 = 0x3E,
  kF5 = 0x3F,
  kF6 = 0x40,
  kF7 = 0x41,
  kF8 = 0x42,
  kF9 = 0x43,
  kF10 = 0x44,
  kNUMLOCK = 0x45,
  kSCROLL = 0x46,    // Scroll Lock
  kNUMPAD7 = 0x47,
  kNUMPAD8 = 0x48,
  kNUMPAD9 = 0x49,
  kSUBTRACT = 0x4A,    // - on numeric keypad
  kNUMPAD4 = 0x4B,
  kNUMPAD5 = 0x4C,
  kNUMPAD6 = 0x4D,
  kADD = 0x4E,    // + on numeric keypad
  kNUMPAD1 = 0x4F,
  kNUMPAD2 = 0x50,
  kNUMPAD3 = 0x51,
  kNUMPAD0 = 0x52,
  kDECIMAL = 0x53,    // . on numeric keypad
  kOEM_102 = 0x56,    // < > | on UK/Germany keyboards
  kF11 = 0x57,
  kF12 = 0x58,
  kF13 = 0x64,    //                     (NEC PC98)
  kF14 = 0x65,    //                     (NEC PC98)
  kF15 = 0x66,    //                     (NEC PC98)
  kKANA = 0x70,    // (Japanese keyboard)
  kABNT_C1 = 0x73,    // / ? on Portugese (Brazilian) keyboards
  kCONVERT = 0x79,    // (Japanese keyboard)
  kNOCONVERT = 0x7B,    // (Japanese keyboard)
  kYEN = 0x7D,    // (Japanese keyboard)
  kABNT_C2 = 0x7E,    // Numpad . on Portugese (Brazilian) keyboards
  kNUMPADEQUALS = 0x8D,    // = on numeric keypad (NEC PC98)
  kPREVTRACK = 0x90,    // Previous Track (kCIRCUMFLEX on Japanese keyboard)
  kAT = 0x91,    //                     (NEC PC98)
  kCOLON = 0x92,    //                     (NEC PC98)
  kUNDERLINE = 0x93,    //                     (NEC PC98)
  kKANJI = 0x94,    // (Japanese keyboard)
  kSTOP = 0x95,    //                     (NEC PC98)
  kAX = 0x96,    //                     (Japan AX)
  kUNLABELED = 0x97,    //                        (J3100)
  kNEXTTRACK = 0x99,    // Next Track
  kNUMPADENTER = 0x9C,    // Enter on numeric keypad
  kRCONTROL = 0x9D,
  kMUTE = 0xA0,    // Mute
  kCALCULATOR = 0xA1,    // Calculator
  kPLAYPAUSE = 0xA2,    // Play / Pause
  kMEDIASTOP = 0xA4,    // Media Stop
  kVOLUMEDOWN = 0xAE,    // Volume -
  kVOLUMEUP = 0xB0,    // Volume +
  kWEBHOME = 0xB2,    // Web home
  kNUMPADCOMMA = 0xB3,    // , on numeric keypad (NEC PC98)
  kDIVIDE = 0xB5,    // / on numeric keypad
  kSYSRQ = 0xB7,
  kRMENU = 0xB8,    // right Alt
  kPAUSE = 0xC5,    // Pause
  kHOME = 0xC7,    // Home on arrow keypad
  kUP = 0xC8,    // UpArrow on arrow keypad
  kPGUP = 0xC9,    // PgUp on arrow keypad
  kLEFT = 0xCB,    // LeftArrow on arrow keypad
  kRIGHT = 0xCD,    // RightArrow on arrow keypad
  kEND = 0xCF,    // End on arrow keypad
  kDOWN = 0xD0,    // DownArrow on arrow keypad
  kPGDOWN = 0xD1,    // PgDn on arrow keypad
  kINSERT = 0xD2,    // Insert on arrow keypad
  kDELETE = 0xD3,    // Delete on arrow keypad
  kLWIN = 0xDB,    // Left Windows key
  kRWIN = 0xDC,    // Right Windows key
  kAPPS = 0xDD,    // AppMenu key
  kPOWER = 0xDE,    // System Power
  kSLEEP = 0xDF,    // System Sleep
  kWAKE = 0xE3,    // System Wake
  kWEBSEARCH = 0xE5,    // Web Search
  kWEBFAVORITES = 0xE6,    // Web Favorites
  kWEBREFRESH = 0xE7,    // Web Refresh
  kWEBSTOP = 0xE8,    // Web Stop
  kWEBFORWARD = 0xE9,    // Web Forward
  kWEBBACK = 0xEA,    // Web Back
  kMYCOMPUTER = 0xEB,    // My Computer
  kMAIL = 0xEC,    // Mail
  kMEDIASELECT = 0xED     // Media Select
};
}
}