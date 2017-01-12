#ifndef CALLBACKTIMER_H_
#define CALLBACKTIMER_H_

#include "simpleTimer.h"

template <class T>
class CallbackTimer : public simpleTimer {
  private:
    void (T::*callback)();
    T* objectToCall = NULL;
  public:

    /* Templated function definitions must be carried in header
     * so that they are available to the linker at compile time
     * as instantiation occurs. */
    CallbackTimer(T* objectToCall, void (T::*callback)(),
                  bool repeatTimer) : simpleTimer(repeatTimer) {
      this->callback = callback;
      this->objectToCall = objectToCall;
    }

    void process() {
      simpleTimer::process();

      /* call the callback if configured to do so */
      if (isComplete() && objectToCall != NULL) {
        (objectToCall->*callback)();
      }
    }
};

#endif
