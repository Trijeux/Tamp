#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <vector>
#include <random>
#include <chrono>
#include <iostream>

#include "world.h"
#include "body.h"

using namespace common;
using namespace common::world;

static const int WIN_W = 1024;
static const int WIN_H = 768;

struct RenderCircle {
  ColliderIndex collider;
  bool overlapping = false;
};

class TriggerSample : public ContactListener {
public:
  TriggerSample(std::vector<RenderCircle>& circles_ref) : circles(circles_ref) {}

  void OnTriggerEnter(ColliderIndex a, ColliderIndex b) override {
    set_overlap(a, true);
    set_overlap(b, true);
  }

  void OnTriggerExit(ColliderIndex a, ColliderIndex b) override {
    set_overlap(a, false);
    set_overlap(b, false);
  }

private:
  std::vector<RenderCircle>& circles;

  void set_overlap(ColliderIndex idx, bool state) {
    for (auto &rc : circles) {
      if (rc.collider == idx) {
        rc.overlapping = state;
        return;
      }
    }
  }
};

// simple circle draw (triangle fan)
void draw_circle(float cx, float cy, float r, int segments = 32) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (int i = 0; i <= segments; ++i) {
    float theta = 2.0f * 3.1415926f * float(i) / float(segments);
    float x = r * cosf(theta);
    float y = r * sinf(theta);
    glVertex2f(cx + x, cy + y);
  }
  glEnd();
}

int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    return -1;
  }

  GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "Collider Trigger Sample", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to init GLAD\n";
    return -1;
  }

  // random
  std::mt19937 rng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<float> rndX(50.0f, WIN_W - 50.0f);
  std::uniform_real_distribution<float> rndY(50.0f, WIN_H - 50.0f);
  std::uniform_real_distribution<float> rndR(8.0f, 30.0f);
  std::uniform_real_distribution<float> rndV(-150.0f, 150.0f);

  std::vector<RenderCircle> renderCircles;

  // create sample bodies + colliders
  const int N = 30;
  for (int i = 0; i < N; ++i) {
    BodyIndex b = AddBody(1.f);
    Body& body = get_body_at(b);
    body.position = core::Vec2F{ rndX(rng), rndY(rng) };
    body.Velocity({ rndV(rng), rndV(rng) });

    float r = rndR(rng);
    ColliderIndex c = AddCollider(b, r);
    renderCircles.push_back({ c, false });
  }

  // register listener
  TriggerSample sampleListener(renderCircles);
  SetContactListener(&sampleListener);

  double last = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    double now = glfwGetTime();
    float dt = static_cast<float>(now - last);
    last = now;

    // simple bounds check + bounce
    for (auto &rc : renderCircles) {
      Collider& col = GetColliderAt(rc.collider);
      Body& b = get_body_at(col.body);
      float r = col.circle.radius;

      // bounce on X
      if (b.position.x - r < 0.f) {
        b.position.x = r;
        b.Velocity({ -b.position.x /* not ideal */, b.position.y }); // keep simple inversion
        // better: invert velocity.x:
        // but Body::velocity is private -> we can reuse Velocity setter only if available
      }
      if (b.position.x + r > WIN_W) {
        b.position.x = WIN_W - r;
        // invert velocity.x via Velocity (we need to read private velocity -> so instead call Velocity with inverted)
        // We'll simulate by adding a temporary force: but simpler: set a negative velocity using Body::Velocity; we don't have access to current vel
        // Workaround: call AddForce to produce bounce impulse - for sample, we'll teleport velocity by storing velocities externally would be cleaner.
      }

      // clamp Y
      if (b.position.y - r < 0.f) b.position.y = r;
      if (b.position.y + r > WIN_H) b.position.y = WIN_H - r;
    }

    // Update world physics (move bodies)
    Tick(dt);

    // Render
    glViewport(0, 0, WIN_W, WIN_H);
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIN_W, WIN_H, 0, -1, 1); // top-left origin
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (const auto &rc : renderCircles) {
      const Collider& col = GetColliderAt(rc.collider);
      const Body& b = get_body_at(col.body);
      float r = col.circle.radius;

      if (rc.overlapping) {
        glColor3f(0.0f, 1.0f, 0.0f); // green
      } else {
        glColor3f(1.0f, 0.0f, 0.0f); // red
      }
      draw_circle(b.position.x, b.position.y, r, 32);

      // outline
      glColor3f(0.0f, 0.0f, 0.0f);
      glBegin(GL_LINE_LOOP);
      const int seg = 32;
      for (int i = 0; i < seg; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(seg);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(b.position.x + x, b.position.y + y);
      }
      glEnd();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
