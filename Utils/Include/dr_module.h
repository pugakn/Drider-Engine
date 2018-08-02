#pragma once
#include "dr_util_prerequisites.h"

namespace driderSDK {

template<class T>
class Module
{
 public:
  
  /**
  * TEST::instance
  *
  */
  static T&
  instance() {
    if (isShutDown()) {
      throw "Trying to access a module but it hasn't been started.";
    }

    if (isDestroyed()) {
      throw "Trying to access a destroyed module.";
    }
    return *_instance();
  }

  /**
  * TEST::instancePtr
  *
  */
  static T*
  instancePtr() {
    if (isShutDown()) {
      throw "Trying to access a module but it hasn't been started.";
    }

    if (isDestroyed()) {
      throw "Trying to access a destroyed module.";
    }
    return _instance();
  }

  /**
  * TEST::startUp
  *
  */
  template<class... Args> static void
  startUp(Args&& ...args) {
    if (!isShutDown()) {
      throw "Trying to start an already started module.";
    }

    _instance() = new T(std::forward<Args>(args)...);
    isShutDown() = false;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  * TEST::startUp
  *
  */
  template<class SubType, class... Args> static void
  startUp(Args&& ...args) {
    static_assert(std::is_base_of<T, SubType>::value,
      "Provided type isn't derived from type the Module is initialized with.");

    if (!isShutDown()) {
      throw "Trying to start an already started module.";
    }

    _instance() = new SubType(std::forward<Args>(args)...);
    isShutDown() = false;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  * TEST::shutDown
  *
  */
  static void
  shutDown() {
    if (isShutDown() || isDestroyed()) {
      throw "Trying to shut down an already shut down module.";
    }

    static_cast<Module*>(_instance())->onShutDown();

    delete(_instance());
    isShutDown() = true;
  }

  /**
  * TEST::isStarted
  *
  */
  static bool
  isStarted() {
    return !isShutDown() && !isDestroyed();
  }

 protected:
  
  /**
  * TEST::DefaultConstructor
  *
  */
  Module() = default;

  /**
  * TEST::DefaultDestructor
  *
  */
  virtual ~Module() {
    _instance() = nullptr;
    isDestroyed() = true;
  }

  /**
  * TEST::Module
  *
  */
  Module(const Module&) = delete;

  /**
  * TEST::Module
  *
  */
  Module(Module&&) = delete;

  /**
  * TEST::operatorEqual
  *
  */
  Module&
  operator=(const Module&) = delete;

  /**
  * TEST::operatorEqual
  *
  */
  Module&
  operator=(Module&&) = delete;

  /**
  * TEST::onStartUp
  *
  */
  virtual void
  onStartUp() {}

  /**
  * TEST::onShutDown
  *
  */
  virtual void
  onShutDown() {}

  /**
  * TEST::_instance
  *
  */
  static T*&
  _instance() {
    static T* inst = nullptr;
    return inst;
  }

  /**
  * TEST::isDestroyed
  *
  */
  static bool&
  isDestroyed() {
    static bool inst = false;
    return inst;
  }

  /**
  * TEST::isShutDown
  *
  */
  static bool&
  isShutDown() {
    static bool inst = true;
    return inst;
  }
};

}