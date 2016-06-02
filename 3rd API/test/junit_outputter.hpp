#ifndef CXX_INC_JUNIT_OUTPUTTER_H_
#define CXX_INC_JUNIT_OUTPUTTER_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       /test/junit_outputter.hpp
/// @~english
/// @brief      An extension of cppunit to convert results to JUnit XML.
/// @~
/// @copyright  UDP Technology
/// @author     Matt Clarkson (matt.clarkson@udptechnology.com)
///////////////////////////////////////////////////////////////////////////////

#include "macros.h"  // NOLINT

WARNING_PUSH
DISABLE_EXTERNAL_HEADER_WARNINGS
#include <ostream>  // NOLINT
#include <memory>  // NOLINT
#include <string>  // NOLINT
#include <cppunit/TestResultCollector.h>  // NOLINT
WARNING_POP

/**
 * @~english
 * Outputs cppunit results in JUnit XML format
 */
class DLL_LOCAL JunitOutputter {
/// @cond
private:    /* Non-copyable */
  JunitOutputter(const JunitOutputter &);
  JunitOutputter &operator = (const JunitOutputter &);
/// @endcond

public:     /* Constructors */
  /**
   * @~english
   * Constructs a JunitOutputter object.
   */
  JunitOutputter();

public:     /* Destructor */
  /**
   * @~english
   * Destructor.
   */
  virtual ~JunitOutputter();

public:     /* Public API */
  /**
   * @~english
   * Writes the specified result as an XML document to the stream.
   * @param results results of the test run
   * @param stream the stream to output too
   * @param encoding the encoding of the XML document
   */
  void Write(const CPPUNIT_NS::TestResultCollector &results,
             std::ostream *const stream,
             const std::string &encoding = std::string("UTF-8")) const;

private:    /* Private Implementation */
  class JunitOutputterImpl;  // forward declaration

  /**
   * @~english
   * The private implementation
   */
  const std::auto_ptr<JunitOutputterImpl>  pimpl_;
};

#endif  // CXX_INC_JUNIT_OUTPUTTER_H_
