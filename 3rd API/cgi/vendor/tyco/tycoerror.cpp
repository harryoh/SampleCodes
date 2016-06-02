///////////////////////////////////////////////////////////////////////////////
/// @file       tycoerror.cpp
/// @brief      tycoerror class implementation code
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <cassert>

#include "tycoerror.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

static const std::string s_error_table[] = {
"[Error 400]: param value",
"[Error 401]: unauthorized",
"forbidden",
"[Error 404]: param name",
"[Error 412]: missing required parameter",
"[Error 500]: execution error"
};

///////////////////////////////////////////////////////////////////////////////
TycoError::TycoError(const TycoCGI::ErrorCode &errorcode,
                           const std::string &msg)
  : errorcode_(errorcode), message_("") {
  message_ = s_error_table[errorcode];
  if (msg.empty() == TRUE) {
    message_ += "\r\n";
  } else {
    message_ += " [" + msg + "].\r\n";
  }
}

///////////////////////////////////////////////////////////////////////////////
TycoError::~TycoError() throw() {
}

///////////////////////////////////////////////////////////////////////////////
const char* TycoError::what() const throw() {
  return message_.c_str();
}

}   // namespace cgi
}   // namespace udp
