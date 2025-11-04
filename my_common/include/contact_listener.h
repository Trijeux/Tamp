#ifndef CORE_CONTACT_LISTENER_H
#define CORE_CONTACT_LISTENER_H

#include "container/indexed_container.h"

namespace common::world {

class ContactListener {
public:
  virtual ~ContactListener() = default;

  // called when two colliders start overlapping
  virtual void OnTriggerEnter(core::Index<core::Index<void>> a, core::Index<core::Index<void>> b) = 0;

  // called when two colliders stop overlapping
  virtual void OnTriggerExit(core::Index<core::Index<void>> a, core::Index<core::Index<void>> b) = 0;
};

} // namespace common::world

#endif // CORE_CONTACT_LISTENER_H
