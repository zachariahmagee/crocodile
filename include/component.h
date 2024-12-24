#pragma once

namespace croc {

  class Component {
  private:
  public:
    bool isVisible = false;
    virtual ~Component() = default;
    virtual void draw() const = 0;
    //virtual bool isVisible() { return isVisible; };
  };
} // namespace try
