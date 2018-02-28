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

bool Keyboard::isKeyDown(KEY_CODE::E key) {

  Keyboard* keyboard = InputManager::getKeyboard();

  OIS::KeyCode keyOIS = static_cast<OIS::KeyCode>(key);

  return keyboard->m_keyboardOIS->isKeyDown(keyOIS);
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

//const TString&
//Keyboard::getAsString(KEY_CODE::E key) {
//
//  Keyboard* keyboard = InputManager::getKeyboard();
//
//  static std::unordered_map<KEY_CODE::E, TString> keysMap
//  {
//    {KEY_CODE::kUNASSIGNED , _T("Unassigned")},
//    {KEY_CODE::kESCAPE, _T("Esc")}, 
//    {KEY_CODE::k1, _T("1")},
//    {KEY_CODE::k2, _T("2")},
//    {KEY_CODE::k3, _T("3")},
//    {KEY_CODE::k4, _T("4")},
//    {KEY_CODE::k5, _T("5")},
//    {KEY_CODE::k6, _T("6")},
//    {KEY_CODE::k7, _T("7")},
//    {KEY_CODE::k8, _T("8")},
//    {KEY_CODE::k9, _T("9")},
//    {KEY_CODE::k0, _T("0")},
//    {KEY_CODE::kMINUS, _T("Minus")},    // - on main keyboard
//    {KEY_CODE::kEQUALS, _T("=")},
//    {KEY_CODE::kBACK, _T("Back")},    // backspace
//    {KEY_CODE::kTAB, _T("Tab")},
//    {KEY_CODE::kQ, _T("Q")},
//    {KEY_CODE::kW, _T("W")},
//    {KEY_CODE::kE, _T("E")},
//    {KEY_CODE::kR, _T("R")},
//    {KEY_CODE::kT, _T("T")},
//    {KEY_CODE::kY, _T("Y")},
//    {KEY_CODE::kU, _T("U")},
//    {KEY_CODE::kI, _T("I")},
//    {KEY_CODE::kO, _T("O")},
//    {KEY_CODE::kP, _T("P")},
//    {KEY_CODE::kLBRACKET, _T("{")},
//  {KEY_CODE::kRBRACKET, _T("}")},
//    {KEY_CODE::kRETURN, _T("Return")},    // Enter on main keyboard
//    {KEY_CODE::kLCONTROL, _T("LControl")},
//    {KEY_CODE::kA, _T("A")},
//    {KEY_CODE::kS, _T("S")},
//    {KEY_CODE::kD, _T("D")},
//    {KEY_CODE::kF, _T("F")},
//    {KEY_CODE::kG, _T("G")},
//    {KEY_CODE::kH, _T("H")},
//    {KEY_CODE::kJ, _T("J")},
//    {KEY_CODE::kK, _T("K")},
//    {KEY_CODE::kL, _T("L")},
//    {KEY_CODE::kSEMICOLON, _T(";")},
//    {KEY_CODE::kAPOSTROPHE, _T("~")},
//    {KEY_CODE::kGRAVE, _T("´")},    // accent
//    {KEY_CODE::kLSHIFT, _T("LShift")},
//    {KEY_CODE::kBACKSLASH, _T("\\")},
//    {KEY_CODE::kZ, _T("Z")},
//    {KEY_CODE::kX, _T("X")},
//    {KEY_CODE::kC, _T("C")},
//    {KEY_CODE::kV, _T("V")},
//    {KEY_CODE::kB, _T("B")},
//    {KEY_CODE::kN, _T("N")},
//    {KEY_CODE::kM, _T("M")},
//    {KEY_CODE::kCOMMA, _T(",")},
//    {KEY_CODE::kPERIOD, _T("")},    // . on main keyboard
//    {KEY_CODE::kSLASH, _T("")},    // / on main keyboard
//    {KEY_CODE::kRSHIFT, _T("")},
//    {KEY_CODE::kMULTIPLY, _T("")},    // * on numeric keypad
//    {KEY_CODE::kLMENU, _T("")},    // left Alt
//    {KEY_CODE::kSPACE, _T("")},
//    {KEY_CODE::kCAPITAL, _T("")},
//    {KEY_CODE::kF1, _T("")},
//    {KEY_CODE::kF2, _T("")},
//    {KEY_CODE::kF3, _T("")},
//    {KEY_CODE::kF4, _T("")},
//    {KEY_CODE::kF5, _T("")},
//    {KEY_CODE::kF6, _T("")},
//    {KEY_CODE::kF7, _T("")},
//    {KEY_CODE::kF8, _T("")},
//    {KEY_CODE::kF9, _T("")},
//    {KEY_CODE::kF10, _T("")},
//    {KEY_CODE::kNUMLOCK, _T("")},
//    {KEY_CODE::kSCROLL, _T("")},    // Scroll Lock
//    {KEY_CODE::kNUMPAD7, _T("")},
//    {KEY_CODE::kNUMPAD8, _T("")},
//    {KEY_CODE::kNUMPAD9, _T("")},
//    {KEY_CODE::kSUBTRACT, _T("")},    // - on numeric keypad
//    {KEY_CODE::kNUMPAD4, _T("")},
//    {KEY_CODE::kNUMPAD5, _T("")},
//    {KEY_CODE::kNUMPAD6, _T("")},
//    {KEY_CODE::kADD, _T("")},    // + on numeric keypad
//    {KEY_CODE::kNUMPAD1, _T("")},
//    {KEY_CODE::kNUMPAD2, _T("")},
//    {KEY_CODE::kNUMPAD3, _T("")},
//    {KEY_CODE::kNUMPAD0, _T("")},
//    {KEY_CODE::kDECIMAL, _T("")},    // . on numeric keypad
//    {KEY_CODE::kOEM_102, _T("")},    // < > | on UK/Germany keyboards
//    {KEY_CODE::kF11, _T("")},
//    {KEY_CODE::kF12, _T("")},
//    {KEY_CODE::kF13, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kF14, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kF15, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kKANA, _T("")},    // (Japanese keyboard)
//    {KEY_CODE::kABNT_C1, _T("")},    // / ? on Portugese (Brazilian) keyboards
//    {KEY_CODE::kCONVERT, _T("")},    // (Japanese keyboard)
//    {KEY_CODE::kNOCONVERT, _T("")},    // (Japanese keyboard)
//    {KEY_CODE::kYEN, _T("")},    // (Japanese keyboard)
//    {KEY_CODE::kABNT_C2, _T("")},    // Numpad . on Portugese (Brazilian) keyboards
//    {KEY_CODE::kNUMPADEQUALS, _T("")},    // = on numeric keypad (NEC PC98)
//    {KEY_CODE::kPREVTRACK, _T("")},    // Previous Track (kCIRCUMFLEX on Japanese keyboard)
//    {KEY_CODE::kAT, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kCOLON, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kUNDERLINE, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kKANJI, _T("")},    // (Japanese keyboard)
//    {KEY_CODE::kSTOP, _T("")},    //                     (NEC PC98)
//    {KEY_CODE::kAX, _T("")},    //                     (Japan AX)
//    {KEY_CODE::kUNLABELED, _T("")},    //                        (J3100)
//    {KEY_CODE::kNEXTTRACK, _T("")},    // Next Track
//    {KEY_CODE::kNUMPADENTER, _T("")},    // Enter on numeric keypad
//    {KEY_CODE::kRCONTROL, _T("")},
//    {KEY_CODE::kMUTE, _T("")},    // Mute
//    {KEY_CODE::kCALCULATOR, _T("")},    // Calculator
//    {KEY_CODE::kPLAYPAUSE, _T("")},    // Play / Pause
//    {KEY_CODE::kMEDIASTOP, _T("")},    // Media Stop
//    {KEY_CODE::kVOLUMEDOWN, _T("")},    // Volume -
//    {KEY_CODE::kVOLUMEUP, _T("")},    // Volume +
//    {KEY_CODE::kWEBHOME, _T("")},    // Web home
//    {KEY_CODE::kNUMPADCOMMA, _T("")},    //, _T("")}, on numeric keypad (NEC PC98)
//    {KEY_CODE::kDIVIDE, _T("")},    // / on numeric keypad
//    {KEY_CODE::kSYSRQ, _T("")},
//    {KEY_CODE::kRMENU, _T("")},    // right Alt
//    {KEY_CODE::kPAUSE, _T("")},    // Pause
//    {KEY_CODE::kHOME, _T("")},    // Home on arrow keypad
//    {KEY_CODE::kUP, _T("")},    // UpArrow on arrow keypad
//    {KEY_CODE::kPGUP, _T("")},    // PgUp on arrow keypad
//    {KEY_CODE::kLEFT, _T("")},    // LeftArrow on arrow keypad
//    {KEY_CODE::kRIGHT, _T("")},    // RightArrow on arrow keypad
//    {KEY_CODE::kEND, _T("")},    // End on arrow keypad
//    {KEY_CODE::kDOWN, _T("")},    // DownArrow on arrow keypad
//    {KEY_CODE::kPGDOWN, _T("")},    // PgDn on arrow keypad
//    {KEY_CODE::kINSERT, _T("")},    // Insert on arrow keypad
//    {KEY_CODE::kDELETE, _T("")},    // Delete on arrow keypad
//    {KEY_CODE::kLWIN, _T("")},    // Left Windows key
//    {KEY_CODE::kRWIN, _T("")},    // Right Windows key
//    {KEY_CODE::kAPPS, _T("")},    // AppMenu key
//    {KEY_CODE::kPOWER, _T("")},    // System Power
//    {KEY_CODE::kSLEEP, _T("")},    // System Sleep
//    {KEY_CODE::kWAKE, _T("")},    // System Wake
//    {KEY_CODE::kWEBSEARCH, _T("")},    // Web Search
//    {KEY_CODE::kWEBFAVORITES, _T("")},    // Web Favorites
//    {KEY_CODE::kWEBREFRESH, _T("")},    // Web Refresh
//    {KEY_CODE::kWEBSTOP, _T("")},    // Web Stop
//    {KEY_CODE::kWEBFORWARD, _T("")},    // Web Forward
//    {KEY_CODE::kWEBBACK, _T("")},    // Web Back
//    {KEY_CODE::kMYCOMPUTER, _T("")},    // My Computer
//    {KEY_CODE::kMAIL, _T("")},    // Mail
//    {KEY_CODE::kMEDIASELECT, _T("")}      // Media Select
//  };
//
//  return keysMap[key];
//
//}

void Keyboard::callCallbacks(KEYBOARD_EVENT::E trigger, 
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