///////////////////////////////////////////////////////////////////////////////
/// @file       cgi/function/hello/main.cpp
/// @~english
/// @brief      Main Implemente
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <stdexcept>

#include "hello.hpp"

/**
 * The entry point for the hello CGI
 * @param argc the number of arguments passed to the CGI
 * @param argv an array of character string arguments passed to the CGI
 * @returns a integer status code for the CGI processing
 */
int main(int argc, char** argv) {
  Hello hello(argv);

  try {
    hello.Prepare();
    hello.Run();
  } catch(const std::invalid_argument& e) {
//    std::cerr << "Error: " << e.what() << std::endl;
  } catch(const std::exception& e) {
//    std::cerr << "Error: " << e.what() << std::endl;
  } catch(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
  }

  return 0;
}
