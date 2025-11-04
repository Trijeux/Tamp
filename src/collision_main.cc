#include "engine/engine.h"
#include "engine/window.h"
#include "world.h"
#include "body.h"
#include <vector>
#include <random>
#include <imgui.h>

#include "engine/gui.h"
#include "engine/renderer.h"

// ------------------- COLLIDER SYSTEM -------------------
namespace collider {

struct Circle {
  common::world::BodyIndex                    body;
    common::world::ColliderIndex collider;
    float                        radius;
    float                        r, g, b, a;
};

class ColliderSystem final : public common::SystemInterface,
                             public common::OnGuiInterface,
                             public common::DrawInterface {
public:
    void Begin() override {
        listener_.circles = &circles_;
        common::SetContactListener(&listener_);

        std::uniform_real_distribution<float> posX(50.f, 800.f - 50.f);
        std::uniform_real_distribution<float> posY(50.f, 600.f - 50.f);
        std::uniform_real_distribution<float> velDist(-1.f, 1.f);
        std::uniform_real_distribution<float> radDist(minRadius_, maxRadius_);

        for (int i = 0; i < circleCount_; ++i) {
            float                    radius = radDist(rng_);
            common::world::BodyIndex b = common::world::AddBody(1.f);
            auto&                    body = common::world::get_body_at(b);
            body.position = { posX(rng_), posY(rng_) };
            body.Velocity({ velDist(rng_) * maxSpeed_, velDist(rng_) * maxSpeed_ });

            common::world::ColliderIndex c = common::world::AddCollider(b, radius);

            circles_.push_back({b, c, radius, 1.f, 0.f, 0.f, 1.f});
        }
    }

    void End() override {}

    void Update(float dt) override {
        for (auto& c : circles_) {
            auto& body = common::world::get_body_at(c.body);
            auto vel = body.Velocity();
            float r = c.radius;

            // rebond sur les bords
            if (body.position.x - r < 0.f) { body.position.x = r; vel.x = std::abs(vel.x); body.Velocity(vel);}
            if (body.position.x + r > 800.f) { body.position.x = 800.f - r; vel.x = -std::abs(vel.x); body.Velocity(vel);}
            if (body.position.y - r < 0.f) { body.position.y = r; vel.y = std::abs(vel.y); body.Velocity(vel);}
            if (body.position.y + r > 600.f) { body.position.y = 600.f - r; vel.y = -std::abs(vel.y); body.Velocity(vel);}
        }

        common::world::Tick(dt);
    }

    void FixedUpdate() override {}

    void Draw() override {
        for (auto& c : circles_) {
            auto& body = common::world::get_body_at(c.body);
            common::DrawCircle(body.position.x, body.position.y, c.radius,
                               SDL_FColor{c.r, c.g, c.b, c.a});
        }
    }

    void OnGui() override {
        ImGui::Begin("Collider Sample");
        ImGui::SliderInt("Nombre de cercles", &circleCount_, 5, 200);
        ImGui::SliderFloat("Rayon min", &minRadius_, 1.f, 100.f);
        ImGui::SliderFloat("Rayon max", &maxRadius_, 1.f, 100.f);
        ImGui::SliderFloat("Vitesse min", &minSpeed_, 1.f, 500.f);
        ImGui::SliderFloat("Vitesse max", &maxSpeed_, 1.f, 500.f);
        ImGui::End();
    }

private:
    std::vector<Circle> circles_;
    std::mt19937 rng_{std::random_device{}()};

    int circleCount_ = 30;
    float minRadius_ = 8.f;
    float maxRadius_ = 30.f;
    float minSpeed_ = 50.f;
    float maxSpeed_ = 150.f;

    class Listener : public common::world::ContactListener {
    public:
        std::vector<Circle>* circles = nullptr;
        void                 OnTriggerEnter(common::world::ColliderIndex a, common::world::ColliderIndex b) override {
            SetOverlap(a, true); SetOverlap(b, true);
        }
        void OnTriggerExit(common::world::ColliderIndex a, common::world::ColliderIndex b) override {
            SetOverlap(a, false); SetOverlap(b, false);
        }
    private:
        void SetOverlap(common::world::ColliderIndex idx, bool state) {
            for (auto& c : *circles)
                if (c.collider == idx)
                    c.r = state ? 0.f : 1.f,
                    c.g = state ? 1.f : 0.f,
                    c.b = 0.f;
        }
    } listener_;
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
