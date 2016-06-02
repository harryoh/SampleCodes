///////////////////////////////////////////////////////////////////////////////
/// @file       cgi/function/hello/unit_test.cpp
/// @~english
/// @brief      unit test
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <stdexcept>

#include "cppunit/extensions/HelperMacros.h"

#include "tycoerror.hpp"
#include "hello.hpp"

///////////////////////////////////////////////////////////////////////////////
/**
 * @~english
 * A unit test class for the HelloTest class
 */
class HelloTest : public CPPUNIT_NS::TestCase {
  /// @cond
  CPPUNIT_TEST_SUITE(HelloTest);
  CPPUNIT_TEST(InitCheck);
  CPPUNIT_TEST(BasicCheck);
  CPPUNIT_TEST(ExecptionCheck);
//  CPPUNIT_TEST(BoundaryCheck);
  CPPUNIT_TEST_SUITE_END();
  /// @endcond

public:  /* tests */
  /**
   * @test Init Hello testing
   */
  void InitCheck() {
    using udp::cgi::CGIBase;

    std::string querystring =
        "action=read&group=A&group=B&group=C";

    Hello hellocgi("/iAPI/hello.cgi?" + querystring);
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "failed to create new class",
        hellocgi.Prepare());

    TycoHello tycocgi(querystring);
    int i = 0;
    std::string group[] = {"A", "B", "C"};

    CGIBase::EntryList entrylist = tycocgi.GetValues("group");
    CGIBase::EntryList::iterator iter;
    for (iter = entrylist.begin(); iter != entrylist.end(); ++iter) {
      CPPUNIT_ASSERT(iter->second == group[i++]);
    }
  }

  /**
   * Check Basic features
   */
  void BasicCheck() {
    Hello hellocgi("/iAPI/hello.cgi?action=read&group=A&group=B&group=C");
    hellocgi.Prepare();
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Hello class run() failed(action=list)",
        hellocgi.Run());

    Hello hellocgiA("/iAPI/hello.cgi?action=update&group=A&group=B&group=C");
    hellocgiA.Prepare();
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Hello class run() failed(action=update)",
        hellocgiA.Run());

    Hello hellocgiB("/iAPI/hello.cgi?action=explain&group=A&group=B&group=C");
    hellocgiB.Prepare();
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "Hello class run() failed(action=explain)",
        hellocgiB.Run());

    Hello helloA("/iAPI/hello.cgi?action=list&group=A");
    helloA.Prepare();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "when action has wrong value, it has to happen error.",
        helloA.Run(),
        udp::cgi::TycoError);

    Hello helloB("/iAPI/hello.cgi?group=A");
    helloB.Prepare();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "action has to be exist",
        helloB.Run(),
        udp::cgi::TycoError);

    TycoHello tycocgi("action=read&group=A&group=B&group=C");
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "TycoHello class CheckParameter() failed",
        tycocgi.CheckParameter());
    CPPUNIT_ASSERT_NO_THROW_MESSAGE(
        "TycoHello class run() failed",
        tycocgi.Run());
  }

  /**
   * Check Exception
   */
  void ExecptionCheck() {
    Hello helloA("/xxx/hello.cgi?action=list&group=A&group=B&group=C");
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "can't find implement cgi class",
        helloA.Prepare(),
        std::invalid_argument);

    Hello helloB("");
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "when no parameter, it has to happen exception",
        helloB.Prepare(),
        std::invalid_argument);
  }

  /**
   * Check Boundary
   */
  void BoundaryCheck() {
  }
};

///////////////////////////////////////////////////////////////////////////////
/// @cond
CPPUNIT_TEST_SUITE_REGISTRATION(HelloTest);
/// @endcond
