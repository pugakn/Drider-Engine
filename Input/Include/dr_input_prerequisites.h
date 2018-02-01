#pragma once

#include <cassert>
#include <dr_defines.h>
#include <dr_types.h>

/**********************************************************************
*															                                  		  *
*					                  Windows Specifics							            *
*																	                                    *
**********************************************************************/

#if DR_PLATFORM == DR_PLATFORM_WINDOWS
# if defined( DR_STATIC_LIB )
#   define DR_INPUT_EXPORT
# else
#	if defined(INPUT_EXPORTS)
#   define DR_INPUT_EXPORT __declspec( dllexport )
#   else
#     if defined( __MINGW32__ )
#       define DR_INPUT_EXPORT              //Linux systems don't need this
#     else
#       define DR_INPUT_EXPORT __declspec( dllimport )
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
#   define DR_INPUT_EXPORT  __attribute__ ((visibility("default")))
# else
#   define DR_INPUT_EXPORT
# endif
#endif
