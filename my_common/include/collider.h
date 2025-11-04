#ifndef CORE_COLLIDER_H
#define CORE_COLLIDER_H

#include "container/indexed_container.h"
#include "maths/vec2.h"

namespace common {

struct Circle {
  float radius = 1.0f;
};

} // namespace common

namespace common::world {

struct Collider {
  core::Index<core::Index<void>> body; // replaced by proper Index<Body> in world.h
  common::Circle circle;
};

struct ColliderPair {
  core::Index<core::Index<void>> a;
  core::Index<core::Index<void>> b;

  bool operator==(const ColliderPair& other) const noexcept {
    return (a == other.a && b == other.b) || (a == other.b && b == other.a);
  }
};

struct ColliderPairHasher {
  std::size_t operator()(const ColliderPair& p) const noexcept {
    auto ai = p.a.index();
    auto bi = p.b.index();
    if (ai > bi) std::swap(ai, bi);
    // combine
    return std::hash<int>()(ai) ^ (std::hash<int>()(bi) << 1);
  }
};

} // namespace common::world

#endif // CORE_COLLIDER_H
