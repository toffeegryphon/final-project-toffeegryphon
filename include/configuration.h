#pragma once

#include <cinder/gl/gl.h>

#include <utility>

namespace epidemic {

using glm::vec2;
using std::move;
using std::pair;
using std::string;
using std::vector;

namespace cfg {

enum Type { kRange, kOption };
enum VType { kInt, kFloat, kVec2, kBool };

struct BaseProperty {
 public:
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
        value_range(move(value_range)),
        format(move(format)){};

  string label;
  T value;
  // For vec2, pair.first is used for range and pair.second is used for step
  // and power
  pair<T, T> value_range;
  vector<T> value_options;
  string format;
};

extern vector<BaseProperty*> kProperties;
// TODO Maybe should combine Getter and populate into one function
/**
 * Adds all properties defined within this function to kProperties
 */
void PopulateProperties();

// https://stackoverflow.com/questions/11055802/static-and-extern-global-variables-in-c-and-c#

// App
extern vec2 kWindowSize;

// Individual

// General
extern Property<float> kSymptomaticThreshold;
extern Property<float> kDyingThreshold;
extern Property<float> kSneezeRadius;

// Minimum Value, Range

// Spread
extern Property<vec2> kSpreadChanceRange;
extern Property<vec2> kSpreadInfectedROCRange;
extern Property<vec2> kSpreadRecoveredROCRange;

// Recovery
extern Property<vec2> kRecoveryChanceRange;
extern Property<vec2> kRecoveryROCRange;

// Death
extern Property<vec2> kDeathChanceRange;
extern Property<vec2> kDeathROCRange;

extern vec2 kDefaultHealthinessRange;
// TODO Currently unused
extern vec2 kDefaultWanderlustRange;

static constexpr float kDefaultIndividualRadius = 10;

// Frames to travel from origin to opposite end of bounds
static constexpr float kTraversalFrames = 1000;
static constexpr float kReturnFactor = 10.0f;

// Location
extern Property<int> kSpreadCheckFrequency;

// City
extern Property<int> kPopulationSize;
extern Property<int> kStartingSickCount;

// Isolation
extern Property<int> kIsolationCount;
extern Property<int> kIsolationCapacity;
extern Property<bool> kIsolationWillSpread;
extern Property<float> kIsolationRecoveryFactor;
extern Property<float> kIsolationDeathFactor;
extern Property<int> kIsolationDetectionFrames;

}  // namespace cfg

}  // namespace epidemic
