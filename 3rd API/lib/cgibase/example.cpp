///////////////////////////////////////////////////////////////////////////////
/// @file       lib/cgibase/example.cpp
/// @~english
/// @brief      test code
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <stdlib.h>  // NOLINT(build/include_order)

#include "udp/keystone/system/types.h"

#include "cgibase.hpp"

/**
 *  Example main function
 * @param argc
 * @param argv
 * @return a integer status code
 */
int main(int argc, char** argv) {
  udp::cgi::CGIBase::EntryList entrylist, grouplist;

  // for debug
  // udp::cgi::CGIBase cgi = udp::cgi::CGIBase::FromArg(argv);
  const std::string querystring = argv[1] ? argv[1]:"";
  udp::cgi::CGIBase cgi(querystring);

  cgi.PrintHeader();

  // std::cout << "Content-type: text/plain" << "\r\n\r\n";
  std::cout << "Test me!!!" << std::endl;
  std::cout << "ex)" << std::endl;
  std::cout << " - ./example.cgi action=list&group=1&group=2&group=3"
            << std::endl;
  std::cout << " - http://<ipaddr>/iAPI/example.cgi?action=list&group=1"
            << "&group=2&group=3" << std::endl << std::endl;

  std::cout << "************************************************" << std::endl;
  std::cout << "*                   Start Test                 *" << std::endl;
  std::cout << "************************************************" << std::endl;

  std::cout << "----- Environment -----" << std::endl;
  std::cout << "QUERY_STRING: " << cgi.GetEnv("QUERY_STRING") << std::endl;
  std::cout << "REQUEST_METHOD: " << cgi.GetEnv("REQUEST_METHOD")
            << std::endl;
  std::cout << "REQUEST_URI: " << cgi.GetEnv("REQUEST_URI")
            << std::endl;
  std::cout << std::endl;

  std::cout << "----- total entry list -----" << std::endl;
  entrylist = cgi.GetValues();
  udp::cgi::CGIBase::EntryList::iterator iter;
  for (iter = entrylist.begin(); iter != entrylist.end(); ++iter) {
    std::cout << iter->first << " = " << iter->second << std::endl;
  }
  std::cout << std::endl;

  std::cout << "----- group entry list -----" << std::endl;
  grouplist = cgi.GetValues("group");
  udp::cgi::CGIBase::EntryList::iterator it;
  for (it = grouplist.begin(); it != grouplist.end(); ++it) {
    std::cout << it->first << " = " << it->second << std::endl;
  }
  std::cout << std::endl;

  std::cout << "----- group entry -----" << std::endl;
  std::string group = cgi.GetValue("group");
  if (group.empty() == FALSE) {
    std::cout << "group = " << group << std::endl;
  }

  return 0;
}
