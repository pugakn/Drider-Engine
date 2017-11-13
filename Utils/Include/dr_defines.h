#pragma once
#pragma warning(disable : 4201)

#define DR_PLATFORM_WINDOWS	  1   //Windows Platform 
#define DR_PLATFORM_LINUX	    2	  //Linux Platform
#define DR_PLATFORM_OSX		    3	  //Mac Platfomr
#define DR_PLATFORM_IOS		    4	  //iPhone Platform
#define DR_PLATFORM_ANDROID	  5	  //Android Platform
#define DR_PLATFORM_PS4		    6	  //Play Station 4 Platform

#define DR_COMPILER_MSV   1   //VS Compiler
#define DR_COMPILER_GNU   2   //GCC Compiler
#define DR_COMPILER_INTEL 3   //Intel Compiler
#define DR_COMPILER_CLANG 4   //Clang Compiler

#define DR_ARCHITECTURE_X86_32 1		  
#define DR_ARCHITECTURE_X86_64 2

#define DR_ENDIAN_LITTLE  1	        //Little endinan
#define DR_ENDIAN_BIG	    2	        //Big endian

#define DR_ENDINAN DR_ENDIAN_LITTLE

/**********************************************************************
*																	                                    *
*                             Debug Macros							              *
*																	                                    *
**********************************************************************/

//If this is a debug build
#if defined(_DEBUG) || defined(DEBUG)	
# define DR_DEBUG_MODE 1			  //Debug mode is on		  
#else
# define DR_DEBUG_MODE 0			  //Debug mode is off
#endif

#if DR_DEBUG_MODE
# define DR_ASSERT(x) assert(x)		  
# define DR_DEBUG_ONLY(x) x
#else
# define DR_ASSERT(x) 
# define DR_DEBUG_ONLY(x)
#endif

/**********************************************************************
*																	                                    *
*						                  Compiler									              *
*																	                                    *
**********************************************************************/

#if defined( _MSC_VER )                       //Visual Studio
# define DR_COMPILER DR_COMPILER_MSV          //Set as Actual Compiler
# define DR_COMP_VER _MSC_VER                 //Compiler version
# define DR_THREADLOCAL __declspec(thread)    //Local Thread type
#elif defined( __GNUC__ )                     //GCC Compiler
# define DR_COMPILER DR_COMPILER_GNU		  //Set as Actual Compiler
//Compiler version (computed from integrated defines)
# define DR_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
# define DR_THREADLOCAL __thread              //Local Thread type
#elif defined ( __INTEL_COMPILER )            //Intel compiler
# define DR_COMPILER DR_COMPILER_INTEL        //Set as Actual Compiler
# define DR_COMP_VER __INTEL_COMPILER         //Compiler version
/** 
 * DR_THREADLOCAL define is down below because Intel compiler defines it
 * differently based on platform
 */
#elif defined ( __clang__ )                   //Clang compiler
# define DR_COMPILER DR_COMPILER_CLANG        //Set as Actual Compiler
# define DR_COMP_VER __clang_version__        //Compiler version
# define DR_THREADLOCAL __thread              //Local Thread type
#else
//No know compiler found, send the error to the output (if any)
# pragma error "No known compiler. "
#endif

/**********************************************************************
*                                 																	  *
*                           Current Platform							            *
*                       					          												  *
**********************************************************************/

#if defined(_WIN32) || defined(__WIN32__)	  //Windows OS
# define DR_PLATFORM DR_PLATFORM_WINDOWS
#elif defined(__APPLE__)					  //Apple Platform
# if defined(__MACH__)						  //OSX Platform
#	define DR_PLATFORM DR_PLATFORM_OSX		  
# else 
#	define DR_PLATFORM DR_PLATFORM_IOS		  //IOS Platform
#endif
#elif defined(__ANDROID__)					  //Android Platform
# define DR_PLATFORM DR_PLATFORM_ANDROID
#elif defined(__ORBIS__)					  //PS4 Platform
# define DR_PLATFORM DR_PLATFORM_PS4 
#else										  //Linux Platform
# define DR_PLATFORM DR_PLATFORM_LINUX		  
#endif

/**********************************************************************
*																	                                    *
*					              Force inline & Restrict						            *
*																	                                    *
**********************************************************************/

#if DR_COMPILER == DR_COMPILER_MSV            //If we are compiling on Visual Studio
# if DR_COMP_VER >= 1200                      //If we are on Visual Studio 6 or higher
#   define FORCEINLINE __forceinline          //Set __forceinline
#   ifndef RESTRICT
#     define RESTRICT __restrict              //No alias hint
#   endif
# endif
#elif defined(__MINGW32__)                    //If we are on a Unix type system
# if !defined(FORCEINLINE)
#   define FORCEINLINE __inline               //Set __inline
#   ifndef RESTRICT
#     define RESTRICT                         //No alias hint
#   endif
# endif
#else                                         //Any other compiler
# define FORCEINLINE __inline                 //Set __inline
# ifndef RESTRICT
#   define RESTRICT __restrict                //No alias hint
# endif
#endif

/**********************************************************************
*																	                                    *
*							                Architecture							              *
*																	                                    *
**********************************************************************/

#if defined(__x86_64__) || defined(_M_X64)    //If this is a x64 compile
# define DR_ARCH_TYPE DR_ARCHITECTURE_X86_64
#else                                         //If it's a x86 compile
# define DR_ARCH_TYPE DR_ARCHITECTURE_X86_32
#endif