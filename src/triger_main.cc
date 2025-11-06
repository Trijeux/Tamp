#include "engine/engine.h"
#include "engine/window.h"
#include "world.h"
#include "body.h"
#include <vector>
#include <random>
#include <imgui.h>
#include <cmath>

#include "engine/gui.h"
#include "engine/renderer.h"

// ------------------- COLLIDER SYSTEM -------------------
namespace collider {
struct Circle {
  common::world::BodyIndex body_index;
  float                    radius;
  float                    r, g, b, a;
};

class ColliderSystem final : public common::SystemInterface,
                             public common::OnGuiInterface,
                             public common::DrawInterface {
public:
  void Begin() override {
    std::uniform_real_distribution<float> posX(50.f, 750.f);
    std::uniform_real_distribution<float> posY(50.f, 550.f);
    std::uniform_real_distribution<float> velDist(-1.f, 1.f);
    std::uniform_real_distribution<float> radDist(minRadius_, maxRadius_);

    for (int i = 0; i < circleCount_; ++i) {
      float                    radius = radDist(rng_);
      common::world::BodyIndex b = common::world::AddBody(1.f);
      auto&                    body = common::world::get_body_at(b);
      body.position = {posX(rng_), posY(rng_)};
      body.Velocity({velDist(rng_) * maxSpeed_, velDist(rng_) * maxSpeed_});

      circles_.push_back({b, radius, 1.f, 0.f, 0.f, 1.f});
    }
  }

  void Update(float dt) override {
    // Déplacement et rebond
    for (auto& c : circles_) {
      auto& body = common::world::get_body_at(c.body_index);
      auto  vel = body.velocity();
      float r = c.radius;

      if (body.position.x - r < 0.f) {
        body.position.x = r;
        vel.x = std::abs(vel.x);
      }
      if (body.position.x + r > 800.f) {
        body.position.x = 800.f - r;
        vel.x = -std::abs(vel.x);
      }
      if (body.position.y - r < 0.f) {
        body.position.y = r;
        vel.y = std::abs(vel.y);
      }
      if (body.position.y + r > 600.f) {
        body.position.y = 600.f - r;
        vel.y = -std::abs(vel.y);
      }
      body.Velocity(vel);
    }

    // Détection de collision simple (triggers)
    for (auto& c : circles_) c.r = 1.f, c.g = 0.f, c.b = 0.f;
    for (size_t i = 0; i < circles_.size(); ++i) {
      auto& b1 = common::world::get_body_at(circles_[i].body_index);
      for (size_t j = i + 1; j < circles_.size(); ++j) {
        auto& b2 = common::world::get_body_at(circles_[j].body_index);
        float dist2 = (b1.position - b2.position).magnitude_sqr();
        float radiusSum = circles_[i].radius + circles_[j].radius;
        if (dist2 <= radiusSum * radiusSum) {
          circles_[i].r = 0.f;
          circles_[i].g = 1.f;
          circles_[i].b = 0.f;
          circles_[j].r = 0.f;
          circles_[j].g = 1.f;
          circles_[j].b = 0.f;
        }
      }
    }

    common::world::Tick(dt);
  }

  void FixedUpdate() override {
  }

  void End() override {
  }

  void Draw() override {
    for (auto& c : circles_) {
      auto& body = common::world::get_body_at(c.body_index);
      common::DrawCircle(body.position.x, body.position.y, c.radius,
                         SDL_FColor{c.r, c.g, c.b, c.a});
    }
  }

  void OnGui() override {
    ImGui::Begin("Collider Sample");

    bool changed = false;

    changed |= ImGui::SliderInt("Nombre de cercles", &circleCount_, 5, 200);
    changed |= ImGui::SliderFloat("Rayon min", &minRadius_, 1.f, maxRadius_);
    changed |= ImGui::SliderFloat("Rayon max", &maxRadius_, 1.f, 100.f);
    changed |= ImGui::SliderFloat("Vitesse min", &minSpeed_, 1.f, maxSpeed_);
    changed |= ImGui::SliderFloat("Vitesse max", &maxSpeed_, 1.f, 500.f);

    if (minRadius_ > maxRadius_) {
      minRadius_ = maxRadius_;
    }

    if (minSpeed_ > maxSpeed_) {
      minSpeed_ = maxSpeed_;
    }

    ImGui::End();

    if (changed) {
      // supprimer les corps existants
      for (auto& c : circles_) {
        common::world::RemoveBody(c.body_index);
      }
      circles_.clear();

      // recréer les corps avec les nouvelles valeurs
      Begin();
    }
  }

private:
  std::vector<Circle> circles_;
  std::mt19937        rng_{std::random_device{}()};

  int   circleCount_ = 30;
  float minRadius_ = 8.f;
  float maxRadius_ = 30.f;
  float minSpeed_ = 50.f;
  float maxSpeed_ = 150.f;
};

void CreateColliderSample() {
  static ColliderSystem system;
  common::SystemObserverSubject::AddObserver(&system);
  common::DrawObserverSubject::AddObserver(&system);
  common::OnGuiObserverSubject::AddObserver(&system);
}
} // namespace collider

// ------------------- MAIN -------------------
int main() {
  common::WindowConfig config;
  config.width = 800;
  config.height = 600;
  common::SetWindowConfig(config);

  collider::CreateColliderSample();

  common::RunEngine();
}