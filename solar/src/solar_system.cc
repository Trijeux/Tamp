//
// Created by antho on 12.10.2025.
//

#include "solar_system.h"

#include <vector>
#include <imgui.h>

#include "planet.h"
#include "world.h"
#include "engine/engine.h"
#include "engine/gui.h"
#include "engine/system.h"
#include "engine/window.h"

namespace solar {
class SolarSystem final : public common::SystemInterface,
                          public common::OnGuiInterface {
private:
  std::vector<Planet> planets_;
  void                AddPlanet(const Planet& planet);
  float gravity_ = 5.f;

public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void FixedUpdate() override;
  void OnGui() override;
};

namespace {
SolarSystem system;
}

void CreateSolarSystem() {
  common::SystemObserverSubject::AddObserver(&system);
  common::OnGuiObserverSubject::AddObserver(&system);
}

void SolarSystem::AddPlanet(const Planet& planet) {
  planets_.push_back(planet);
}

void SolarSystem::End() {
}

void SolarSystem::Begin() {
  // Création du Soleil
  Planet sun("Sun", {kWidth / 2, kHeight / 2}, {0, 0}, 50, 5000,
             {1.f, 1.f, 0.2f, 1.f});
  AddPlanet(sun);

  // Création de la Terre
  const core::Vec2F offset = {200, 0};
  Planet            earth("Earth", offset, common::world::get_body_at(sun.body_idx()).position, 10, 10,
               {0.f, 0.6f, 1.f, 1.f});
  AddPlanet(earth);

  // Enregistrer les planètes
  for (auto& p : planets_) common::DrawObserverSubject::AddObserver(&p);

  // Constantes physiques
  gravity_ = 250.f; // gravité douce pour stabilité

  // ------------------------------
  // Vitesse orbitale initiale
  // ------------------------------
  auto& sun_body = common::world::get_body_at(planets_[0].body_idx());
  auto& earth_body = common::world::get_body_at(planets_[1].body_idx());

  core::Vec2F dir = sun_body.position - earth_body.position;
  float       distance = dir.magnitude();
  core::Vec2F dir_norm = dir / distance;

  // Vecteur tangent (orbite antihoraire)
  core::Vec2F tangent = {dir_norm.y, -dir_norm.x};

  // Vitesse orbitale équilibrée (ajuste le facteur si nécessaire)
  const float v = std::sqrt(gravity_ * sun_body.mass / distance);

  earth_body.Velocity(tangent * v);
}

void SolarSystem::Update(const float dt) {
  for (auto& p : planets_) {
    p.Update(dt);
  }
}

void SolarSystem::FixedUpdate() {
  // Position du Soleil
  const core::Vec2F pos_sun = common::world::get_body_at(planets_[0].body_idx()).position;
  const float       mass_sun = common::world::get_body_at(planets_[0].body_idx()).mass;

  // Appliquer la gravité pour chaque planète
  for (int i = 1; i < planets_.size(); ++i) {
    const float       planet_mass = common::world::get_body_at(planets_[i].body_idx()).mass;
    const core::Vec2F pos_planet = common::world::get_body_at(planets_[i].body_idx()).position;

    core::Vec2F dir = pos_sun - pos_planet; // vecteur vers le Soleil
    float       distance = dir.magnitude();
    if (distance > 1e-4f) {
      core::Vec2F dir_norm = dir / distance;
      float force_mag = gravity_ * planet_mass * mass_sun / (
                          distance * distance);
      common::world::get_body_at(planets_[i].body_idx()).AddForce(dir_norm * force_mag);
    }
  }
  common::world::Tick(common::GetFixedDT());
}

void SolarSystem::OnGui() {
  ImGui::Begin("Info Solar System");
  for (Planet& planet : planets_) {
    ImGui::Text("Pos %s : %f %f", planet.name().c_str(),
                common::world::get_body_at(planet.body_idx()).position.x,
                 common::world::get_body_at(planet.body_idx()).position.y);
  }
  ImGui::End();
}
} // namespace solar