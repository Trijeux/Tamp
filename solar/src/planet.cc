#include "planet.h"
#include "engine/renderer.h"
#include "world.h"
#include <cmath>

namespace solar {

Planet::Planet(const std::string_view& name, const core::Vec2F offset,
               const core::Vec2F pos_grav, const float size,
               const float mass, const SDL_FColor color) : body_idx_(common::world::AddBody(mass)) {

  // Initialisation correcte
  pos_grav_ = pos_grav; // centre de rotation
  common::world::get_body_at(body_idx_).position = pos_grav_ + offset;

  // Calcul initial de l'orbite
  const core::Vec2F pos = pos_grav_ - common::world::get_body_at(body_idx_).
                          position;
  orbit_radius_ = pos.magnitude();
  orbit_angle_ = atan2(offset.y, offset.x);

  name_ = name;
  size_ = size;
  color_ = color;
}

void Planet::Update(const float dt) {
  const core::Vec2F new_pos = pos_grav_ - common::world::get_body_at(body_idx_).position;
  orbit_radius_ = new_pos.magnitude();
}

void Planet::Draw() {
  if (const auto* renderer = common::GetRenderer(); !renderer) return;
  common::DrawCircle(common::world::get_body_at(body_idx_).position.x, common::world::get_body_at(body_idx_).position.y, size_, color_, nb_segment_);
}

}  // namespace solar
