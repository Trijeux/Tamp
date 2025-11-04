#include "solar_system.h"
#include "engine/engine.h"
#include "engine/window.h"

int main() {
  common::WindowConfig config;
  config.height = solar::kHeight;
  config.width = solar::kWidth;
  common::SetWindowConfig(config);

  solar::CreateSolarSystem();

  common::RunEngine();
}