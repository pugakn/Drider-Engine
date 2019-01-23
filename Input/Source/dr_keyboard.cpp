#include "dr_keyboard.h"
#include "dr_input_manager.h"

namespace driderSDK {

Keyboard::Keyboard(Pass) 
  : m_callbacks(2),
    m_anyKeyCallbacks(2),
    m_keyboardOIS(nullptr), 
    m_helper(*this)
{}

Keyboard::~Keyboard() 
{}

bool
Keyboard::isKeyDown(KEY_CODE::E key) {

  Keyboard* keyboard = InputManager::getKeyboard();

  OIS::KeyCode keyOIS = static_cast<OIS::KeyCode>(key);

  return keyboard->m_keyboardOIS->isKeyDown(keyOIS);
}

bool 
Keyboard::isModifierDown(KEYBOARD_MOD::E mod) {

  Keyboard* keyboard = InputManager::getKeyboard();
  
  auto keyMod = static_cast<OIS::Keyboard::Modifier>(mod);

  return keyboard->m_keyboardOIS->isModifierDown(keyMod);
}

void 
Keyboard::addCallback(KEYBOARD_EVENT::E trigger, 
                      KEY_CODE::E key, 
                      Callback callback) {

  Keyboard* keyboard = InputManager::getKeyboard();
  
  keyboard->m_callbacks[trigger][key].push_back(callback);
}

void 
Keyboard::addAnyKeyCallback(KEYBOARD_EVENT::E trigger, 
                            AnyKeyCallback callback) {
  Keyboard* keyboard = InputManager::getKeyboard();

  keyboard->m_anyKeyCallbacks[trigger].push_back(callback);
}

const char
Keyboard::getAsChar(KEY_CODE::E key) {
  
  static std::unordered_map<KEY_CODE::E, char> keysMap
  {
    {KEY_CODE::kLEFT, 37},
    {KEY_CODE::kRIGHT, 39},
    {KEY_CODE::kUP, 38},
    {KEY_CODE::kDOWN, 40},
    {KEY_CODE::kUNASSIGNED , 0},
    {KEY_CODE::kESCAPE, 0}, 
    {KEY_CODE::k1, '1'},
    {KEY_CODE::k2, '2'},
    {KEY_CODE::k3, '3'},
    {KEY_CODE::k4, '4'},
    {KEY_CODE::k5, '5'},
    {KEY_CODE::k6, '6'},
    {KEY_CODE::k7, '7'},
    {KEY_CODE::k8, '8'},
    {KEY_CODE::k9, '9'},
    {KEY_CODE::k0, '0'},
    {KEY_CODE::kNUMPAD1, '1'},
    {KEY_CODE::kNUMPAD2, '2'},
    {KEY_CODE::kNUMPAD3, '3'},
    {KEY_CODE::kNUMPAD4, '4'},
    {KEY_CODE::kNUMPAD5, '5'},
    {KEY_CODE::kNUMPAD6, '6'},
    {KEY_CODE::kNUMPAD7, '7'},
    {KEY_CODE::kNUMPAD8, '8'},
    {KEY_CODE::kNUMPAD9, '9'},
    {KEY_CODE::kNUMPAD0, '0'},
    {KEY_CODE::kADD, '+'},
    {KEY_CODE::kSUBTRACT, '-'},
    {KEY_CODE::kDIVIDE, '/'},
    {KEY_CODE::kDECIMAL, '.'},
    {KEY_CODE::kMINUS, '-'},    // - on main keyboard
    {KEY_CODE::kEQUALS, '='},
    {KEY_CODE::kBACK, 8},    // backspace
    {KEY_CODE::kTAB, 9},
    {KEY_CODE::kQ, 'q'},
    {KEY_CODE::kW, 'w'},
    {KEY_CODE::kE, 'e'},
    {KEY_CODE::kR, 'r'},
    {KEY_CODE::kT, 't'},
    {KEY_CODE::kY, 'y'},
    {KEY_CODE::kU, 'u'},
    {KEY_CODE::kI, 'i'},
    {KEY_CODE::kO, 'o'},
    {KEY_CODE::kP, 'p'},
    {KEY_CODE::kLBRACKET, '{'},
    {KEY_CODE::kRBRACKET, '}'},
    {KEY_CODE::kRETURN, 13},    // Enter on main keyboard
    {KEY_CODE::kNUMPADENTER, 13},
    {KEY_CODE::kLCONTROL, 0},
    {KEY_CODE::kA, 'a'},
    {KEY_CODE::kS, 's'},
    {KEY_CODE::kD, 'd'},
    {KEY_CODE::kF, 'f'},
    {KEY_CODE::kG, 'g'},
    {KEY_CODE::kH, 'h'},
    {KEY_CODE::kJ, 'j'},
    {KEY_CODE::kK, 'k'},
    {KEY_CODE::kL, 'l'},
    {KEY_CODE::kSEMICOLON, ';'},
    {KEY_CODE::kAPOSTROPHE, '~'},
    {KEY_CODE::kGRAVE, '�'},    // accent
    {KEY_CODE::kLSHIFT, 16},
    {KEY_CODE::kBACKSLASH, '\\'},
    {KEY_CODE::kZ, 'z'},
    {KEY_CODE::kX, 'x'},
    {KEY_CODE::kC, 'c'},
    {KEY_CODE::kV, 'v'},
    {KEY_CODE::kB, 'b'},
    {KEY_CODE::kN, 'n'},
    {KEY_CODE::kM, 'm'},
    {KEY_CODE::kCOMMA, ','},
    {KEY_CODE::kPERIOD, '.'},    // . on main keyboard
    {KEY_CODE::kSLASH, '/'},    // / on main keyboard
    {KEY_CODE::kRSHIFT, 16},
    {KEY_CODE::kMULTIPLY, '*'},    // * on numeric keypad
    {KEY_CODE::kLMENU, 0},    // left Alt
    {KEY_CODE::kSPACE, ' '},
    {KEY_CODE::kCAPITAL, 20},
    {KEY_CODE::kF1, 0},
    {KEY_CODE::kF2, 0},
    {KEY_CODE::kF3, 0},
    {KEY_CODE::kF4, 0},
    {KEY_CODE::kF5, 0},
    {KEY_CODE::kF6, 0},
    {KEY_CODE::kF7, 0},
    {KEY_CODE::kF8, 0},
    {KEY_CODE::kF9, 0},
    //{KEY_CODE::kF10, _T("")},
    //{KEY_CODE::kNUMLOCK, _T("")},
    //{KEY_CODE::kSCROLL, _T("")},    // Scroll Lock
    //{KEY_CODE::kNUMPAD7, _T("")},
    //{KEY_CODE::kNUMPAD8, _T("")},
    //{KEY_CODE::kNUMPAD9, _T("")},
    //{KEY_CODE::kSUBTRACT, _T("-")},    // - on numeric keypad
    //{KEY_CODE::kNUMPAD4, _T("")},
    //{KEY_CODE::kNUMPAD5, _T("")},
    //{KEY_CODE::kNUMPAD6, _T("")},
    //{KEY_CODE::kADD, _T("+")},    // + on numeric keypad
    //{KEY_CODE::kNUMPAD1, _T("")},
    //{KEY_CODE::kNUMPAD2, _T("")},
    //{KEY_CODE::kNUMPAD3, _T("")},
    //{KEY_CODE::kNUMPAD0, _T("")},
    //{KEY_CODE::kDECIMAL, _T(".")},    // . on numeric keypad
    //{KEY_CODE::kOEM_102, _T("")},    // < > | on UK/Germany keyboards
    //{KEY_CODE::kF11, _T("")},
    //{KEY_CODE::kF12, _T("")},
    //{KEY_CODE::kF13, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kF14, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kF15, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kKANA, _T("")},    // (Japanese keyboard)
    //{KEY_CODE::kABNT_C1, _T("")},    // / ? on Portugese (Brazilian) keyboards
    //{KEY_CODE::kCONVERT, _T("")},    // (Japanese keyboard)
    //{KEY_CODE::kNOCONVERT, _T("")},    // (Japanese keyboard)
    //{KEY_CODE::kYEN, _T("")},    // (Japanese keyboard)
    //{KEY_CODE::kABNT_C2, _T("")},    // Numpad . on Portugese (Brazilian) keyboards
    //{KEY_CODE::kNUMPADEQUALS, _T("")},    // = on numeric keypad (NEC PC98)
    //{KEY_CODE::kPREVTRACK, _T("")},    // Previous Track (kCIRCUMFLEX on Japanese keyboard)
    //{KEY_CODE::kAT, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kCOLON, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kUNDERLINE, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kKANJI, _T("")},    // (Japanese keyboard)
    //{KEY_CODE::kSTOP, _T("")},    //                     (NEC PC98)
    //{KEY_CODE::kAX, _T("")},    //                     (Japan AX)
    //{KEY_CODE::kUNLABELED, _T("")},    //                        (J3100)
    //{KEY_CODE::kNEXTTRACK, _T("")},    // Next Track
    //{KEY_CODE::kNUMPADENTER, _T("")},    // Enter on numeric keypad
    //{KEY_CODE::kRCONTROL, _T("")},
    //{KEY_CODE::kMUTE, _T("")},    // Mute
    //{KEY_CODE::kCALCULATOR, _T("")},    // Calculator
    //{KEY_CODE::kPLAYPAUSE, _T("")},    // Play / Pause
    //{KEY_CODE::kMEDIASTOP, _T("")},    // Media Stop
    //{KEY_CODE::kVOLUMEDOWN, _T("")},    // Volume -
    //{KEY_CODE::kVOLUMEUP, _T("")},    // Volume +
    //{KEY_CODE::kWEBHOME, _T("")},    // Web home
    //{KEY_CODE::kNUMPADCOMMA, _T("")},    //, _T("")}, on numeric keypad (NEC PC98)
    //{KEY_CODE::kDIVIDE, _T("")},    // / on numeric keypad
    //{KEY_CODE::kSYSRQ, _T("")},
    //{KEY_CODE::kRMENU, _T("")},    // right Alt
    //{KEY_CODE::kPAUSE, _T("")},    // Pause
    //{KEY_CODE::kHOME, _T("")},    // Home on arrow keypad
    //{KEY_CODE::kUP, _T("")},    // UpArrow on arrow keypad
    //{KEY_CODE::kPGUP, _T("")},    // PgUp on arrow keypad
    //{KEY_CODE::kLEFT, _T("")},    // LeftArrow on arrow keypad
    //{KEY_CODE::kRIGHT, _T("")},    // RightArrow on arrow keypad
    //{KEY_CODE::kEND, _T("")},    // End on arrow keypad
    //{KEY_CODE::kDOWN, _T("")},    // DownArrow on arrow keypad
    //{KEY_CODE::kPGDOWN, _T("")},    // PgDn on arrow keypad
    //{KEY_CODE::kINSERT, _T("")},    // Insert on arrow keypad
    //{KEY_CODE::kDELETE, _T("")},    // Delete on arrow keypad
    //{KEY_CODE::kLWIN, _T("")},    // Left Windows key
    //{KEY_CODE::kRWIN, _T("")},    // Right Windows key
    //{KEY_CODE::kAPPS, _T("")},    // AppMenu key
    //{KEY_CODE::kPOWER, _T("")},    // System Power
    //{KEY_CODE::kSLEEP, _T("")},    // System Sleep
    //{KEY_CODE::kWAKE, _T("")},    // System Wake
    //{KEY_CODE::kWEBSEARCH, _T("")},    // Web Search
    //{KEY_CODE::kWEBFAVORITES, _T("")},    // Web Favorites
    //{KEY_CODE::kWEBREFRESH, _T("")},    // Web Refresh
    //{KEY_CODE::kWEBSTOP, _T("")},    // Web Stop
    //{KEY_CODE::kWEBFORWARD, _T("")},    // Web Forward
    //{KEY_CODE::kWEBBACK, _T("")},    // Web Back
    //{KEY_CODE::kMYCOMPUTER, _T("")},    // My Computer
    //{KEY_CODE::kMAIL, _T("")},    // Mail
    //{KEY_CODE::kMEDIASELECT, _T("")}      // Media Select
  };
  return keysMap[key];

    //return ("a");
}

void
Keyboard::callCallbacks(KEYBOARD_EVENT::E trigger, 
                        KEY_CODE::E key) {

  auto& callbacks = m_callbacks[trigger][key];

  for (auto& callback : callbacks) {
    callback();
  }

  auto& anyKeyCallbacks = m_anyKeyCallbacks[trigger];

  for(auto& callback : anyKeyCallbacks) {
    callback(key);
  }
}

Keyboard::Helper::Helper(Keyboard& keyboard) : m_parent(keyboard) 
{}

bool 
Keyboard::Helper::keyPressed(const OIS::KeyEvent& arg) {

  KEY_CODE::E key = static_cast<KEY_CODE::E>(arg.key);

  m_parent.callCallbacks(KEYBOARD_EVENT::kKeyPressed, key);
  
  return true;
}

bool 
Keyboard::Helper::keyReleased(const OIS::KeyEvent& arg) {

  m_parent.callCallbacks(KEYBOARD_EVENT::kKeyReleased, 
                         static_cast<KEY_CODE::E>(arg.key));

  return true;
}

}