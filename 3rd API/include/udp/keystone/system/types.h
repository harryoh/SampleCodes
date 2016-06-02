#ifndef INCLUDE_UDP_KEYSTONE_SYSTEM_TYPES_H_
#define INCLUDE_UDP_KEYSTONE_SYSTEM_TYPES_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       system/types.h
/// @authors    Matt Clarkson (matt.clarkson@udptechnology.com)
/// @copyright  2012 UDP Technology
/// @~english
/// @brief      A set of platform independent typedefs
/// @details    These types are intended to be used across all Keystone code
///             to provide standard typedefs between modules.  The \b reason
///             we need these is because TI use XDC to build their source
///             code and use \c \<xdc/std.h\> for they're typedefs and not
///             \<stdint.h\>.  This will also provide us an easy way to
///             incorporate other platforms if they do not use \<stdint.h\>
///////////////////////////////////////////////////////////////////////////////

/* If the <xdc/std.h> is included it will already define all the types we
 * are going to need
 */
#ifndef xdc_std__include
#ifndef _TI_STD_TYPES
/**
 * @~english
 * Overrides the TI standard types header
 *  \details    The TI standard types are emulated so we must stop the
 *              inclusion of conflicting TI typedefs
 */
#define _TI_STD_TYPES

// Because Microsoft are complete muppets their stdint.h produces warnings on
// their own headers for INTPTR_MAX, INTPTR_MIN and UINTPTR_MAX.  See this bug:
// http://social.msdn.microsoft.com/Forums/en/vcgeneral/thread/45fdfff3-9f6a-411c-abe8-fc3ce0d4568b
// The following is a fix for _INTPTR - we only use it if we are compiling for
// Windows and using the Visual Studio 2010 Windows SDK 7.1 compiler
// http://msdn.microsoft.com/en-us/library/b0084kay.aspx
#if _MSC_FULL_VER == 160030319
#   ifdef _WIN64
#       define _INTPTR 2
#   else
#       define _INTPTR 0
#   endif
#endif

// Microsoft also can't write header files correctly - their 'yvals.h'
// throws the following error:
// yvals.h(773) : warning C4514: 'std::_Mutex::_Mutex' : unreferenced inline
// function has been removed
// http://msdn.microsoft.com/en-us/library/cw9x3tcf(v=vs.80).aspx
// The offending code is thus:
//  __thiscall _Mutex(_Uninitialized)
//      {   // do nothing
//      }
// We would ideally want this warning turned off just for the inclusion of
// stdint.h (which includes yval.h) but Microsoft only checks the warning at
// the end of the compilation unit.  Therefore if we disable the warning
// then reenable it - it'll still fire.  You can read about it here:
// http://stackoverflow.com/a/10477154/192993
#if _MSC_FULL_VER == 160030319
#   pragma warning(disable:4514)
#endif

/* cppline Supression
 * We need to include the standard headers after the headers above to get
 * the correct defines to make decisions on what typedefs to define
 */
#include <stddef.h>  // NOLINT(build/include_order)
#if defined(_MSC_FULL_VER) && (_MSC_FULL_VER < 160030319)
#include "udp/keystone/system/msstdint.h"
#else
#   include <stdint.h>  // NOLINT(build/include_order)
#endif
#include <stdarg.h>  // NOLINT(build/include_order)

/**
 * @~english
 * A void type
 */
#define Void                    void

/**
 * @~english
 * A character type
 * @remarks This will be the system default bit width
 */
typedef char                    Char;

/**
 * @~english
 * An unsigned character type
 * @remarks This will be the system default bit width
 */
typedef unsigned char           UChar;

/**
 * @~english
 * A short type
 * @remarks This will be the system default bit width
 * @internal
 * @par cpplint Suppressions
 * * @c runtime/int - suggests using @c intN_t rather than standard types
 *                    but we need this to emulate the @c xdc/std.h
 */
typedef short                   Short;  // NOLINT(runtime/int)

/**
 * @~english
 * An unsigned short type
 * @remarks This will be the system default bit width
 * @internal
 * @par cpplint Suppressions
 * * @c runtime/int - suggests using @c intN_t rather than standard types
 *                    but we need this to emulate the @c xdc/std.h
 */
typedef unsigned short          UShort;  // NOLINT(runtime/int)

/**
 * @~english
 * A integer type
 * @remarks This will be the system default bit width
 */
typedef int                     Int;

/**
 * @~english
 * An unsigned integer type
 * @remarks This will be the system default bit width
 */
typedef unsigned int            UInt;

/**
 * @~english
 * An unsigned integer type
 * @remarks This will be the system default bit width
 */
typedef unsigned int            Uns;

/**
 * @~english
 * A long type
 * @remarks This will be the system default bit width
 * @internal
 * @par cpplint Suppressions
 * * @c runtime/int - suggests using @c intN_t rather than standard types
 *                    but we need this to emulate the @c xdc/std.h
 */
typedef long                    Long;  // NOLINT(runtime/int)

/**
 * @~english
 * An unsigned short type
 * @remarks This will be the system default bit width
 * @internal
 * @par cpplint Suppressions
 * * @c runtime/int - suggests using @c intN_t rather than standard types
 *                    but we need this to emulate the @c xdc/std.h
 */
typedef unsigned long           ULong;  // NOLINT(runtime/int)

/**
 * @typedef LLong
 * @~english
 * A long long type
 * @remarks This will be the system default bit width
 * @internal
 * @par cpplint Suppressions
 * * @c runtime/int - suggests using @c intN_t rather than standard types
 *                    but we need this to emulate the @c xdc/std.h
 */
/**
 * @typedef ULLong
 * @~english
 * An unsigned long long type
 * @remarks This will be the system default bit width
 * @internal
 * @par cpplint Suppressions
 * * @c runtime/int - suggests using @c intN_t rather than standard types
 *                    but we need this to emulate the @c xdc/std.h
 */

#if __cplusplus >= 201103L
typedef long long               LLong;  // NOLINT(runtime/int)
typedef unsigned long long      ULLong;  // NOLINT(runtime/int)
#else
/* If there is no long long we need to emulate for the  TI types so we'd better
 * make them the longest they can be.  If someone is using these they should be
 * slapped anyway because they should be using bit specific types like UInt64,
 * etc
 */
typedef intmax_t                LLong;
typedef uintmax_t               ULLong;
#endif

/**
 * @~english
 * A float type
 * @remarks This will be the system default bit width
 */
typedef float                   Float;

/**
 * @~english
 * A double type
 * @remarks This will be the system default bit width
 */
typedef double                  Double;

/**
 * @~english
 * A long double type
 * @remarks This will be the system default bit width
 *  \since      0.1.0
 *  \ingroup    module_osal_types
 */
typedef long double             LDouble;

/**
 * @~english
 * A size type.
 *  \details    This should be used when specifying the size of an object.
 *  \par Example:
 *  \code
void foobar(const Char buffer[const], const SizeT bufferSize);
 *  \endcode
 * @remarks    This will be the system default size
 *  \since      0.1.0
 *  \ingroup    module_osal_types
 */
typedef size_t                  SizeT;

/**
 * @~english
 * A variable argument list
 * @remarks This will be the system default
 */
typedef va_list                 VaList;

/**
 * @~english
 * A boolean type
 * @warning TI use <tt>unsigned short</tt> for their @c Bool but this will
 *          mess up certain C++ specializations, so we @c typedef as @c Bool
 */
typedef bool                    Bool;

#undef FALSE
/**
 * @~english
 * A false boolean value
 * @see     #Bool
 */
#define FALSE                   false

#undef TRUE
/**
 * @~english
 * A truthful boolean value
 * @see     #Bool
 */
#define TRUE                    true

/**
 * @~english
 * A 8-bit integer
 */
typedef int8_t                  Int8;

/**
 * @~english
 * A 16-bit integer
 */
typedef int16_t                 Int16;

/**
 * @~english
 * A 32-bit integer
 */
typedef int32_t                 Int32;

/**
 * @~english
 * A 64-bit integer
 */
typedef int64_t                 Int64;

/**
 * @~english
 * A function pointer
 */
typedef Int(*Fxn)();

/**
 * @~english
 * A pointer
 */
typedef Void                   *Ptr;

/**
 * @~english
 * An string type
 * @remarks A character pointer.  Usually better to use <tt>Char *</tt> as
 *          it is more readable
 */
typedef Char                   *String;

/**
 * @~english
 * An unsigned 8-bit integer
 */
typedef uint8_t                 UInt8;

/**
 * @~english
 * An unsigned 16-bit integer
 */
typedef uint16_t                UInt16;

/**
 * @~english
 * An unsigned 32-bit integer
 */
typedef uint32_t                UInt32;

/**
 * @~english
 * An unsigned 64-bit integer
 */
typedef uint64_t                UInt64;

#endif /* _TI_STD_TYPES */
#endif /* xdc_std__include */

// These are our own types

/**
 * @~english
 * A wide character
 */
typedef wchar_t                 WChar;

#if __cplusplus >= 201103L
/**
 * @~english
 * A @c C++11 UTF16 char
 */
typedef char16_t                Char16;

/**
 * @~english
 * A @c C++11 UTF32 char
 */
typedef char32_t                Char32;
#endif

#endif  // INCLUDE_UDP_KEYSTONE_SYSTEM_TYPES_H_
