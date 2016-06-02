///////////////////////////////////////////////////////////////////////////////
/// @file       cgi/vendor/tyco/unit_test.cpp
/// @~english
/// @brief      unit test
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>

#include "cppunit/extensions/HelperMacros.h"
#include "tycocgi.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

///////////////////////////////////////////////////////////////////////////////
/**
 * @~english
 * A unit test class for the udp::cgi::TycoCGITest class
 */
class TycoCGITest : public CPPUNIT_NS::TestCase {
  /// @cond
  CPPUNIT_TEST_SUITE(TycoCGITest);
  CPPUNIT_TEST(InitCheck);
  CPPUNIT_TEST(BasicCheck);
  CPPUNIT_TEST(ExecptionCheck);
//  CPPUNIT_TEST(BoundaryCheck);
  CPPUNIT_TEST_SUITE_END();
  /// @endcond

public:  /* tests */
  /**
   * @test Init Cgibase testing
   */
  void InitCheck() {
    TycoCGI cgi("action=read&group=A&group=B&group=C");
    CPPUNIT_ASSERT(cgi.GetValue("action") == "read");

    int i = 0;
    std::string group[] = {"A", "B", "C"};

    CGIBase::EntryList entrylist = cgi.GetValues("group");
    CGIBase::EntryList::iterator iter;
    for (iter = entrylist.begin(); iter != entrylist.end(); ++iter) {
      CPPUNIT_ASSERT(iter->second == group[i++]);
    }
  }

  /**
   * Check Basic features
   */
  void BasicCheck() {
    const CGIBase *cgi = new TycoCGI("action=list&group=A&group=B&group=C");

    CPPUNIT_ASSERT(cgi->GetValue("action") == "list");
    CPPUNIT_ASSERT(cgi->GetValue("group") == "A");

    int i = 0;
    std::string group[] = {"A", "B", "C"};

    CGIBase::EntryList entrylist = cgi->GetValues("group");
    CGIBase::EntryList::iterator iter;
    for (iter = entrylist.begin(); iter != entrylist.end(); ++iter) {
      CPPUNIT_ASSERT(iter->second == group[i++]);
    }

    CPPUNIT_ASSERT(cgi->SearchEntry("action") == TRUE);
    CPPUNIT_ASSERT(cgi->SearchEntry("group") == TRUE);
    CPPUNIT_ASSERT(cgi->SearchEntry("options") == FALSE);

    // Check Header
    CPPUNIT_ASSERT(cgi->GetHeaderContentType() == "text/plain");
    CPPUNIT_ASSERT(cgi->GetHeaderCharset() == "utf-8");
    CPPUNIT_ASSERT(cgi->GetHeaderStatus() == 200);

    cgi->SetHeaderContentType("text/html");
    cgi->SetHeaderCharset("euc-kr");
    cgi->SetHeaderStatus(500);

    CPPUNIT_ASSERT(cgi->GetHeaderContentType() == "text/html");
    CPPUNIT_ASSERT(cgi->GetHeaderCharset() == "euc-kr");
    CPPUNIT_ASSERT(cgi->GetHeaderStatus() == 500);

    delete cgi;
  }

  /**
   * Check Exceptions
   */
  void ExecptionCheck() {
    TycoCGI cgi("action=read&group=A&group=B&group=C");
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "querystring is invalid",
        cgi.CheckParameter());
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
CPPUNIT_TEST_SUITE_REGISTRATION(udp::cgi::TycoCGITest);
/// @endcond
