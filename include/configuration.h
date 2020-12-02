#pragma once

#include <cinder/gl/gl.h>

#include <utility>

namespace epidemic {

using glm::vec2;
using std::move;
using std::pair;
using std::string;
using std::vector;

class Configuration {
 public:
  enum Type { kRange, kOption };
  enum VType { kInt, kFloat, kVec2, kBool };

  // TODO Test somehow
  struct BaseProperty {
   public:
    // TODO move into .cc
    BaseProperty(Type type, VType v_type) : type(type), v_type(v_type){};
    virtual ~BaseProperty() = default;
    Type type;
    VType v_type;
  };

  template <typename T>
  struct Property : public BaseProperty {
    Property<T>(VType v_type, string label, T default_value,
                pair<T, T> value_range, string format = "")
        : BaseProperty{Type::kRange, v_type},
          label(move(label)),
          value(default_value),
          value_range(value_range),
          format(move(format)){};

    string label;
    T value;
    // For vec2, pair.first is used for range and pair.second is used for step
    // and power
    pair<T, T> value_range;
    vector<T> value_options;
    string format;
  };

  static vector<BaseProperty*> kProperties;
  // TODO Maybe should combine Getter and populator into one function
  static void PopulateProperties();

  // App
  static Property<int> kIsolationCount;

  static vec2 kWindowSize;

  // Individual

  static Property<float> kSymptomaticThreshold;
  static Property<float> kDyingThreshold;
  static Property<float> kSneezeRadius;

  // Minimum Value, Range

  // TODO Dragfloat
  static Property<vec2> kSpreadChanceRange;
  static Property<vec2> kSpreadInfectedROCRange;
  static Property<vec2> kSpreadRecoveredROCRange;

  static vec2 kDefaultRecoveryChanceRange;
  static vec2 kDefaultRecoveryROCRange;

  static vec2 kDefaultDeathChanceRange;
  static vec2 kDefaultDeathROCRange;

  static vec2 kDefaultHealthinessRange;
  static vec2 kDefaultWanderlustRange;

  static constexpr float kDefaultMovementRadius = 3;

  static constexpr float kDefaultIndividualRadius = 10;

  // Frames to travel from origin to opposite end of bounds
  static constexpr float kTraversalFrames = 1000;
  static constexpr float kReturnFactor = 10.0f;

  // City
  static constexpr size_t kDefaultPopulationSize = 30;
  static constexpr size_t kDefaultSickCount = 1;

  // Isolation
  static constexpr size_t kIsolationDefaultCapacity = 3;
  static constexpr bool kWillSpread = true;
  static constexpr float kIsolationRecoveryFactor = 2.0f;
  static constexpr float kIsolationDeathFactor = 0.5f;
};

}  // namespace epidemic
