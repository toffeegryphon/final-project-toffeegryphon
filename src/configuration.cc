#include <configuration.h>

namespace epidemic {

namespace cfg {

void PopulateProperties() {
  // Individual
  kProperties.push_back(&kSymptomaticThreshold);
  kProperties.push_back(&kDyingThreshold);
  kProperties.push_back(&kSneezeRadius);

  kProperties.push_back(&kSpreadChanceRange);
  kProperties.push_back(&kSpreadInfectedROCRange);
  kProperties.push_back(&kSpreadRecoveredROCRange);

  kProperties.push_back(&kRecoveryChanceRange);
  kProperties.push_back(&kRecoveryROCRange);

  kProperties.push_back(&kDeathChanceRange);
  kProperties.push_back(&kDeathROCRange);

  // Location
  kProperties.push_back(&kSpreadCheckFrequency);

  // City
  kProperties.push_back(&kPopulationSize);
  kProperties.push_back(&kStartingSickCount);

  // Isolation
  kProperties.push_back(&kIsolationCount);
  kProperties.push_back(&kIsolationCapacity);
  kProperties.push_back(&kIsolationWillSpread);
  kProperties.push_back(&kIsolationRecoveryFactor);
  kProperties.push_back(&kIsolationDeathFactor);
  kProperties.push_back(&kIsolationDetectionFrames);
}

vector<BaseProperty*> kProperties{};

// App
vec2 kWindowSize{1200, 800};

// Individual

Property<float> kSymptomaticThreshold{VType::kFloat, "Symptomatic Threshold",
                                      0.5f, pair<float, float>(0.0f, 1.0f),
                                      "%.1f"};

Property<float> kDyingThreshold{VType::kFloat, "Dying Threshold", 0.5f,
                                pair<float, float>(0.0f, 1.0f), "%.1f"};
Property<float> kSneezeRadius{VType::kFloat, "Sneeze Radius", 30.0f,
                              pair<float, float>(10.0f, 100.0f), "%.f"};

// Minimum Value, Range
Property<vec2> kSpreadChanceRange{
    VType::kVec2, "Spread Chance Min, Range", vec2(0.010, 0.010),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.001f, 1.0f)), "%.3f"};
Property<vec2> kSpreadInfectedROCRange{
    VType::kVec2, "Spread Chance (Infected) Rate of Change Min, Range",
    vec2(0.0001, 0.0005),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0001f, 1.0f)), "%.4f"};
Property<vec2> kSpreadRecoveredROCRange{
    VType::kVec2, "Spread Chance (Recovered) Rate of Change Min, Range",
    vec2(-0.0005, -0.0001),
    pair<vec2, vec2>(vec2(-1.0f, 0.0f), vec2(0.0001f, 1.0f)), "%.4f"};

Property<vec2> kRecoveryChanceRange{
    VType::kVec2, "Recovery Chance Min, Range", vec2(0.0f, 0.000001),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};
Property<vec2> kRecoveryROCRange{
    VType::kVec2, "Recovery Chance Rate of Change Min, Range",
    vec2(0.000001, 0.000001),
    pair<vec2, vec2>(vec2(-1.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};

Property<vec2> kDeathChanceRange{
    VType::kVec2, "Death Chance Min, Range", vec2(0.0000001, 0.00001),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};
Property<vec2> kDeathROCRange{
    VType::kVec2, "Death Chance Rate of Change Min, Range",
    vec2(0.0f, 0.0000001),
    pair<vec2, vec2>(vec2(-1.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};

vec2 kDefaultHealthinessRange{-0.5, 0.3};
// TODO Currently unused
vec2 kDefaultWanderlustRange{0.9, 3};

// Location
Property<int> kSpreadCheckFrequency{
    VType::kInt, "Spread Check Frequency (frames)", 10, pair<int, int>(1, 200)};

// City
Property<int> kPopulationSize{VType::kInt, "Population Size", 30,
                              pair<int, int>(1, 200)};
Property<int> kStartingSickCount{VType::kInt, "Starting Sick Count", 1,
                                 pair<int, int>(1, kPopulationSize.value)};
// TODO Need to fix overshooting or find some way to cap it
// Probably can have a normalize function called before starting

// Isolation
Property<int> kIsolationCount{VType::kInt, "Isolation Count", 3,
                              pair<int, int>(1, 5)};
Property<int> kIsolationCapacity{VType::kInt, "Isolation Capacity", 3,
                                 pair<int, int>(1, 10)};
Property<bool> kIsolationWillSpread{
    VType::kBool, "Isolation Will Perform Spread Check if Sneeze", true,
    pair<bool, bool>(false, true)};
Property<float> kIsolationRecoveryFactor{
    VType::kFloat, "Isolation Recovery Chance ROC Factor", 10.0f,
    pair<float, float>(0.0f, 100.0f), "%.1f"};
Property<float> kIsolationDeathFactor{VType::kFloat,
                                      "Isolation Death Chance ROC Factor", 0.5f,
                                      pair<float, float>(0.0f, 1.0f), "%.2f"};
Property<int> kIsolationDetectionFrames{
    VType::kInt, "Frames required to detect symptoms from onset", 10,
    pair<int, int>(0, 1000)};

}  // namespace cfg

}  // namespace epidemic