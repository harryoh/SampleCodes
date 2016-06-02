#ifndef HELLO_H_
#define HELLO_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       hello.hpp
/// @brief      The Hello class
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <memory>

#include "udp/keystone/system/types.h"

#include "tycohello.hpp"

/**
 * Common feature class.
 */
class Hello {
public:  /* Constructs */
  /**
   * Default constructor
   * @param argv the argument for debugging
   * @code
   * ./hello.cgi "/iAPI/hello.cgi?action=read"
   * @endcode
   */
  explicit Hello(Char** argv = NULL);

  /**
   * Default Destruct
   */
  ~Hello();

  /**
   * Default constructor
   * @param url request url
   */
  explicit Hello(const std::string &url);

public: /* Public members functions */
  /**
   * Preparing to use a 3rd-party class.
   * Using SCRIPT_NAME environment, creating a class.
   */
  void Prepare();

  /**
   * Realization code
   */
  void Run();

private:  /* Private method */
  /**
   * CGI Base class variable
   */
  udp::cgi::CGIBase* cgibase_;
};

#endif  // HELLO_H_
