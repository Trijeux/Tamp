//
// Created by antho on 12.10.2025.
//

#ifndef SOLAR_OLD_PLANET_H
#define SOLAR_OLD_PLANET_H
#include "engine/gui.h"
#include "engine/renderer.h"
#include "maths/vec2.h"

namespace solar {
class OldPlanet final : public common::DrawInterface {
  std::string name_;
  core::Vec2F position_;
  core::Vec2F pos_grav_;
  float       speed_ = 0.f;
  float       size_ = 0;
  SDL_FColor  color_{1.f, 1.f, 1.f, 1.f};
  float       orbit_radius_ = 0.f;
  float       orbit_angle_ = 0.f;
  int         count_years_ = 0;
  int         nb_segment_ = 0;

public:
  [[nodiscard]] int count_years() const {
    return count_years_;
  }

  explicit OldPlanet(std::string name, core::Vec2F   pos, core::Vec2F offset,
                  float       speed_planet, float size_planet,
                  SDL_FColor  color_planet, int   nb_segment = 20);

  [[nodiscard]] std::string name() const {
    return name_;
  }

  [[nodiscard]] core::Vec2F position() const {
    return position_;
  }

  void Update(float dt);

  void Draw() override;
};
}

#endif //SOLAR_OLD_PLANET_H