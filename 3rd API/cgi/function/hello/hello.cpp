///////////////////////////////////////////////////////////////////////////////
/// @file       hello.cpp
/// @~english
/// @brief      Hello Implemente
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <stdexcept>
#include <cassert>

#include <stdlib.h>    // NOLINT(build/include_order)

#include "autoregister.hpp"
#include "hello.hpp"

///////////////////////////////////////////////////////////////////////////////
Hello::Hello(Char** argv) : cgibase_(0) {
  if (argv && argv[1]) {
    // Todo: should parsing argv[1] to request url;
    SizeT next = static_cast<std::string>(argv[1]).find_first_of("?");
    std::string scriptname = static_cast<std::string>(argv[1]).substr(0, next);
    std::string uri = "http://localhost" + scriptname;

    if (next != std::string::npos) {
      std::string querystring =
          static_cast<std::string>(argv[1]).substr(next+1);
      setenv("QUERY_STRING", querystring.c_str(), true);
    }
    setenv("REQUEST_METHOD", "GET", true);
    setenv("REQUEST_URI", uri.c_str(), true);
    setenv("SCRIPT_NAME", scriptname.c_str(), true);
  }
}

///////////////////////////////////////////////////////////////////////////////
Hello::Hello(const std::string &url) : cgibase_(0) {
  // Todo: should parsing argv[1] to request url;
  SizeT next = url.find_first_of("?");
  std::string scriptname = url.substr(0, next);
  std::string uri = "http://localhost" + scriptname;

  if (next != std::string::npos) {
    std::string querystring = url.substr(next+1);
    setenv("QUERY_STRING", querystring.c_str(), true);
  }
  setenv("REQUEST_METHOD", "GET", true);
  setenv("REQUEST_URI", uri.c_str(), true);
  setenv("SCRIPT_NAME", scriptname.c_str(), true);
}

///////////////////////////////////////////////////////////////////////////////
Hello::~Hello() {
  delete cgibase_;
}

///////////////////////////////////////////////////////////////////////////////
void Hello::Prepare() {
  std::string identify;
  String value = getenv("SCRIPT_NAME");

  identify = (value) ? static_cast<std::string>(value) : "";
  if (identify.empty()) {
    throw std::invalid_argument(
        "Cane read SCRIPT_NAME of environment variables.");
  }

  cgibase_ = udp::cgi::AutoRegister::GetInstance(identify);
  if (cgibase_ == NULL) {
    throw std::invalid_argument("Can't find identify in cgi tables.");
  }
}

///////////////////////////////////////////////////////////////////////////////
void Hello::Run() {
  assert(cgibase_);
  std::cerr << "------- Common Hello Run -------" << std::endl;
  cgibase_->Run();
  cgibase_->PrintHeader();
  cgibase_->Print("Hello!!");
}

