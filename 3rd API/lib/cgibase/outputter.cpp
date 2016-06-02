///////////////////////////////////////////////////////////////////////////////
/// @file       outputter.cpp
/// @brief      Outputter class implementation code
/// @~
/// @author     Harry Oh (harry.oh@udptechnology.com)
/// @copyright  UDP Technology
///////////////////////////////////////////////////////////////////////////////
#include <iostream>    // NOLINT(readability/streams)
#include <string>
#include <cassert>

#include "outputter.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace udp {
namespace cgi {

///////////////////////////////////////////////////////////////////////////////
/**
* Private implementation of the Outputter API
*/
class Outputter::OutputterImpl {
public:  /* Public constructor */
  /**
   * Default Constructor
   */
  OutputterImpl() : style_(kStyleText) {
  }

  /**
   * Creates a OutputterImpl class using message
   * @param msg a message that will be printed
   */
  explicit OutputterImpl(const std::string &msg) : style_(kStyleText) {
    Print(msg);
  }

  /**
   * Default Disstructor
   */
  ~OutputterImpl() {}

public:  /* Public member functions */
  /**
   * @copydoc Outputter::GetStyle
   */
  const Style GetStyle() const {
    return style_;
  }

  /**
   * @copydoc Outputter::SetStyle
   */
  void SetStyle(const Style &style) {
    style_ = style;
  }

  /**
   * @copydoc Outputter::Print
   */
  void Print(const std::string &str) const {
    // when cgi is fastcgi, string should not print cout
    if (style_ == kStyleText) {
      std::cout << str;
    }
  }

private:  /* Private methods */
  /**
   * Enum variable for printing type
   */
  Style style_;
};

///////////////////////////////////////////////////////////////////////////////
Outputter::Outputter() : pimpl_(new OutputterImpl()) {}

Outputter::Outputter(const std::string &msg) {
}

///////////////////////////////////////////////////////////////////////////////
Outputter::~Outputter() {
}

///////////////////////////////////////////////////////////////////////////////
const Outputter::Style Outputter::GetStyle() const {
  assert(pimpl_.get());
  return pimpl_->GetStyle();
}

///////////////////////////////////////////////////////////////////////////////
void Outputter::SetStyle(const Style &style) {
  assert(pimpl_.get());
  return pimpl_->SetStyle(style);
}

///////////////////////////////////////////////////////////////////////////////
void Outputter::Print(const std::string &output) const {
  assert(pimpl_.get());
  return pimpl_->Print(output);
}

}   // namespace cgi
}   // namespace udp
