#ifndef HTTPHEADER_H_
#define HTTPHEADER_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       httpheader.hpp
/// @brief      httpheader
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <memory>

#include "udp/keystone/system/types.h"

namespace udp {
namespace cgi {

/**
 * Set/Get HTTP Header
 */
class HttpHeader {
public:  /* Public type definition */
  /**
   * Content-Type List
   */
  enum ContentType {
    /*
     * Content-Type: text/plain
     */
    kContentTypePlain,
    /*
     * Content-Type: text/html
     */
    kContentTypeHtml,
    /*
     * Content-Type: text/xml
     */
    kContentTypeXml
  };

public:  /* Construntors */
  HttpHeader();
  ~HttpHeader();

public:  /* Public member function */
  /**
   * Set Content-Type of http header menually
   * @param content_type string of content-type
   */
  void SetContentType(const std::string &content_type) const;

  /**
   * Set Status code of http header
   * @param status status code. if it is success, status code is 200.
   */
  void SetStatus(const Int &status) const;

  /**
   * Set Charset of http header
   * @param charset the string of charset
   */
  void SetCharset(const std::string &charset) const;

  /**
   * Get Content-Type of Http Header
   * @return The string of Content-Type
   */
  const std::string GetContentType() const;

  /**
   * Get Status Code of Http header
   * @return status code of http header
   */
  const Int GetStatus() const;

  /**
   * Get Charset of Http header
   * @return The string of charset
   */
  const std::string GetCharset() const;

private:  /* Private methods */
  /**
   * HttpHeaderImpl implementation class
   */
  class HttpHeaderImpl;

  /**
   * HttpHeaderImpl implementation variable
   */
  std::auto_ptr<HttpHeaderImpl> pimpl_;
};  // class Http_Header

}  // namespace cgi
}  // namespace udp

#endif  // HTTPHEADER_H_
