///////////////////////////////////////////////////////////////////////////////
/// @file       tycoReboot.cpp
/// @~english
/// @brief      tycoReboot CGI implementation code
/// @~
/// @authors    Charles Lee (charles.lee@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdexcept>
#include <string>

#include "tycoerror.hpp"
#include "autoregister.hpp"
#include "tycoReboot.hpp"


CTycoReboot::CTycoReboot() {
}

CTycoReboot::CTycoReboot(Char** argv) {
}

CTycoReboot::CTycoReboot(const std::string &querystring)
  : TycoCGI(querystring) {
}

void CTycoReboot::Run() {
  try {
    PrintHeader();
    CheckParameter();

    if ( ::system("/sbin/reboot") )
      throw udp::cgi::TycoError(udp::cgi::TycoCGI::kCodeExecutionError);

    Print("Now Rebooting...\r\n");
  } catch(const udp::cgi::TycoError &e) {
    Print(e.what());
  }
}

const udp::cgi::Creator<CTycoReboot> CTycoReboot::creator_("/iAPI/reboot.cgi");
