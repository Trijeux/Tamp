//
// Created by antho on 12.10.2025.
//

#include <vector>
#include <imgui.h>

#include "old_solar_system.h"
#include "old_moon.h"
#include "old_planet.h"
#include "engine/engine.h"
#include "engine/system.h"

namespace solar {

class OldSolarSystem final : public common::SystemInterface,
                          public common::OnGuiInterface {
private:
  std::vector<OldPlanet> planets_;
  std::vector<OldMoon>   moons_;
  void                AddPlanet(const OldPlanet& planet);
  void                AddMoon(const OldMoon& moon);

public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void OnGui() override;
};

namespace {
  OldSolarSystem system;
}

static constexpr float kHeight = 900;
static constexpr float kWidth = 1700;
static constexpr float kMutispeed = 1;

void OldSolarSystem::AddPlanet(const OldPlanet& planet) {
  planets_.push_back(planet);
}

void OldSolarSystem::AddMoon(const OldMoon& moon) {
  moons_.push_back(moon);
}

void OldSolarSystem::End() {
}

void CreateOldSolarSystem() {
  common::SystemObserverSubject::AddObserver(&system);
  common::OnGuiObserverSubject::AddObserver(&system);
}

void OldSolarSystem::Begin() {
  // Soleil
  const OldPlanet sun(
      "Sun", {static_cast<float>(kWidth) / 2, static_cast<float>(kHeight) / 2},
      {0, 0}, 0, 50.0f,
      {1.f, 1.f, 0.f, 1.f}); // Jaune
  AddPlanet(sun);

  // Mercure
  const OldPlanet mercury("Mercury", sun.position(), {100, 0}, 1.728f * kMutispeed,
                       2.5f,
                       {150.f / 255, 150.f / 255, 150.f / 255, 1}); // Gris
  AddPlanet(mercury);

  // Vénus
  const OldPlanet venus("Venus", sun.position(), {150, 0}, 1.26f * kMutispeed,
                     4.0f,
                     {1.f, 165.f / 255, 0, 1.f}); // Orange
  AddPlanet(venus);

  // Terre
  const OldPlanet earth("Earth", sun.position(), {200, 0}, 1.044f * kMutispeed,
                     4.5f,
                     {0, 0, 1.f, 1.f}); // Bleu
  AddPlanet(earth);

  // Mars
  const OldPlanet mars("Mars", sun.position(), {250, 0}, 0.861f * kMutispeed, 3.5f,
                    {1.f, 0, 0, 1.f}); // Rouge
  AddPlanet(mars);

  // Jupiter
  const OldPlanet jupiter("Jupiter", sun.position(), {300, 0}, 0.468f * kMutispeed,
                       20.0f,
                       {222.f / 255, 184.f / 255, 135.f / 255, 1});
  // Orange (géante)
  AddPlanet(jupiter);

  // Saturne
  const OldPlanet saturn("Saturn", sun.position(), {350, 0}, 0.36f * kMutispeed,
                      17.0f,
                      {1.f, 1.f, 102.f / 255, 1.f}); // Jaune clair
  AddPlanet(saturn);

  // Uranus
  const OldPlanet uranus("Uranus", sun.position(), {400, 0}, 0.252f * kMutispeed,
                      10.0f,
                      {0, 1.f, 1.f, 1.f}); // Cyan
  AddPlanet(uranus);

  // Neptune
  const OldPlanet neptune("Neptune", sun.position(), {450, 0}, 0.18f * kMutispeed,
                       9.0f,
                       {0, 0, 128.f / 255, 1}); // Bleu foncé
  AddPlanet(neptune);

  // Lune
  const OldMoon moon_earth("Moon Earth", &planets_[3], {20, 0},
                        0.3844 * kMutispeed, 2.0f,
                        {200.f / 255, 200.f / 255, 200.f / 255, 1.f});
  // Gris clair
  AddMoon(moon_earth);

  for (auto& p : planets_) common::DrawObserverSubject::AddObserver(&p);
  for (auto& m : moons_) common::DrawObserverSubject::AddObserver(&m);
}

void OldSolarSystem::Update(const float dt) {
  for (auto& p : planets_) p.Update(dt);
  for (auto& m : moons_) m.Update(dt);
}


void OldSolarSystem::OnGui() {
  {
    ImGui::Begin("Info Solar System");
    for (OldPlanet& planet : planets_) {
      ImGui::Text("Pos %s : %f %f", planet.name().c_str(), planet.position().x,
                  planet.position().y);
      ImGui::Text("Years %s : %d", planet.name().c_str(), planet.count_years());
    }
    for (OldMoon& moon : moons_)
      ImGui::Text("Pos %s : %f %f", moon.name().c_str(),
                  moon.position().x, moon.position().y);
    ImGui::End();
  }
}

}