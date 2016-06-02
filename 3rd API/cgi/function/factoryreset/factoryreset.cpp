
///////////////////////////////////////////////////////////////////////////////
/// @file       factoryreset.cpp
/// @~english
/// @brief      Reset system with factory settings
/// @~
/// @authors    Charles Lee (charles.lee@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <iostream>  // NOLINT(readability/streams)

//#define BASE_CMD    "/usr/sbin/fsreset;/sbin/reboot"

/**
 * main
 * @param argc
 * @param argv
 * @param env
 * @return
 */
int main(int argc, char *argv[], char *env[]) {
  std::cout << "HTTP/1.0 200 OK" << std::endl;
  std::cout << "Content-Type: text/plain; charset=utf-8";
  std::cout << std::endl << std::endl;

  const char* szParam = getenv("QUERY_STRING");

  if ( !szParam || !strcasecmp("?level=soft", szParam) ) {
    std::cout << "Startting soft reset..." << std::endl;
    system(BASE_CMD);
  } else if ( !strcasecmp("?level=hard", szParam) ) {
    std::cout << "[Error 500]: hard reset execution error" << std::endl;
  } else {
    std::cout << "[Error 500]: execution error" << std::endl;
  }

  return 0;
}

