///////////////////////////////////////////////////////////////////////////////
/// @internal
/// @file       test/junit_outputter.cpp
/// @~english
/// @brief      The implementation of the JunitOutputter API
/// @~
/// @author     Matt Clarkson(matt.clarkson@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include "junit_outputter.hpp"

WARNING_PUSH
DISABLE_EXTERNAL_HEADER_WARNINGS
#include <cppunit/TestResultCollector.h>
#include <cppunit/Portability.h>
#include <cppunit/Outputter.h>
#include <cppunit/tools/XmlElement.h>
#include <cppunit/tools/XmlDocument.h>
#include <cppunit/portability/CppUnitDeque.h>
#include <cppunit/portability/CppUnitMap.h>
#include <cppunit/portability/Stream.h>
#include <cppunit/Exception.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cassert>
#include <vector>
#include <ctime>
#include <utility>
#include <string>
#include <functional>
#include <stdlib.h>  // NOLINT(build/include_order)
WARNING_POP

///////////////////////////////////////////////////////////////////////////////
/**
 * @~english
 * Outputs cppunit results in JUnit XML format
 */
class JunitOutputter::JunitOutputterImpl {
/// @cond
private:    /* Non-copyable */
  JunitOutputterImpl(const JunitOutputterImpl &);
  JunitOutputterImpl &operator =(const JunitOutputterImpl &);
/// @endcond

public:     /* Constructors */
  /**
   * @copydoc JunitOutputter::JunitOutputter
   */
  JunitOutputterImpl() {}

public:     /* Destructor */
  /**
   * @copydoc JunitOutputter::~JunitOutputter
   */
  ~JunitOutputterImpl() {}

public:     /* Public API */
  /**
   * @copydoc JunitOutputter::Write
   */
  void Write(const CPPUNIT_NS::TestResultCollector &results,
             std::ostream *const stream,
             const std::string &encoding) const {
    CPPUNIT_NS::XmlDocument xml_doc(encoding);
    SetRootNode(&xml_doc, results);
    *stream << xml_doc.toString();
  }

private:    /* Private typedefs */
  /**
   * @~english
   * A map of tests to failed tests
   */
  typedef CppUnitMap < const CPPUNIT_NS::Test *const,
          const CPPUNIT_NS::TestFailure *const,
          std::less<const CPPUNIT_NS::Test *const > > FailedTests;

  /**
   * @~english
   * A map of tests to XML elements corresponding to the test
   */
  typedef CppUnitMap < const CPPUNIT_NS::Test *const,
          CPPUNIT_NS::XmlElement *const,
          std::less<const CPPUNIT_NS::Test *const > > Elements;

  /**
   * @~english
   * A split test name
   * @see SplitTestName
   */
  typedef std::vector<std::string> TestNameSplit;

private:    /* Private API */
  /**
   * @~english
   * Sets the root node for an XML document
   * @param xml_doc the XML document
   * @param results the results of the cppunit run
   */
  void SetRootNode(CPPUNIT_NS::XmlDocument *const xml_doc,
                   const CPPUNIT_NS::TestResultCollector &results) const {
    using CPPUNIT_NS::XmlElement;

    XmlElement *const root_node = new XmlElement("testsuites");
    xml_doc->setRootElement(root_node);

    FailedTests failed_tests;
    FillFailedTestsMap(&failed_tests, results);

    Elements elements;
    FillElementsMap(&failed_tests, &elements, root_node, results);

    AddTests(&failed_tests, &elements, results);
  }

  /**
   * @~english
   * Parses a cppunit namespace.  A Cppunit namespace looks like this:
   *
   * @c N3udp2ks7logging10LoggerTestE
   *
   * Where:
   * - @c N is the start of the namespace
   * - digits represent the following characters in the namespace
   * - @c E is the end of the namespace
   * @param ns the cppunit namespace
   * @returns a naturally typed C++ namespace
   */
  std::string ParseCppunitNamespace(const std::string &ns) const {
    /* This is a really rubbish, inefficient parser,
     * feel free to improve :)
     */
    if (ns.at(0) != 'N' && ns.at(ns.length() - 1) != 'E') {
      return ns;
    }
    std::string parsed;
    unsigned int index = 1;
    while (index < ns.length() - 1) {
      int characters = atoi(&ns[index]);
      while ((ns[index] >= '0') && (ns[index] <= '9')) {
        ++index;
      }
      parsed += std::string(ns, index, static_cast<unsigned int>(characters)) +
                std::string("::");
      index += characters;
    }

    return std::string(parsed, 0, parsed.length() - 2);
  }

  /**
   * @~english
   * Splits a cppunit test name for JUnit XML
   * @param test the test
   * @returns a split test name
   */
  TestNameSplit SplitTestName(const CPPUNIT_NS::Test &test) const {
    /* This is a really rubbish, inefficient tokeniser,
     * feel free to improve :)
     */
    TestNameSplit test_name_split(2);
    std::string name = test.getName();
    std::string::size_type n = name.rfind("::");
    if (n != std::string::npos) {
      test_name_split.at(0) = ParseCppunitNamespace(std::string(name, 0, n));
      test_name_split.at(1) = std::string(name, n + 2);
    } else {
      test_name_split.at(0) = std::string("anonymous");
      test_name_split.at(1) = test.getName();
    }
    return test_name_split;
  }

  /**
   * @~english
   * Gets the test execution time of the test in second.
   * @param test the test
   * @returns execution time of the test in seconds
   */
  double GetTestTime(const CPPUNIT_NS::Test &test) const {
    static_cast<void>(test);
    return 0.0;
  }

  /**
   * @~english
   * Fills the XML elements map so that cppunit tests correspond to an XML
   * element
   * @param failed_tests a map of failed tests
   * @param elements a map of XML elements
   * @param root_node the XML document root node
   * @param results the results of the cppunit run
   */
  void FillElementsMap(FailedTests *const failed_tests,
                       Elements *const elements,
                       CPPUNIT_NS::XmlElement *const root_node,
                       const CPPUNIT_NS::TestResultCollector &results)
  const {
    typedef CPPUNIT_NS::TestResultCollector::Tests Tests;
    using CPPUNIT_NS::XmlElement;
    using CPPUNIT_NS::Test;
    typedef CppUnitMap<std::string, XmlElement *> TestSuiteNames;
    enum CountIndexes {
      kTests,
      kErrors,
      kFailures,
      kCountTotal
    };
    typedef std::vector<int> Counts;
    typedef CppUnitMap<XmlElement *, Counts> TestSuiteCounts;
    typedef CppUnitMap<XmlElement *, double> TestSuiteTimes;
    TestSuiteNames test_suite_names;
    TestSuiteTimes testSuiteTimes;
    TestSuiteCounts testSuiteCounts;
    const Tests &tests = results.tests();
    XmlElement *element = NULL;
    TestSuiteNames::const_iterator test_suite_names_iter;
    for (unsigned int number = 0; number < tests.size(); ++number) {
      const Test *const test = tests[number];
      assert(test);
      TestNameSplit test_name_split = SplitTestName(*test);
      if (!test_name_split.empty()) {
        test_suite_names_iter = test_suite_names.find(test_name_split.at(0));
        if (test_suite_names_iter == test_suite_names.end()) {
          element = new XmlElement("testsuite");
          assert(element);
          element->addAttribute("name", test_name_split.at(0));
          element->addAttribute("hostname", "localhost");
          XmlElement *sys_err_element = new XmlElement("system-err");
          element->addElement(sys_err_element);
          XmlElement *sys_out_element = new XmlElement("system-out");
          element->addElement(sys_out_element);
          char buf[128];
          time_t mytimet;
          time(&mytimet);
          struct tm tmtime;
          struct tm *mytime = &tmtime;
#ifdef _MSC_VER
          localtime_s(mytime, &mytimet);
#else
          localtime_r(&mytimet, mytime);
#endif
          strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", mytime);
          element->addAttribute("timestamp", buf);
          root_node->addElement(element);
          test_suite_names.insert(std::pair<std::string, XmlElement *>(
                                    test_name_split[0], element));
          Counts counts(kCountTotal, 0);
          testSuiteCounts.insert(std::make_pair(element, counts));
          testSuiteTimes.insert(std::make_pair(element, 0.0));
        } else {
          element = test_suite_names_iter->second;
        }
        elements->insert(std::pair<const Test *const, XmlElement *const>(
                           test, element));
        testSuiteCounts[element][kTests]++;
        if (failed_tests->find(test) != failed_tests->end()) {
          if (((*failed_tests)[test])->isError()) {
            testSuiteCounts[element][kErrors]++;
          } else {
            testSuiteCounts[element][kFailures]++;
          }
        }
        testSuiteTimes[element] += GetTestTime(*test);
      }
    }
    test_suite_names_iter = test_suite_names.begin();
    while (test_suite_names_iter != test_suite_names.end()) {
      element = test_suite_names_iter->second;
      element->addAttribute("tests", testSuiteCounts[element][kTests]);
      element->addAttribute("failures", testSuiteCounts[element][kFailures]);
      element->addAttribute("errors", testSuiteCounts[element][kErrors]);
      std::stringstream test_time;
      test_time.precision(9);
      test_time << std::fixed << testSuiteTimes[element];
      element->addAttribute("time", test_time.str());
      ++test_suite_names_iter;
    }
  }

  /**
   * @~english
   * Fills the failed tests map
   * @param failed_tests a map of failed tests
   * @param results the results of the cppunit run
   */
  void FillFailedTestsMap(FailedTests *const failed_tests,
                          const CPPUNIT_NS::TestResultCollector &results)
        const {
    typedef CPPUNIT_NS::TestResultCollector::TestFailures TestFailures;
    using CPPUNIT_NS::TestFailure;
    using CPPUNIT_NS::Test;

    const TestFailures &failures = results.failures();
    TestFailures::const_iterator itFailure = failures.begin();
    while (itFailure != failures.end()) {
      TestFailure *const failure = *itFailure++;
      failed_tests->insert(std::pair < Test * const, TestFailure * const >
                           (failure->failedTest(), failure));
    }
  }

  /**
   * @~english
   * Adds all tests to the XML document
   * @param failed_tests a map of failed tests
   * @param elements a map of XML elements
   * @param results the results of the cppunit run
   */
  void AddTests(FailedTests *const failed_tests,
                Elements *const elements,
                const CPPUNIT_NS::TestResultCollector &results) const {
    typedef CPPUNIT_NS::TestResultCollector::Tests Tests;
    using CPPUNIT_NS::TestFailure;
    using CPPUNIT_NS::Test;
    assert(failed_tests);
    assert(elements);

    const Tests &tests = results.tests();
    for (unsigned int test_number = 0; test_number < tests.size();
         ++test_number) {
      const Test *const test = tests[test_number];
      assert(test);
      if (elements->find(test) != elements->end()) {
        if (failed_tests->find(test) != failed_tests->end()) {
          AddFailedTest(*(*failed_tests)[test], (*elements)[test]);
        } else {
          AddSuccessfulTest(*test, (*elements)[test]);
        }
      }
    }
  }


  /**
   * @~english
   * Adds a successful test to the XML document
   * @param test the test to add
   * @param element the XML element to add it too
   */
  void AddSuccessfulTest(const CPPUNIT_NS::Test &test,
                         CPPUNIT_NS::XmlElement *const element)
  const {
    using CPPUNIT_NS::XmlElement;
    assert(element);
    const TestNameSplit test_name_split = SplitTestName(test);
    XmlElement *const test_case_element = new XmlElement("testcase");
    assert(test_case_element);
    test_case_element->addAttribute("class", test_name_split.at(0));
    test_case_element->addAttribute("name", test_name_split.at(1));
    std::stringstream test_time;
    test_time.precision(9);
    test_time << std::fixed << GetTestTime(test);
    test_case_element->addAttribute("time", test_time.str());
    element->addElement(test_case_element);
  }

  /**
   * @~english
   * Adds a failed test to the XML document
   * @param failure the failed test to add
   * @param element the XML element to add it too
   */
  void AddFailedTest(const CPPUNIT_NS::TestFailure &failure,
                     CPPUNIT_NS::XmlElement *const element) const {
    using CPPUNIT_NS::XmlElement;
    using CPPUNIT_NS::Test;
    using CPPUNIT_NS::Message;
    using CPPUNIT_NS::SourceLine;
    assert(element);
    const Test *const test = failure.failedTest();
    assert(test);
    const TestNameSplit test_name_split = SplitTestName(*test);
    XmlElement *const test_case_element = new XmlElement("testcase");
    test_case_element->addAttribute("class", test_name_split.at(0));
    test_case_element->addAttribute("name", test_name_split.at(1));
    std::stringstream test_time;
    test_time.precision(9);
    test_time << std::fixed << GetTestTime(*test);
    test_case_element->addAttribute("time", test_time.str());
    element->addElement(test_case_element);
    const Message message = failure.thrownException()->message();
    const SourceLine sourceLine = failure.sourceLine();
    XmlElement *const fail_element = new XmlElement(
      failure.isError() ? "error" : "failure");
    assert(fail_element);
    fail_element->addAttribute("message", message.shortDescription());
    fail_element->addAttribute("type", "CPPUNIT_NS::TestFailure");
    std::stringstream content;
    for (int detail = 0; detail < message.detailCount(); ++detail) {
      content << message.detailAt(detail) << std::endl;
    }
    content << "File Name: " << sourceLine.fileName() << std::endl;
    content << "Line Number: " << sourceLine.lineNumber() << std::endl;
    fail_element->setContent(content.str());
    test_case_element->addElement(fail_element);
  }
};

///////////////////////////////////////////////////////////////////////////////
JunitOutputter::JunitOutputter()
  : pimpl_(new JunitOutputterImpl()) {}

///////////////////////////////////////////////////////////////////////////////
JunitOutputter::~JunitOutputter() {}

///////////////////////////////////////////////////////////////////////////////
void JunitOutputter::Write(
  const CPPUNIT_NS::TestResultCollector &results,
  std::ostream *const stream,
  const std::string &encoding) const {
  assert(pimpl_.get());
  pimpl_->Write(results, stream, encoding);
}

