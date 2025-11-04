#ifndef CORE_WORLD_H
#define CORE_WORLD_H
#include "body.h"
#include "container/indexed_container.h"

namespace common::world {
[[nodiscard]] core::Index<Body> AddBody(float mass);
[[nodiscard]] Body& get_body_at(core::Index<Body> body_index);
void RemoveBody(core::Index<Body> body_index);
void Tick(float dt);
}

#endif //CORE_WORLD_H