#ifndef CXX_INC_MAIN_H_  // NOLINT(build/header_guard)
#define CXX_INC_MAIN_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       test/main.h
/// @~english
/// @brief      C++ test file.
/// @~
/// @copyright  UDP Technology
/// @author     Matt Clarkson (matt.clarkson@udptechnology.com)
///////////////////////////////////////////////////////////////////////////////

/* cpplint Supression
 * The above NOLINT(build/header_guard) needs to be in place because Bamboo
 * checks out the tests folder directly.  cpplint incorrectly reports the
 * correct define due to a different folder layout when ran as part of the
 * normal tool chain checkout.
 */

/* cpplint Supression
 * It asks to add a folder before the include file, but in this case, we're
 * just doing a simple build, so we can ignore it
 */
#include "macros.h"  // NOLINT(build/include)

#endif  // CXX_INC_MAIN_H_  // NOLINT(build/header_guard)
