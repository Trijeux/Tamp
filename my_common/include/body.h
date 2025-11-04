#ifndef COMMON_BODY_H
#define COMMON_BODY_H

#include <vector>

#include "maths/vec2.h"

namespace common {

class Body {
public:
  explicit Body(float mass_init);

  core::Vec2F position = {0,0};
  float mass = 0.1f;

  void Velocity(const core::Vec2F& vel);
  void AddForce(const core::Vec2F& force);
  void Tick(float dt);

  [[nodiscard]] bool IsInvalid() const {return mass <= 0.f;}
private:

  core::Vec2F velocity = {0,0};
  core::Vec2F accumulated_force = {0,0};
};

} // namespace common

#endif // COMMON_BODY_H
