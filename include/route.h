#pragma once

#include <cinder/gl/gl.h>

#include <queue>

namespace epidemic {

using glm::vec2;
using std::pair;
using std::queue;

class Route {
 public:
  enum Mode { kDeplete, kContinuous };

  static const pair<size_t, size_t> kQueueSize;

  // Constructors
  Route() = default;
  explicit Route(const vec2& bounds);
  Route(const vec2& bounds, const vec2& position);

  // Lifecycle
  void Update(float speed, const vec2& bounds);

  // Getters & Setters
  Mode GetMode() const;
  void SetMode(Mode mode);
  const vec2& GetPosition() const;
  void SetPosition(const vec2& position);
  const queue<vec2>& GetDestinations() const;
  void SetDestinations(const queue<vec2>& destinations);

 private:
  Mode mode_;

  vec2 position_;
  queue<vec2> destinations_;

  void FillDestinations(const vec2& bounds);
  void MoveTowards(float speed, const vec2& destination);
};

}  // namespace epidemic
