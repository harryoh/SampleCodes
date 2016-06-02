///////////////////////////////////////////////////////////////////////////////
/// @file       tycohello.cpp
/// @~english
/// @brief      Sample CGI implementation code
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <map>
#include <stdexcept>
#include <cassert>

#include "udp/keystone/system/types.h"

#include "tycoerror.hpp"
#include "autoregister.hpp"
#include "tycohello.hpp"

///////////////////////////////////////////////////////////////////////////////
TycoHello::TycoHello() {
}

///////////////////////////////////////////////////////////////////////////////
TycoHello::TycoHello(const std::string &querystring) : TycoCGI(querystring) {
}

///////////////////////////////////////////////////////////////////////////////
TycoHello::~TycoHello() {
}

///////////////////////////////////////////////////////////////////////////////
void TycoHello::Run() {
  try {
//    TycoCGI::CheckParameter();
    CheckParameter();
  } catch(const udp::cgi::TycoError &e) {
//    SetHeaderStatus(500);
    PrintHeader();
    Print(static_cast<std::string>(e.what()));
    throw e;
  }

  std::cerr << "-------- Tyco Hello Run --------" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
void TycoHello::CheckParameter() const {
  EntryList entrylist = GetValues();
  EntryList::iterator iter;

  // check action
  iter = entrylist.find("action");
  if (iter == entrylist.end()) {
    throw udp::cgi::TycoError(
        udp::cgi::TycoCGI::kCodeMissingParameter, "action");
  } else {
    if (iter->second == "read") {
    } else if (iter->second == "update") {
    } else if (iter->second == "explain") {
    } else {
      throw udp::cgi::TycoError(
          udp::cgi::TycoCGI::kCodeValueError, "action=" + iter->second);
    }
  }
}

const udp::cgi::Creator<TycoHello> TycoHello::creator_("/iAPI/hello.cgi");
