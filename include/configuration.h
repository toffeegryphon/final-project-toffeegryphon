#pragma once

#include <cinder/gl/gl.h>

namespace epidemic {

using glm::vec2;

// TODO Possibly struct
class Configuration {
 public:
  static constexpr float kSymptomaticThreshold = 0.3f;
  static constexpr float kDyingThreshold = 0.5f;
  static constexpr float kDefaultSneezeRadius = 10.0f;

  // Minimum Value, Range

  static vec2 kDefaultSpreadChanceRange;
  static vec2 kDefaultSpreadInfectedROCRange;
  static vec2 kDefaultSpreadRecoveredROCRange;

  static vec2 kDefaultRecoveryChanceRange;
  static vec2 kDefaultRecoveryROCRange;

  static vec2 kDefaultDeathChanceRange;
  static vec2 kDefaultDeathROCRange;

  static vec2 kDefaultHealthinessRange;
  static vec2 kDefaultWanderlustRange;

  static constexpr float kDefaultMovementRadius = 3;

  static constexpr float kDefaultIndividualRadius = 10;
};

}  // namespace epidemic