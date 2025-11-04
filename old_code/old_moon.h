//
// Created by antho on 12.10.2025.
//

#ifndef SOLAR_OLD_MOON_H
#define SOLAR_OLD_MOON_H
#include <utility>

#include "../solar/include/old_planet.h"

namespace solar {
class OldMoon final : public common::DrawInterface {
public:
  explicit OldMoon(const std::string_view name, OldPlanet* planet,
                const core::Vec2F&     offset,
                const float            speed, const float size,
                const SDL_FColor&      color, const int   nb_segment = 20)
    : name_(name),
      planet_(planet),
      speed_(speed),
      size_(size),
      color_(color),
      nb_segment_(nb_segment) {
    orbit_radius_ = offset.magnitude();
    orbit_angle_ = atan2(offset.y, offset.x);
    planet_ = planet; // centre de rotation
    position_ = planet_->position() + offset;
  }

  void Draw() override;
  void Update(float dt);

  [[nodiscard]] std::string name() const {
    return name_;
  }

  [[nodiscard]] core::Vec2F position() const {
    return position_;
  }

private:
  std::string name_;
  OldPlanet*     planet_;
  core::Vec2F position_;
  float       speed_ = 0.f;
  float       size_ = 0;
  SDL_FColor  color_{1.f, 1.f, 1.f, 1.f};
  int         nb_segment_ = 0;
  float       orbit_radius_ = 0.f;
  float       orbit_angle_ = 0.f;
};
}

#endif //SOLAR_OLD_MOON_H