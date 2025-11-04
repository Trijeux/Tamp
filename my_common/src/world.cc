#include "world.h"
#include <ranges>
#include <stdexcept>
#include <vector>
#include <optional>

namespace common::world {
namespace {
  // bodies are already stored as pairs (Body, generation)
  std::vector<std::pair<Body, int>> bodies;

  // colliders storage: stores Collider + generation int (similar to bodies)
  std::vector<std::pair<Collider, int>> colliders;

  // active overlapping pairs
  std::unordered_set<ColliderPair, ColliderPairHasher> activePairs;

  ContactListener* listener = nullptr;
}

// ---------- Body functions (adapted from your existing code) ----------
[[nodiscard]] BodyIndex AddBody(const float mass) {
  const auto it = std::ranges::find_if(bodies, [&](const auto& body) {
    return body.first.IsInvalid();
  });
  if (it != bodies.end()) {
    it->first.mass = mass;
    return BodyIndex{static_cast<int>(std::distance(bodies.begin(), it)), it->second};
  }
  auto body = Body(mass);
  const BodyIndex idx(static_cast<int>(bodies.size()));
  bodies.emplace_back(body, 0);
  return idx;
}

[[nodiscard]] Body& get_body_at(const BodyIndex body_index) {
  if (body_index.index() < 0 ||
      body_index.index() >= static_cast<int>(bodies.size()))
    throw std::out_of_range("Trying to get a body with an out of range index");

  if (body_index.generationIndex() != bodies[body_index.index()].second) {
    throw std::runtime_error("Trying to get a body with an invalid generation index");
  }
  return bodies[body_index.index()].first;
}

void RemoveBody(const BodyIndex body_index) {
  if (body_index.index() < 0 ||
      body_index.index() >= static_cast<int>(bodies.size())) {
    throw std::out_of_range("Trying to remove a body with an out of range index");
  }
  if (body_index.generationIndex() != bodies[body_index.index()].second) {
    throw std::runtime_error("Trying to remove a body with an invalid generation index");
  }
  bodies[body_index.index()].first.mass = -1;
  bodies[body_index.index()].second++;
}

void Tick(const float dt) {
  for (auto& key : bodies | std::views::keys) {
    key.Tick(dt);
  }

  // --- Trigger detection (naive O(n^2) for simplicity) ---
  std::unordered_set<ColliderPair, ColliderPairHasher> newPairs;

  const int n = static_cast<int>(colliders.size());
  for (int i = 0; i < n; ++i) {
    // skip invalid
    if (colliders[i].first.body.index() < 0) continue;
    for (int j = i + 1; j < n; ++j) {
      if (colliders[j].first.body.index() < 0) continue;

      const auto& A = colliders[i].first;
      const auto& B = colliders[j].first;

      const auto& posA = get_body_at(A.body).position;
      const auto& posB = get_body_at(B.body).position;

      const float r = A.circle.radius + B.circle.radius;
      const float dx = posA.x - posB.x;
      const float dy = posA.y - posB.y;
      const float dist2 = dx*dx + dy*dy;

      if (dist2 <= r * r) {
        ColliderPair p{ ColliderIndex(i), ColliderIndex(j) };
        newPairs.insert(p);
        // Enter event
        if (activePairs.find(p) == activePairs.end() && listener) {
          listener->OnTriggerEnter(p.a, p.b);
        }
      }
    }
  }

  // Exit events: pairs that were active but not in newPairs
  for (const auto& oldP : activePairs) {
    if (newPairs.find(oldP) == newPairs.end() && listener) {
      listener->OnTriggerExit(oldP.a, oldP.b);
    }
  }

  activePairs = std::move(newPairs);
}

// ---------- Collider functions ----------
[[nodiscard]] ColliderIndex AddCollider(const BodyIndex body, const float radius) {
  // try to reuse
  const auto it = std::ranges::find_if(colliders, [&](const auto& c) {
    return c.first.body.index() < 0;
  });
  if (it != colliders.end()) {
    it->first.body = body;
    it->first.circle.radius = radius;
    return ColliderIndex{static_cast<int>(std::distance(colliders.begin(), it)), it->second};
  }
  Collider c;
  c.body = body;
  c.circle.radius = radius;
  const ColliderIndex idx(static_cast<int>(colliders.size()));
  colliders.emplace_back(c, 0);
  return idx;
}

[[nodiscard]] Collider& GetColliderAt(const ColliderIndex idx) {
  if (idx.index() < 0 || idx.index() >= static_cast<int>(colliders.size()))
    throw std::out_of_range("Trying to get a collider with an out of range index");
  if (idx.generationIndex() != colliders[idx.index()].second)
    throw std::runtime_error("Trying to get a collider with an invalid generation index");
  return colliders[idx.index()].first;
}

void RemoveCollider(const ColliderIndex idx) {
  if (idx.index() < 0 || idx.index() >= static_cast<int>(colliders.size()))
    throw std::out_of_range("Trying to remove a collider with an out of range index");
  if (idx.generationIndex() != colliders[idx.index()].second)
    throw std::runtime_error("Trying to remove a collider with an invalid generation index");
  // mark invalid: set body index negative
  colliders[idx.index()].first.body = BodyIndex(-1);
  colliders[idx.index()].second++;
}

void SetContactListener(ContactListener* l) {
  listener = l;
}

} // namespace common::world
