#include "world.h"

#include <ranges>
#include <stdexcept>
#include <vector>

#include "container/indexed_container.h"

namespace common::world {
namespace {
std::vector<std::pair<Body, int>> bodies;
}

[[nodiscard]] core::Index<Body> AddBody(const float mass) {
  const auto it = std::ranges::find_if(bodies, [&](const auto& body) {
    return body.first.IsInvalid();
  });
  if (it != bodies.end()) {
    it -> first.mass = 1;
    return core::Index<Body>{static_cast<int>(std::distance(bodies.begin(), it)), it->second};
  }
  auto body = Body(mass);
  const core::Index<Body> idx(static_cast<int>(bodies.size()));
  bodies.emplace_back(body, 0);
  return idx;
}

[[nodiscard]] Body& get_body_at(const core::Index<Body> body_index) {
  if (body_index.index() < 0 ||
      body_index.index() >= static_cast<int>(bodies.size()))
    throw std::out_of_range("Trying to get a body with an out of range index");

  if (body_index.generationIndex() != bodies[body_index.index()].second) {
    throw std::runtime_error(
        "Trying to get a body with an invalid generation index");
  }
  return bodies[body_index.index()].first;
}

void RemoveBody(const core::Index<Body> body_index) {
  if (body_index.index() < 0 ||
      body_index.index() >= static_cast<int>(bodies.size())) {
    throw std::out_of_range(
        "Trying to remove a body with an out of range index");
      }
  if (body_index.generationIndex() != bodies[body_index.index()].second) {
    throw std::runtime_error(
        "Trying to remove a body with an invalid generation index");
  }
  bodies[body_index.index()].first.mass = -1;
  bodies[body_index.index()].second++;
}

void Tick(const float dt) {
  for (auto& key : bodies | std::views::keys) {
    key.Tick(dt);
  }
}
}