///////////////////////////////////////////////////////////////////////////////
/// @file       cgi/vendor/tyco/example.cpp
/// @~english
/// @brief      test code
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <stdexcept>

#include "udp/keystone/system/types.h"

#include "tycoerror.hpp"
#include "tycocgi.hpp"


/**
 * main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
  const std::string querystring = argv[1] ? argv[1]:"";
  udp::cgi::TycoCGI cgi(querystring);

try {
  cgi.CheckParameter();
  cgi.PrintHeader();
  std::cout << cgi << std::endl;
} catch(const udp::cgi::TycoError &e) {
  cgi.PrintHeader();
  cgi.Print(e.what());
} catch(const std::exception &e) {
  std::cerr << e.what() << std::endl;
} catch(...) {
}

  return 0;
}
