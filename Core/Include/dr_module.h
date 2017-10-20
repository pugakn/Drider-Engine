#pragma once
#include <dr_prerequisites.h>

namespace driderSDK {
/**
* Represents one engine module. Essentially it is a specialized type
* of singleton. Module must be manually started up and shut down
* before and after use.
*
*/
template<class T>
class Module
{
 public:
  /**
  * @return
  *   A reference to the module instance. Module has to have
  *   been started up first otherwise an exception will be thrown.
  */
  static T&
  instance() {
    if (isShutDown()) {
      return nullptr;
      //dr_EXCEPT(InternalErrorException, "Trying to access a module but it hasn't been started.");
    }
    if (isDestroyed()) {
      return nullptr;
      //DR_EXCEPT(InternalErrorException, "Trying to access a destroyed module.");
    }

    return *_instance();
  }

  /**
  * @return
  *   A pointer to the module instance. Module has to have been
  *   started up first otherwise an exception will be thrown.
  *
  */
  static T*
  instancePtr() {
    if (isShutDown()) {
      return nullptr;
      //DR_EXCEPT(InternalErrorException, "Trying to access a module but it hasn't been started.");
    }

    if (isDestroyed()) {
      return nullptr;
      //GE_EXCEPT(InternalErrorException, "Trying to access a destroyed module.");
    }

    return _instance();
  }

  /**
  * Constructs and starts the module using the specified parameters.
  *
  */
  template<class... Args>
  static void
  startUp(Args&& ...args) {
    if (!isShutDown()) {
      //GE_EXCEPT(InternalErrorException, "Trying to start an already started module.");
      return;
    }

    _instance() = new T(std::forward<Args>(args)...);
    isShutDown() = false;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  * Constructs and starts a specialized type of the module.
  * Provided type must derive from type the Module is initialized with.
  *
  */
  template<class SubType, class... Args>
  static void
  startUp(Args&& ...args) {
    //DR_ASSERT(std::is_base_of<T, SubType>::value);

    if (!isShutDown()) {
      //GE_EXCEPT(InternalErrorException, "Trying to start an already started module.");
    }

    _instance() = new SubType(std::forward<Args>(args)...);
    isShutDown() = false;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  * Shuts down this module and frees any resources it is using.
  *
  */
  static void
  shutDown() {
    if (isShutDown() || isDestroyed()) {
      //GE_EXCEPT(InternalErrorException, "Trying to shut down an already shut down module.");
    }

    static_cast<Module*>(_instance())->onShutDown();

    delete(_instance());
    isShutDown() = true;
  }

  /**
  * Query if the module has been started.
  *
  */
  static bool
  isStarted() {
    return !isShutDown() && !isDestroyed();
  }

 protected:
  /**
  * Default constructor.
  *
  */
  Module() {}

  /**
  * Default constructor (virtual).
  *
  */
  virtual
  ~Module() {
    _instance() = nullptr;
    isDestroyed() = true;
  }

  /**
  * Default copy constructor.
  *
  */
  Module(const Module&) {}

  Module& operator=(const Module&) {
    return *this;
  }

  /**
  * @brief
  *   Override if you want your module to be notified once it has been
  *   constructed and started.
  *
  * @note
  *   Useful when your module is polymorphic and you cannot perform some
  *   implementation specific initialization in constructor itself.
  *
  */
  virtual void
  onStartUp() {}

  /**
  * @brief
  *   Override if you want your module to be notified just before it is deleted.
  *
  * @note
  *   Useful when your module is polymorphic and you might want to perform
  *   some kind of clean up perhaps overriding that of a base class.
  *
  */
  virtual void
  onShutDown() {}

  /**
  * @brief
  *   Returns a singleton instance of this module. Throws an exception
  *   if module is not yet initialized.
  */
  static T*&
  _instance() {
  static T* inst = nullptr;
    return inst;
  }

  /**
  * @brief
  *   Checks has the Module been shut down.
  *
  * @note
  *   If module was never even started, this will return false.
  *
  */
  static bool&
  isDestroyed() {
    static bool inst = false;
    return inst;
  }

  /**
  * @brief
  *   Checks has the Module been started up.
  *
  */
  static bool&
  isShutDown() {
    static bool inst = true;
    return inst;
  }
};

}