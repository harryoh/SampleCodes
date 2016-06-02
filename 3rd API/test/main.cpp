///////////////////////////////////////////////////////////////////////////////
/// @internal
/// @file       test/main.cpp
/// @~english
/// @brief      C++ unit test file
/// @~
/// @author     Matt Clarkson (matt.clarkson@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

/* cpplint Supression
 * It asks to add a folder before the include file, but in this case, we're
 * just doing a simple build, so we can ignore it
 */
#include "main.h"  // NOLINT(build/include)

WARNING_PUSH
DISABLE_EXTERNAL_HEADER_WARNINGS
#include <string>
#include <cstring>
/* cpplint Supression
 * cpplint HATES streams for readability but we need a stream for cppunit to
 * output XML
 */
#include <fstream>  // NOLINT(readability/streams)
#include "cppunit/TestRunner.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestResultCollector.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/BriefTestProgressListener.h"
#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
WARNING_POP

#include "junit_outputter.hpp"

/**
 * @~english
 * The binary entry point
 */
int main(int argc, char *argv[]) {
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // Add a listener that collects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    // Outputs tests to command line so that waf can parse the output
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    // Decide what the file name should be
    const char * name = argv[0];

    // Prepare for the XML output
    std::ofstream xml_file;
    const char * const extension = "_cppunit.xml";
    size_t size = std::strlen(name);
    size += std::strlen("_Zunit");
    size += std::strlen(extension);
    size += 1;  // For null character
    char * const filename = new char[size];

    // Output xUnit XML file
    #ifdef _MSC_VER
        _snprintf_s(filename, size, size, "%s_xunit%s", argv[0], extension);
    #else
        std::snprintf(filename, size, "%s_xunit%s", argv[0], extension);
    #endif
    filename[size - 1] = '\0';
    xml_file.open(filename);
    CPPUNIT_NS::XmlOutputter xunit_outputter(&result, xml_file);
    xunit_outputter.write();
    xml_file.close();

    // Output jUnit XML file
    #ifdef _MSC_VER
        _snprintf_s(filename, size, size, "%s_junit%s", argv[0], extension);
    #else
        std::snprintf(filename, size, "%s_junit%s", argv[0], extension);
    #endif
    filename[size - 1] = '\0';
    xml_file.open(filename);
    JunitOutputter junit_outputter;
    junit_outputter.Write(result, &xml_file);
    xml_file.close();

    // Clean up
    delete [] filename;

    // Do something with argc so there are no unused parameter warnings
    return (result.wasSuccessful() && argc) ? 0 : 1;
}
