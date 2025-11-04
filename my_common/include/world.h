#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include "body.h"
#include "container/indexed_container.h"
#include <unordered_set>
#include <vector>
#include <functional>
#include "maths/vec2.h"

namespace common::world {

// Forward declarations
struct Circle;
struct Collider;
struct ColliderPair;
struct ColliderPairHasher;

using BodyIndex = core::Index<Body>;
using ColliderIndex = core::Index<int>; // indices simples pour les colliders

// Body management
[[nodiscard]] BodyIndex AddBody(float mass);
[[nodiscard]] Body& get_body_at(BodyIndex body_index);
void RemoveBody(BodyIndex body_index);
void Tick(float dt);

// Collider & trigger API
struct Circle {
  float radius = 1.f;
};

struct Collider {
  BodyIndex body{-1}; // invalide par défaut
  Circle circle;

  Collider() = default; // constructeur par défaut explicite
};

struct ColliderPair {
  ColliderIndex a;
  ColliderIndex b;

  bool operator==(const ColliderPair& other) const noexcept {
    return (a == other.a && b == other.b) || (a == other.b && b == other.a);
  }
};

struct ColliderPairHasher {
  std::size_t operator()(const ColliderPair& p) const noexcept {
    int ai = p.a.index();
    int bi = p.b.index();
    if (ai > bi) std::swap(ai, bi);
    return std::hash<int>()(ai) ^ (std::hash<int>()(bi) << 1);
  }
};

// Contact listener interface
class ContactListener {
public:
  virtual ~ContactListener() = default;
  virtual void OnTriggerEnter(ColliderIndex a, ColliderIndex b) = 0;
  virtual void OnTriggerExit(ColliderIndex a, ColliderIndex b) = 0;
};

// Collider functions
[[nodiscard]] ColliderIndex AddCollider(BodyIndex body, float radius);
[[nodiscard]] Collider& GetColliderAt(ColliderIndex idx);
void RemoveCollider(ColliderIndex idx);

void SetContactListener(ContactListener* l);

} // namespace common::world

#endif // CORE_WORLD_H
