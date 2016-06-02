#ifndef CGIBASE_H_
#define CGIBASE_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       cgibase.hpp
/// @brief      Base class for cgi
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <map>
#include <memory>

#include "udp/keystone/system/types.h"

namespace udp {
namespace cgi {

/**
 * Common feature class.
 * This class parse query string, print header and print output
 */
class CGIBase {
public:   /* Public type definition */
  /**
   * The enum that is method list
   */
  enum Method {
    /**
     * Get the method post/get but not cookie.
     */
    kMethodAll,
    /**
     * The get method
     */
    kMethodGet,
    /**
     * The post method
     */
    kMethodPost,
    /**
     * The cookie method
     */
    kMethodCookie
  };

  /**
   * This map table use to store all of parameters.
   * @remarks There can be same entry.
   */
  typedef std::multimap<std::string, std::string> EntryList;

public:   /* Constructors */
  /**
   * Default constructor
   */
  CGIBase();

  /**
   * Construct a new CGIBase using Method
   * @param method The Method to get from cgi.
   * @see enum Method
   */
  explicit CGIBase(const Method &method);

  /**
   * Construct a new CGIBase using Environment\n
   * If you want to see Environment, see
   * &operator<<(std::ostream &stream, CGIBase &cgi)
   * @param env Environment Table
   */
  explicit CGIBase(const std::map<std::string, std::string> &env);

  /**
   * Construct a new CGIBase using arguments
   * @param querystring query string requested.
   * @remarks when this construct is called, the method is only GET
   */
  explicit CGIBase(const std::string &querystring);

  /**
   * Copy constructor.
   * @param other The CGIBase class to copy from
   */
  CGIBase(const CGIBase &other);

  /**
   * Copy assignment.
   * @param rhs The CGIBase class to copy from
   * @return The copied CGIBase class for chaining
   */
  CGIBase &operator=(const CGIBase &rhs);

  /**
   * Default destructor
   */
  virtual ~CGIBase();

public:   /* public member functions */
  /**
   * Return Get a values of the name entry.
   * @param name Then entry name to get
   * @return A entry value of the name
   *
   * @code
   *    [example.cgi?action=list]
   *
   *    CGIBase cgi;
   *    std::cout << "action=" << cgi.GetValue("action") << std::endl;
   *
   *    // output
   *    // action=list
   *
   *    [example.cgi?action=list&group=A&group=B&group=C]
   *
   *    CGIBase cgi;
   *    std::cout << "group=" << cgi.GetValue("group") << std::endl;
   *
   *    // output
   *    // group=A
   * @endcode
   */
  const std::string GetValue(const std::string &name) const;

  /**
   * Return all values of method(post/get).
   * @return CGIBase::EntryList
   *
   * @code
   *    [example.cgi?action=list&group=AA&group=BB&group=CC]
   *
   *    CGIBase cgi;
   *    CGIBase::EntryList list = cgi.GetValues();
   *    CGIBase::EntryList::iterator iter;
   *    for (iter = list.begin(); iter != list.end(); ++iter) {
   *      std::cout << (*iter).first << "=" << (*iter).second << std::endl;
   *    }
   *
   *    // output
   *    // action=list
   *    // group=AA
   *    // group=BB
   *    // group=CC
   * @endcode
   */
  EntryList GetValues() const;

  /**
   * Return a entry values of method.
   * @param name The entry name to get
   * @return CGIBase::EntryList
   * 
   * @code
   *    [example.cgi?action=list&group=AA&group=BB&group=CC]
   *
   *    CGIBase cgi;
   *    CGIBase::EntryList list = cgi.GetValues("group");
   *    CGIBase::EntryList::iterator iter;
   *    for (iter = list.begin(); iter != list.end(); ++iter) {
   *      std::cout << (*iter).first << "=" << (*iter).second << std::endl;
   *    }
   *
   *    // output
   *    // group=AA
   *    // group=BB
   *    // group=CC
   * @endcode
   */
  const EntryList GetValues(const std::string &name) const;

  /**
   * Test if there is a entry of name
   * @param name a entry name to search
   * @return TRUE/FALSE
   */
  const Bool SearchEntry(const std::string &name) const;

  /**
   * Set up Content-Type of HTTP Header.
   * @param content_type Mime-Type
   */
  void SetHeaderContentType(const std::string &content_type) const;

  /**
   * Get Content-Type of HTTP Header.
   * @return Content-Type
   */
  const std::string GetHeaderContentType() const;

  /**
   * Set up status of HTTP Header.
   * @param status Status code
   */
  void SetHeaderStatus(const Int &status) const;

  /**
   * Get status of HTTP Header.
   * @return Status code
   */
  const Int GetHeaderStatus() const;

  /**
   * Set up charset of HTTP Header.
   * @param charset charset(utf8, euc-kr...)
   */
  void SetHeaderCharset(const std::string &charset) const;

  /**
   * Get charset of HTTP Header.
   * @return charset(utf8, euc-kr...)
   */
  const std::string GetHeaderCharset() const;

  /**
   * Print Header
   */
  void PrintHeader() const;

  /**
   * Print output
   * @param str Output results as string
   */
  void Print(const std::string &str) const;

  /**
   * Return Get Environment.
   * @param name Then entry name to get
   * @return A environment value of the name
   *
   * [Environment List]
   * @li DOCUMENT_ROOT  The root directory of your server
   * @li HTTP_COOKIE The visitor's cookie, if one is set
   * @li HTTP_HOST The hostname of the page being attempted
   * @li HTTP_REFERER  The URL of the page that called your program
   * @li HTTP_USER_AGENT The browser type of the visitor
   * @li HTTPS "on" if the program is being called through a secure server
   * @li PATH  The system path your server is running under
   * @li QUERY_STRING  The query string (see GET, below)
   * @li REMOTE_ADDR The IP address of the visitor
   * @li REMOTE_HOST The hostname of the visitor (if your server has
   * reverse-name-lookups on; otherwise this is the IP address again)
   * @li REMOTE_PORT The port the visitor is connected to on the web server
   * @li REMOTE_USER The visitor's username (for .htaccess-protected pages)
   * @li REQUEST_METHOD  GET or POST
   * @li REQUEST_URI The interpreted pathname of the requested document or CGI
   * (relative to the document root)
   * @li SCRIPT_FILENAME The full pathname of the current CGI
   * @li SCRIPT_NAME The interpreted pathname of the current CGI (relative to
   * the document root)
   * @li SERVER_ADMIN  The email address for your server's webmaster
   * @li SERVER_NAME Your server's fully qualified domain name
   * (e.g. www.cgi101.com)
   * @li SERVER_PORT The port number your server is listening on
   * @li SERVER_SOFTWARE The server software you're using (e.g. Apache 1.3)
   *
   * @code
   *    CGIBase cgi;
   *    std::cout << "QUERY_STRING:" << cgi.GetEnv("QUERY_STRING")\
   *              << std::endl;
   *
   *    // output
   *    // QUERY_STRING:action=list&group=A&group=B
   * @endcode
   */
  const std::string GetEnv(const std::string &name) const;

  /**
   * Check validation parameters
   */
  virtual void CheckParameter() const {}

  /**
   * CGI implementation
   */
  virtual void Run() {}

public: /* Stream Operators */
  /**
   * Outputs the internal state of a parameter to a stream in json style as
   * <tt>{'action'='list', 'group'='A.B.C', ....}</tt>
   * @param stream The stream to output too.
   * @param cgi The cgi class to output to the stream
   * @returns Output stream
   */
  friend std::ostream &operator<<(std::ostream &stream, CGIBase &cgi);

private:   /* Private method */
  /**
   * CGIBase implementation class
   */
  class CGIBaseImpl;

  /**
   * CGIBase implementation variable
   */
  const std::auto_ptr<CGIBaseImpl> pimpl_;
};  // class CGIBase

}   // namespace cgi
}   // namespace udp

#endif  // CGIBASE_H_
