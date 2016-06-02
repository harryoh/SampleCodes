///////////////////////////////////////////////////////////////////////////////
/// @file       httpheader.cpp
/// @brief      httpheader class implementation code
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////
#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <cassert>

#include "httpheader.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

///////////////////////////////////////////////////////////////////////////////
/**
* Private implementation of the HttpHeader API
*/
class HttpHeader::HttpHeaderImpl {
public:  /* Public constructor */
  HttpHeaderImpl()
    : content_type_("text/plain"), charset_("utf-8"), status_(200) {
  }

  ~HttpHeaderImpl() {}

public:  /* Public member functions */
  /**
   * @copydoc HttpHeader::GetContentType
   */
  const std::string GetContentType() const {
    return content_type_;
  }

  /**
   * @copydoc HttpHeader::GetStatus
   */
  const Int GetStatus() const {
    return status_;
  }

  /**
   * @copydoc HttpHeader::GetCharset
   */
  const std::string GetCharset() const {
    return charset_;
  }

  /**
   * @copydoc HttpHeader::SetContentType(const std::string &content_type) const
   */
  void SetContentType(const std::string &content_type) {
    content_type_ = content_type;
  }

  /**
   * @copydoc HttpHeader::SetStatus
   */
  void SetStatus(const Int &status) {
    status_ = status;
  }

  /**
   * @copydoc HttpHeader::SetCharset
   */
  void SetCharset(const std::string &charset) {
    charset_ = charset;
  }

private:  /* Private methods */
  /**
   * Content-Type of http header
   */
  std::string content_type_;

  /**
   * Charset of http header
   */
  std::string charset_;

  /**
   * Status code of http header
   */
  Int status_;
};

///////////////////////////////////////////////////////////////////////////////
HttpHeader::HttpHeader() : pimpl_(new HttpHeaderImpl()) {}

///////////////////////////////////////////////////////////////////////////////
HttpHeader::~HttpHeader() {}

///////////////////////////////////////////////////////////////////////////////
const std::string HttpHeader::GetContentType() const {
  assert(pimpl_.get());
  return pimpl_->GetContentType();
}

///////////////////////////////////////////////////////////////////////////////
const Int HttpHeader::GetStatus() const {
  assert(pimpl_.get());
  return pimpl_->GetStatus();
}

///////////////////////////////////////////////////////////////////////////////
const std::string HttpHeader::GetCharset() const {
  assert(pimpl_.get());
  return pimpl_->GetCharset();
}

///////////////////////////////////////////////////////////////////////////////
void HttpHeader::SetContentType(const std::string &content_type) const {
  assert(pimpl_.get());
  return pimpl_->SetContentType(content_type);
}

///////////////////////////////////////////////////////////////////////////////
void HttpHeader::SetStatus(const Int &status) const {
  assert(pimpl_.get());
  return pimpl_->SetStatus(status);
}

///////////////////////////////////////////////////////////////////////////////
void HttpHeader::SetCharset(const std::string &charset) const {
  assert(pimpl_.get());
  return pimpl_->SetCharset(charset);
}

}   // namespace cgi
}   // namespace udp
