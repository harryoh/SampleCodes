#ifndef OUTPUTTER_H_
#define OUTPUTTER_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       outputter.hpp
/// @brief      Print CGI results.
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <memory>

#include "udp/keystone/system/types.h"

#include "httpheader.hpp"

namespace udp {
namespace cgi {

/**
 * Set or Print HTTP Header
 */
class Outputter {
public:  /* Public type definition */
  /**
   * Content-Type List
   */
  enum Style {
    /**
     * Content-Type: text/plain
     */
    kStyleText,
    /**
     * Content-Type: text/html
     */
    kStyleXml,
    /**
     * Content-Type: text/xml
     */
    kStyleJson
  };

public:  /* Construntors */
  /**
   * Default Constructor
   */
  Outputter();

  /**
   * Default Distructor
   */
  ~Outputter();

  /**
   * Constructor for printing msg
   * @param msg A message that will be printed
   */
  explicit Outputter(const std::string &msg);

public:  /* Public member function */
  /**
   * Set ouput style
   * @param style kStyleText, kStyleXml, kStyleJson
   */
  void SetStyle(const Style &style);

  /**
   * Get output style
   * @return output style
   */
  const Style GetStyle() const;

  /**
   * Print output as Style
   * @param str The string to print
   */
  void Print(const std::string &str) const;

private:  /* Private methods */
  /**
   * OutputterImpl implementation class
   */
  class OutputterImpl;

  /**
   * OutputterImpl implementation variable
   */
  std::auto_ptr<OutputterImpl> pimpl_;
};  // class Http_Header

}  // namespace cgi
}  // namespace udp

#endif  // HTTP_OUTPUTTER_H_
