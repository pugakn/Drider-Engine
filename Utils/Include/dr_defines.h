#pragma once

#define DR_OPSYS 
#define DR_OPSYS_WINDOWS 68
#define DR_OPSYS_LINUX 69
#define DR_OPSYS_ANDROID 70
#define DR_OPSYS_IOS 71
#define DR_OPSYS_MAC 72

#if defined(_DEBUG) || defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_)
#define DR_DEBUG
#else
#define DR_RELEASE
#endif

#if defined(__x86_64__) || \ //Definded by GNU 
    defined(_M_X64)          //Defined by VS
#define DR_ARCH_X86_64 
#endif

#if defined(__i386__) || \ //Defined by GNU
    defined(_M_IX86) || \  //Definded by VS
    defined (__INTEL__)    //Definded by CodeWarrior
#define DR_ARCH_X86
#endif

#if defined(__arm__) || \ //Definded by GNU 
defined(_M_ARM)           //Defined by VS
#define DR_ARCH_ARM 
#endif

#if defined(__aarch64__) //Defined by GNU
#define DR_ARCH_ARM_64
#endif

namespace driderSDK {

}