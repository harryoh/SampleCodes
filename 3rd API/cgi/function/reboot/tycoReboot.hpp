#ifndef TYCOREBOOT_H_
#define TYCOREBOOT_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       tycoReboot.hpp
/// @brief      The tycoReboot class
/// @~
/// @authors    Charles Lee (charles.lee@udptechnology.com)
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
class CTycoReboot : public udp::cgi::TycoCGI {
public:  /* Constructs */
  /**
   * Default constructor
   */
  CTycoReboot();

  /**
   * querystring constructor
   * @param querystring The query string will be setting
   */
  explicit CTycoReboot(const std::string &querystring);

  /**
   * Construct a new tycoReboot class using arguments.
   * You can test cgi without webserver.
   * @param argv querystring is set by argument.
   */
  explicit CTycoReboot(Char** argv);

public: /* Public members functions */
  virtual void Run();

public: /* Public static class*/
  /**
   * This class should be registered into map table.
   */
  static const udp::cgi::Creator<CTycoReboot> creator_;
};

#endif  // TYCOREBOOT_H_
