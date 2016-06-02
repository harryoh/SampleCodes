///////////////////////////////////////////////////////////////////////////////
/// @file       tycocgi.cpp
/// @~english
/// @brief      Implementation of the TycoCGI class
/// @~
/// @authors    Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <cassert>

#include "udp/keystone/system/types.h"

#include "tycoerror.hpp"
#include "tycocgi.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

///////////////////////////////////////////////////////////////////////////////
/**
* Private implementation of the TycoCGI API
*/
class TycoCGI::TycoCGIImpl {
public:  /* Constructors */
  TycoCGIImpl() {}
  ~TycoCGIImpl() {}

public:  /* Public member functions */
  /**
   * Implemente check querystring is validated.
   * @param entrylist parameter entry list
   */
  void CheckParameter(const EntryList &entrylist) const {
    // Check Common validation.
    // throw TycoError(kCodeMissingParameter, "action");
  }
};

///////////////////////////////////////////////////////////////////////////////
TycoCGI::TycoCGI() : pimpl_(new TycoCGIImpl()) {}

///////////////////////////////////////////////////////////////////////////////
TycoCGI::~TycoCGI() {}

///////////////////////////////////////////////////////////////////////////////
TycoCGI::TycoCGI(const std::string &querystring)
  : CGIBase(querystring), pimpl_(new TycoCGIImpl()) {}

///////////////////////////////////////////////////////////////////////////////
void TycoCGI::CheckParameter() const {
  assert(pimpl_.get());
  EntryList entrylist = GetValues();
  return pimpl_->CheckParameter(entrylist);
}

}   // namespace cgi
}   // namespace udp
