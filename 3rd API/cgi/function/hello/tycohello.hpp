#ifndef TYCOTEST_H_
#define TYCOTEST_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       tycohello.hpp
/// @brief      The TycoTest class
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <memory>

#include "udp/keystone/system/types.h"

#include "autoregister.hpp"
#include "tycocgi.hpp"

/**
 * Common feature class.
 */
class TycoHello : public udp::cgi::TycoCGI {
public:  /* Constructs */
  /**
   * Default constructor
   */
  TycoHello();

  /**
   * Default Destruct
   */
  ~TycoHello();

  /**
   * Default constructor
   * @param querystring The query string will be setting
   */
  explicit TycoHello(const std::string &querystring);

public: /* Public members functions */
  virtual void CheckParameter() const;

  virtual void Run();

public: /* Public static class*/
  /**
   * This class should be registered into map table.
   */
  static const udp::cgi::Creator<TycoHello> creator_;
};

#endif  // TYCOTEST_H_
