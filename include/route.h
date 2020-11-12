#pragma once

#include <cinder/gl/gl.h>

#include <queue>

namespace epidemic {

using glm::vec2;
using std::queue;
using std::pair;

class Route {
 public:
  static const pair<size_t, size_t> kQueueSize;

  // Constructors
  Route() = default;
  explicit Route(const vec2& bounds);

  // Lifecycle
  vec2 Update(float speed, const vec2& bounds);

  // Getters & Setters
  const vec2& GetPosition() const;
  const queue<vec2>& GetDestinations() const;

 private:
  vec2 position_;
  queue<vec2> destinations_;

  void FillDestinations(const vec2& bounds);
};

}  // namespace epidemic
