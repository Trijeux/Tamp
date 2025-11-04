//
// Created by antho on 12.10.2025.
//

#ifndef SOLAR_PLANET_H
#define SOLAR_PLANET_H
#include "body.h"
#include "container/indexed_container.h"
#include "engine/renderer.h"

namespace solar {
class Planet final : public common::DrawInterface {
  std::string name_;
  SDL_FColor  color_{1.f, 1.f, 1.f, 1.f};
  float       size_ = 10;
  int         nb_segment_ = 20;
  core::Vec2F pos_grav_;
  float       orbit_radius_ = 0.f;
  float       orbit_angle_ = 0.f;

  core::Index<common::Body> body_idx_;

public:
  explicit Planet(const std::string_view& name, core::Vec2F offset,
                  core::Vec2F             pos_grav, float   size, float mass,
                  SDL_FColor              color);

  void Update(float dt);

  void Draw() override;

  [[nodiscard]] float orbitAngle() const {
    return orbit_angle_;
  }

  [[nodiscard]] float orbitRadius() const {
    return orbit_radius_;
  }

  [[nodiscard]] core::Index<common::Body> body_idx() const {
    return body_idx_;
  }

  [[nodiscard]] std::string name() const {
    return name_;
  }
};
}

#endif //SOLAR_PLANET_H