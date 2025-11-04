#ifndef CORE_CONTACT_LISTENER_H
#define CORE_CONTACT_LISTENER_H

#include "container/indexed_container.h"
#include "core/world.h" // pour ColliderIndex si besoin

namespace common::world {

class ContactListener {
public:
  virtual ~ContactListener() = default;
  virtual void OnTriggerEnter(ColliderIndex a, ColliderIndex b) = 0;
  virtual void OnTriggerExit(ColliderIndex a, ColliderIndex b) = 0;
};

} // namespace common::world

#endif // CORE_CONTACT_LISTENER_H
