///////////////////////////////////////////////////////////////////////////////
/// @file       lib/cgibase/unit_test.cpp
/// @~english
/// @brief      unit test
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>

#include <stdlib.h>    // NOLINT(build/include_order)

#include "cppunit/extensions/HelperMacros.h"

#include "cgibase.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

///////////////////////////////////////////////////////////////////////////////
/**
 * @~english
 * A unit test class for the udp::cgi::CGIBaseTest class
 */
class CGIBaseTest : public CPPUNIT_NS::TestCase {
  /// @cond
  CPPUNIT_TEST_SUITE(CGIBaseTest);
  CPPUNIT_TEST(InitCheck);
  CPPUNIT_TEST(BasicCheck);
//  CPPUNIT_TEST(ExecptionCheck);
//  CPPUNIT_TEST(BoundaryCheck);
  CPPUNIT_TEST_SUITE_END();
  /// @endcond

public:  /* tests */
  /**
   * @test Init Cgibase testing
   */
  void InitCheck() {
    std::string querystring = "action=read&group=A";
    std::string uri(static_cast<std::string>(__FILE__));
    uri += "?";
    uri += querystring;

    setenv("QUERY_STRING", querystring.c_str(), true);
    setenv("REQUEST_METHOD", "GET", true);
    setenv("REQUEST_URI", uri.c_str(), true);
    CGIBase cgiall(CGIBase::kMethodAll);

    CPPUNIT_ASSERT(cgiall.GetValue("action") == "read");
    CPPUNIT_ASSERT(cgiall.GetValue("group") == "A");

    CGIBase cgipost(CGIBase::kMethodPost);
    CPPUNIT_ASSERT(cgipost.GetValue("action").empty());
    CPPUNIT_ASSERT(cgipost.GetValue("group").empty());

    CGIBase cgi(querystring);
    CPPUNIT_ASSERT(cgi.GetValue("action") == "read");
    CPPUNIT_ASSERT(cgi.GetValue("group") == "A");

//    CGIBase copycgi = cgi;
//    CPPUNIT_ASSERT(copycgi.GetValue("action") == cgi.GetValue("action"));
//    CPPUNIT_ASSERT(copycgi.GetValue("group") == cgi.GetValue("group"));
  }

  /**
   * Check Basic feature
   */
  void BasicCheck() {
    CGIBase cgi("action=list&group=A&group=B&group=C");

    CPPUNIT_ASSERT(cgi.GetValue("action") == "list");
    CPPUNIT_ASSERT(cgi.GetValue("group") == "A");

    int i = 0;
    std::string group[] = {"A", "B", "C"};

    CGIBase::EntryList entrylist = cgi.GetValues("group");
    CGIBase::EntryList::iterator iter;
    for (iter = entrylist.begin(); iter != entrylist.end(); ++iter) {
      CPPUNIT_ASSERT(iter->second == group[i++]);
    }

    CPPUNIT_ASSERT(cgi.SearchEntry("action") == TRUE);
    CPPUNIT_ASSERT(cgi.SearchEntry("group") == TRUE);
    CPPUNIT_ASSERT(cgi.SearchEntry("options") == FALSE);

    // Check Header
    CPPUNIT_ASSERT(cgi.GetHeaderContentType() == "text/plain");
    CPPUNIT_ASSERT(cgi.GetHeaderCharset() == "utf-8");
    CPPUNIT_ASSERT(cgi.GetHeaderStatus() == 200);

    cgi.SetHeaderContentType("text/html");
    cgi.SetHeaderCharset("euc-kr");
    cgi.SetHeaderStatus(500);

    CPPUNIT_ASSERT(cgi.GetHeaderContentType() == "text/html");
    CPPUNIT_ASSERT(cgi.GetHeaderCharset() == "euc-kr");
    CPPUNIT_ASSERT(cgi.GetHeaderStatus() == 500);
  }

  /**
   * Check Exception
   */
  void ExecptionCheck() {
  }

  /**
   * Check Boundary
   */
  void BoundaryCheck() {
  }
};

///////////////////////////////////////////////////////////////////////////////
}  // namespace cgi
}  // namespace udp

///////////////////////////////////////////////////////////////////////////////
/// @cond
CPPUNIT_TEST_SUITE_REGISTRATION(udp::cgi::CGIBaseTest);
/// @endcond
