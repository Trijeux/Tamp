//
// Created by antho on 12.10.2025.
//
static constexpr float kUnitmetre = 100;

#include <utility>

#include "old_planet.h"

namespace solar {
OldPlanet::OldPlanet(std::string       name, const core::Vec2F pos_grav,
               const core::Vec2F offset,
               const float       speed_planet, const float size_planet,
               const SDL_FColor  color_planet, const int   nb_segment)
  : name_(std::move(name)), speed_(speed_planet), size_(size_planet),
    color_(color_planet), nb_segment_(nb_segment) {
  orbit_radius_ = offset.magnitude();
  orbit_angle_ = atan2(offset.y, offset.x);
  pos_grav_ = pos_grav; // centre de rotation
  position_ = pos_grav_ + offset;
}

void OldPlanet::Update(const float dt) {
  if (speed_ == 0.0f) return;

  const float angular_speed = kUnitmetre * speed_ / orbit_radius_;
  orbit_angle_ += angular_speed * dt;

  if (orbit_angle_ >= 2.f * 3.14159265f) {
    orbit_angle_ -= 2.f * 3.14159265f;
    count_years_++;
  }

  const float new_x = orbit_radius_ * cos(orbit_angle_);
  const float new_y = orbit_radius_ * sin(orbit_angle_);
  position_ = pos_grav_ + core::Vec2F{new_x, new_y};
}

void OldPlanet::Draw() {
  if (const auto* renderer = common::GetRenderer(); !renderer) return;

  common::DrawCircle(position_.x, position_.y, size_, color_, nb_segment_);
}
}