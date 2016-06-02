///////////////////////////////////////////////////////////////////////////////
/// @file       cgibase.cpp
/// @~english
/// @brief      Implementation of the CGIBase class
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <map>
#include <string>
#include <cassert>

#include <stdlib.h>    // NOLINT(build/include_order)

#include "qdecoder.h"  // NOLINT(build/include)
#include "cgibase.hpp"
#include "httpheader.hpp"
#include "outputter.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

///////////////////////////////////////////////////////////////////////////////
/**
* Private implementation of the CGIBase API
*/
class CGIBase::CGIBaseImpl {
public:   /* Public constructor */
  /**
   * default constructor
   */
  CGIBaseImpl() : header_(new HttpHeader()), output_(new Outputter()) {
    preq_ = qcgireq_parse(NULL, static_cast<Q_CGI_T>(Q_CGI_POST | Q_CGI_GET));
  }

  /**
   * Creates a CGIBase class using method
   * @param method The Method to get from cgi
   */
  explicit CGIBaseImpl(Method method)
    : header_(new HttpHeader()), output_(new Outputter()) {
    Q_CGI_T qmethod;
    switch (method) {
      case CGIBase::kMethodAll:
        qmethod = Q_CGI_GET;
        break;
      case CGIBase::kMethodPost:
        qmethod = Q_CGI_POST;
        break;
      case CGIBase::kMethodCookie:
        qmethod = Q_CGI_COOKIE;
        break;
      default:
        qmethod = static_cast<Q_CGI_T>(Q_CGI_POST | Q_CGI_GET);
        break;
    }
    preq_ = qcgireq_parse(NULL, qmethod);
  }

  /**
   * Creates a CGIBase class using querystring
   * @param querystring querystring set QUERY_STRING of envronment
   */
  explicit CGIBaseImpl(const std::string &querystring)
    : header_(new HttpHeader()), output_(new Outputter()) {
    if (!querystring.empty()) {
      std::string uri(static_cast<std::string>(__FILE__));
      uri += "?";
      uri += querystring;

      setenv("QUERY_STRING", querystring.c_str(), true);
      setenv("REQUEST_METHOD", "GET", true);
      setenv("REQUEST_URI", uri.c_str(), true);
    }
    preq_ = qcgireq_parse(NULL, static_cast<Q_CGI_T>(Q_CGI_GET));
  }

  /**
  * Default deconstructor
  */
  ~CGIBaseImpl() {
    preq_->free(preq_);
  }

public:   /* public functions */
  /**
   * @copydoc CGIBase::GetValue
   */
  const std::string GetValue(const std::string &name) const {
    String value = preq_->getstr(preq_, name.c_str(), false);
    return value ? static_cast<std::string>(value) : "";
  }

  /**
   * @copydoc CGIBase::GetValues
   */
  const EntryList GetValues() const {
    return GetValues(std::string());
  }

  /**
   * @copydoc CGIBase::GetValues(const std::string &name) const
   */
  const EntryList GetValues(const std::string &name) const {
    EntryList list;
    qentobj_t *obj;

    for (obj = preq_->first; obj; obj = obj->next) {
      if ((!name.empty()) && (name != static_cast<std::string>(obj->name))) {
        continue;
      }
      list.insert(
        make_pair(static_cast<std::string>(obj->name),
                  static_cast<std::string>(static_cast<String>(obj->data))));
    }

    return list;
  }

  /**
   * @copydoc CGIBase::SearchEntry
   */
  const Bool SearchEntry(const std::string &name) const {
    qentobj_t *obj;
    for (obj = preq_->first; obj; obj = obj->next) {
      if (static_cast<std::string>(obj->name) == name) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * @copydoc CGIBase::GetEnv
   */
  const std::string GetEnv(const std::string &name) const {
    String value = getenv(name.c_str());
    return (value) ? static_cast<std::string>(value) : std::string();
  }

  /**
   * Print http header. \n
   */
  void PrintHeader() const {
    assert(output_.get());
    assert(header_.get());

    std::string header;

    if (header_->GetStatus() != 200) {
      header += "status: ";
      header += header_->GetStatus();
      header += "\r\n";
    }
    header += "Content-type: " + header_->GetContentType();
    if (header_->GetCharset() == "utf8") {
      header += "; charset=";
      header += header_->GetCharset();
    }
    header += "\r\n\r\n";
    // this must print to text type.
    return output_->Print(header);
  }

  /**
   * @copydoc Outputter::Print
   */
  void Print(const std::string str) const {
    assert(output_.get());
    return output_->Print(str);
  }

  /**
   * @copydoc HttpHeader::SetContentType
   */
  void SetHeaderContentType(const std::string &content_type) const {
    assert(header_.get());
    return header_->SetContentType(content_type);
  }

  /**
   * @copydoc HttpHeader::GetContentType
   */
  const std::string GetHeaderContentType() const {
    assert(header_.get());
    return header_->GetContentType();
  }

  /**
   * @copydoc HttpHeader::SetStatus
   */
  void SetHeaderStatus(const Int &status) const {
    assert(header_.get());
    return header_->SetStatus(status);
  }

  /**
   * @copydoc HttpHeader::GetStatus
   */
  const Int GetHeaderStatus() const {
    assert(header_.get());
    return header_->GetStatus();
  }

  /**
   * @copydoc HttpHeader::SetCharset
   */
  void SetHeaderCharset(const std::string &charset) const {
    assert(header_.get());
    return header_->SetCharset(charset);
  }

  /**
   * @copydoc HttpHeader::GetCharset
   */
  const std::string GetHeaderCharset() const {
    assert(header_.get());
    return header_->GetCharset();
  }

private:    /* Private method */
  /**
   * qdecoder struct
   */
  qentry_t *preq_;

  /**
   * HTTP Header Class
   */
  const std::auto_ptr<HttpHeader> header_;

  /**
   * Print Output Class
   */
  const std::auto_ptr<Outputter> output_;
};

///////////////////////////////////////////////////////////////////////////////
CGIBase::CGIBase()
  : pimpl_(new CGIBaseImpl(CGIBase::kMethodAll)) {}

///////////////////////////////////////////////////////////////////////////////
CGIBase::~CGIBase() {}

///////////////////////////////////////////////////////////////////////////////
CGIBase::CGIBase(const Method &method)
  : pimpl_(new CGIBaseImpl(method)) {}

///////////////////////////////////////////////////////////////////////////////
CGIBase::CGIBase(const std::map<std::string, std::string> &env)
  : pimpl_(new CGIBaseImpl(CGIBase::kMethodAll)) {
  std::map<std::string, std::string>::const_iterator iter;

  for (iter = env.begin(); iter != env.end(); ++iter) {
    setenv(iter->first.c_str(), iter->second.c_str(), true);
  }
}

///////////////////////////////////////////////////////////////////////////////
CGIBase::CGIBase(const std::string &querystring)
  : pimpl_(new CGIBaseImpl(querystring)) {}

///////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &stream, CGIBase &cgi) {
  CGIBase::EntryList entrylist = cgi.GetValues();
  CGIBase::EntryList::iterator iter;

  stream << "{";
  for (iter = entrylist.begin(); iter != entrylist.end(); ++iter) {
    stream << (iter == entrylist.begin() ? "" : ", ") << "\""
           <<iter->first << "\":\"" << iter->second << "\"";
  }
  stream << "}";
  return stream;
}

///////////////////////////////////////////////////////////////////////////////
const std::string CGIBase::GetValue(const std::string &name) const {
  assert(pimpl_.get());
  return pimpl_->GetValue(name);
}

///////////////////////////////////////////////////////////////////////////////
CGIBase::EntryList CGIBase::GetValues() const {
  assert(pimpl_.get());
  return pimpl_->GetValues();
}

///////////////////////////////////////////////////////////////////////////////
const CGIBase::EntryList CGIBase::GetValues(const std::string &name) const {
  assert(pimpl_.get());
  return pimpl_->GetValues(name);
}

///////////////////////////////////////////////////////////////////////////////
const Bool CGIBase::SearchEntry(const std::string &name) const {
  assert(pimpl_.get());
  return pimpl_->SearchEntry(name);
}

///////////////////////////////////////////////////////////////////////////////
const std::string CGIBase::GetEnv(const std::string &name) const {
  assert(pimpl_.get());
  return pimpl_->GetEnv(name);
}

///////////////////////////////////////////////////////////////////////////////
void CGIBase::Print(const std::string &str) const {
  assert(pimpl_.get());
  return pimpl_->Print(str);
}

///////////////////////////////////////////////////////////////////////////////
void CGIBase::PrintHeader() const {
  assert(pimpl_.get());
  return pimpl_->PrintHeader();
}

///////////////////////////////////////////////////////////////////////////////
void CGIBase::SetHeaderContentType(const std::string &content_type) const {
  assert(pimpl_.get());
  return pimpl_->SetHeaderContentType(content_type);
}

///////////////////////////////////////////////////////////////////////////////
const std::string CGIBase::GetHeaderContentType() const {
  assert(pimpl_.get());
  return pimpl_->GetHeaderContentType();
}

///////////////////////////////////////////////////////////////////////////////
void CGIBase::SetHeaderStatus(const Int &status) const {
  assert(pimpl_.get());
  return pimpl_->SetHeaderStatus(status);
}

///////////////////////////////////////////////////////////////////////////////
const Int CGIBase::GetHeaderStatus() const {
  assert(pimpl_.get());
  return pimpl_->GetHeaderStatus();
}

///////////////////////////////////////////////////////////////////////////////
void CGIBase::SetHeaderCharset(const std::string &charset) const {
  assert(pimpl_.get());
  return pimpl_->SetHeaderCharset(charset);
}

///////////////////////////////////////////////////////////////////////////////
const std::string CGIBase::GetHeaderCharset() const {
  assert(pimpl_.get());
  return pimpl_->GetHeaderCharset();
}

}   // namespace cgi
}   // namespace udp
