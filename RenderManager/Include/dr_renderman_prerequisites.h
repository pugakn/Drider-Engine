#pragma once

#include <cassert>
#include <dr_defines.h>
#include <dr_types.h>

#define RENDER_MANAGER_MAX_LIGHTS 512
#define RENDER_MANAGER_MAX_LIGHTS_PER_BLOCK 128

/**********************************************************************
*															                                  		  *
*					                  Windows Specifics							            *
*																	                                    *
**********************************************************************/

#if DR_PLATFORM == DR_PLATFORM_WINDOWS
# if defined( DR_STATIC_LIB )
#   define DR_RENDERMAN_EXPORT
# else
#	if defined(DR_RENDERMAN_EXPORTS)
#   define DR_RENDERMAN_EXPORT __declspec( dllexport )
#   else
#     if defined( __MINGW32__ )
#       define DR_RENDERMAN_EXPORT              //Linux systems don't need this
#     else
#       define DR_RENDERMAN_EXPORT __declspec( dllimport )
#     endif
#   endif
# endif
#endif

/**********************************************************************
*																	                                    *
*                   	    	  Linux Specifics							            *
*																	                                    *
**********************************************************************/

#if DR_PLATFORM == DR_PLATFORM_LINUX || DR_PLATFORM == DR_PLATFORM_OSX
//Enable GCC symbol visibility
# if defined( DR_GCC_VISIBILITY )
#   define DR_RENDERMAN_EXPORT  __attribute__ ((visibility("default")))
# else
#   define DR_RENDERMAN_EXPORT
# endif

# if DR_COMPILER == DR_COMPILER_INTEL
#   define DR_THREADLOCAL __thread          //Set the local thread for the Intel compiler
# endif
#endif
