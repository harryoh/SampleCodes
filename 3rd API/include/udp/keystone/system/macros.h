#ifndef INCLUDE_UDP_KEYSTONE_SYSTEM_MACROS_H_
#define INCLUDE_UDP_KEYSTONE_SYSTEM_MACROS_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       system/macros.h
/// @~english
/// @brief      Preprocessor helper macros.
/// @~
/// @authors    Matt Clarkson (matt.clarkson@udptechnology.com)
/// @authors    Mark Bianchi (mark.bianchi@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////
#include <string>  // only needed for FILENAME macro

/**
 * @def EXTERN_CPP_START
 * @~english
 * Begins a C region when included in a C++ file.
 *
 * @def EXTERN_CPP_END
 * @~english
 * Ends a C region in C++ files.
 */
#ifdef __cplusplus
#   define EXTERN_CPP_START     extern "C" {
#   define EXTERN_CPP_END       }
#else
#   define EXTERN_CPP_START
#   define EXTERN_CPP_END
#endif

/**
 * @def DLL_PUBLIC
 * @~english
 * Enables public export of function in shared library
 *
 * @def DLL_LOCAL
 * @~english
 * Disables public export of function in shared library
 */
#if defined _WIN32 || defined __CYGWIN__
#   ifdef BUILDING_DLL
#       ifdef __GNUC__
#           define DLL_PUBLIC  __attribute__ ((dllexport))
#       else
#           define DLL_PUBLIC  __declspec(dllexport)
#       endif
#   else
#       ifdef __GNUC__
#           define DLL_PUBLIC  __attribute__ ((dllimport))
#       else
#           define DLL_PUBLIC  __declspec(dllimport)
#       endif
#   endif
#   define DLL_LOCAL
#elif __GNUC__ >= 4
#   define DLL_PUBLIC          __attribute__ ((visibility ("default")))
#   define DLL_LOCAL           __attribute__ ((visibility ("hidden")))
#else
#   define DLL_PUBLIC
#   define DLL_LOCAL
#endif

/**
 * @def FILENAME
 * @~english
 * Macro for shortening __FILE__ name. Useful macro for extracting just the
 * name of the file out of the full path __FILE__
 */
#ifndef FILENAME
#   define FILENAME     ((strrchr(__FILE__, '/') ? 0 : __FILE__- 1) + 1)
#endif

/**
 * @~english
 * Annotation for switch-case statements without a break. Use this define in
 * case statements that do not have a break statement
 * @code
 * switch(some_int) {
 *     case 1:
 *         fallthrough;
 *     case 2:
 *         doSomething();
 *         break;
 *     default:
 *         doSomethingElse();
 *         break;
 * }
 * @endcode
 */
#define fallthrough do { } while (0)

#endif  // INCLUDE_UDP_KEYSTONE_SYSTEM_MACROS_H_
