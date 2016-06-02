#ifndef CREATE_H_
#define CREATE_H_
///////////////////////////////////////////////////////////////////////////////
/// @file       autoregister.hpp
/// @brief      Autoregister class for cgi
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <memory>
#include <map>

#include "udp/keystone/system/types.h"
#include "cgibase.hpp"

namespace udp {
namespace cgi {

/**
 * When a class create new implement class, it can be found from map table.
 * @code
 *   udp::cgi::CGIBase* cgibase_;
 *   cgibase_ = udp::cgi::AutoRegister::GetInstance(identify);
 *   if (cgibase_ == NULL) {
 *     throw std::string("Can't find identify in cgi tables.");
 *   }
 * @endcode
 */
class AutoRegister {
public:  /* Public Constructor */
  /**
   * Default Constructor
   * @param identify a unique key is for distinction
   */
  explicit AutoRegister(const std::string& identify);

  /**
   * Default Distructor
   */
  virtual ~AutoRegister();

public:  /* Public member function */
  /**
   * virtual function that new class can be register.
   * @return CGIBase class
   */
  virtual CGIBase* Create() = 0;

  /**
   * Find the class that a unique key is 'identify'
   * @param identify a unique key
   * @return CGIBase class
   */
  static CGIBase* GetInstance(const std::string& identify);

private:  /* Private methods */
  class Factory;
};  // class AutoRegister

/**
 * 3rd-party Class can be register automatically.
 * @code
 * const udp::cgi::Creator<TycoHello> TycoHello::creator_("/iAPI/hello.cgi");
 * @endcode
 */
template <class T>
class Creator : public AutoRegister {
public:  /* Public constructor */
  /**
   * Default Constructor
   * @param identify a unique key
   */
  explicit Creator(const std::string& identify) : AutoRegister(identify) {}

  /**
   * Default Distructor
   */
  ~Creator() {}

public:  /* Public member functions */
  /**
   * Create a new class for registration into map table
   * @return CGIBase class
   */
  virtual CGIBase* Create() { return new T; }
};

}  // namespace cgi
}  // namespace udp

#endif  // CREATE_H_
