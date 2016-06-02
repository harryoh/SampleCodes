#ifndef TYCOCGI_H_
#define TYCOCGI_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       tycocgi.hpp
/// @brief      The class for tyco
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <memory>

#include "udp/keystone/system/types.h"

#include "cgibase.hpp"

namespace udp {
namespace cgi {

/**
 * Common feature class.
 * This class parse query string and print header.
 */
class TycoCGI : public CGIBase {
public:  /* Handle Error */
  /**
   * Error Code list for Tyco
   */
  enum ErrorCode {
    /**
     * parameter value error
     */
    kCodeValueError = 0,
    /**
     * don't authorized
     */
    kCodeUnauthorized,
    /**
     * forbidden error
     */
    kCodeForbidden,
    /**
     * parameter name error
     */
    kCodeNameError,
    /**
     * missing required parameter
     */
    kCodeMissingParameter,
    /**
     * exection error
     */
    kCodeExecutionError
  };

public:   /* Constructors */
  /**
   * Default constructor
   */
  TycoCGI();

  /**
   * Default constructor
   * @param querystring The query string will be setting
   */
  explicit TycoCGI(const std::string &querystring);

  /**
   * Default deconstructor
   */
  virtual ~TycoCGI();

public:  /* Public member functions */
  /**
   * Valid check for parameter
   */
  virtual void CheckParameter() const;

  virtual void Run() {}

private:  /* Private methods */
  /**
   * CGIBase implementation class
   */
  class TycoCGIImpl;

  /**
   * CGIBase implementation variable
   */
  const std::auto_ptr<TycoCGIImpl> pimpl_;
};  // class TycoCGI

}   // namespace cgi
}   // namespace udp


#endif  // TYCOCGI_H_
