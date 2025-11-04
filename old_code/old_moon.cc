//
// Created by antho on 12.10.2025.
//

#include "old_moon.h"

namespace solar {
static constexpr float kUnitmetre = 100;

void OldMoon::Draw() {
  if (const auto* renderer = common::GetRenderer(); !renderer) return;

  common::DrawCircle(position_.x, position_.y, size_, color_, nb_segment_);
}

void OldMoon::Update(float dt) {
  if (speed_ <= 0.0) {
    return;
  }
  const float angular_speed = kUnitmetre * speed_ / orbit_radius_;
  orbit_angle_ += angular_speed * dt;
  const float new_x = orbit_radius_ * cos(orbit_angle_);
  const float new_y = orbit_radius_ * sin(orbit_angle_);
  position_ = planet_->position() + core::Vec2F{new_x, new_y};
}
}