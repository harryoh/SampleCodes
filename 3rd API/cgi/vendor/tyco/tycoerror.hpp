#ifndef TYCOERROR_H_
#define TYCOERROR_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       tycoerror.hpp
/// @brief      Error messages for Tyco
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <memory>

#include "udp/keystone/system/types.h"

#include "tycocgi.hpp"

namespace udp {
namespace cgi {

/**
 * Manage error messages
 */
class TycoError : public std::exception {
public:  /* Construntors */
  /**
   * Making tyco error class
   * @param errorcode ErrorCode
   * @param msg store error message
   */
  TycoError(const TycoCGI::ErrorCode &errorcode, const std::string &msg = "");
  virtual ~TycoError() throw();

public:  /* Public member functions */
  /**
   * Get error message
   * @return error message
   */
  virtual const char* what() const throw();

private:  /* Private methods */
  TycoCGI::ErrorCode errorcode_;
  std::string message_;
};  // class TycoError

}  // namespace cgi
}  // namespace udp

#endif  // TYCOERROR_H_
