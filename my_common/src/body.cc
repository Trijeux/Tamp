#include "body.h"
#include "world.h"

namespace common {

Body::Body(const float mass_init) : mass(mass_init) {}

void Body::Velocity(const core::Vec2F& vel) {
  velocity_ = vel;
}

void Body::AddForce(const core::Vec2F& force) {
  accumulated_force += force; // on cumule les forces
}

void Body::Tick(const float dt) {
  // Accélération a = F/m
  core::Vec2F acceleration = accumulated_force / mass;

  // Mise à jour de la vitesse
  velocity_ += acceleration * dt;

  // // Frottement de l'air
  // velocity += -velocity * common::world::air_friction * dt;

  // Mise à jour de la position
  position += velocity_ * dt;

  // Réinitialiser les forces pour la prochaine frame
  accumulated_force = {0,0};
}

} // namespace common
