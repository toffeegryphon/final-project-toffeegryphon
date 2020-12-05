#pragma once

#include <cinder/gl/gl.h>

#include <queue>

namespace epidemic {

using glm::vec2;
using std::pair;
using std::queue;

class Route {
 public:
  /**
   * Destination generation modes
   */
  enum Mode { kDeplete, kContinuous };

  /**
   * Pair of minimum, maximum number of destinations the route can contain
   */
  static const pair<size_t, size_t> kQueueSize;

  // Constructors
  Route() = default;

  /**
   * Initializes route with maximum number of random destinations within bounds,
   * and a random default starting position
   *
   * @param bounds To generate destinations within
   */
  explicit Route(const vec2& bounds);

  /**
   * Initializes route with maximum number of random destinations within bounds,
   * and given starting position
   *
   * @param bounds To generate destinations within
   * @param position To set as starting position
   */
  Route(const vec2& bounds, const vec2& position);

  // Lifecycle

  /**
   * Updates position towards first destination by speed, and replenishes to
   * maximum random destinations within bounds if mode is kContinuous and
   * destinations count falls to minimum
   *
   * @param speed To change position by
   * @param bounds To generate destinations within
   */
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

  /**
   * Replenishes to maximum random destinations within bounds
   * @param bounds To generate destinations within
   */
  void FillDestinations(const vec2& bounds);

  /**
   * Updates position towards destination by speed
   *
   * @param speed To change position by
   * @param destination To be reached
   */
  void MoveTowards(float speed, const vec2& destination);
};

}  // namespace epidemic
