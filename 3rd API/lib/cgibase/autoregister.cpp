///////////////////////////////////////////////////////////////////////////////
/// @file       autoregister.cpp
/// @~english
/// @brief      Creator Implemente
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <iostream>  // NOLINT(readability/streams)
#include <string>
#include <map>

#include "autoregister.hpp"

namespace udp {
namespace cgi {

/**
 * Factory class implementation.
 * This is for AutoRegister class.
 */
class AutoRegister::Factory {
public:  /* Default Constructor */
  Factory() {}
  ~Factory() {}

public:  /* Public static functions */
  /**
   * @copydoc AutoRegister::GetInstance
   */
  static CGIBase* GetInstance(const std::string& identify) {
    std::map<std::string, AutoRegister*>::iterator i;
    i = GetCgiTable().find(identify);

    if (i != GetCgiTable().end()) {
      return i->second->Create();
    } else {
      return static_cast<CGIBase*>(NULL);
    }
  }

  /**
   * Register a new class
   * @param identify a unique key
   * @param creator a new class
   */
  static void Register(const std::string& identify, AutoRegister* creator) {
    GetCgiTable()[identify] = creator;
  }

private:  /* private methods */
  /**
   * Return cgi map table
   * @return cgi map table
   */
  static std::map<std::string, AutoRegister*> &GetCgiTable() {
    static std::map<std::string, AutoRegister*> table;
    return table;
  }
};

///////////////////////////////////////////////////////////////////////////////
AutoRegister::AutoRegister(const std::string& identify) {
  Factory::Register(identify, this);
}

///////////////////////////////////////////////////////////////////////////////
AutoRegister::~AutoRegister() {
}

///////////////////////////////////////////////////////////////////////////////
CGIBase* AutoRegister::GetInstance(const std::string& identify) {
  return Factory::GetInstance(identify);
}

}  // namespace cgi
}  // namespace udp
