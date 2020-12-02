#include <configuration.h>

namespace epidemic {

vector<Configuration::BaseProperty*> Configuration::kProperties{};

void Configuration::PopulateProperties() {
  kProperties.push_back(&kIsolationCount);
  kProperties.push_back(&kSymptomaticThreshold);
  kProperties.push_back(&kDyingThreshold);
  kProperties.push_back(&kSneezeRadius);

  kProperties.push_back(&kSpreadChanceRange);
  kProperties.push_back(&kSpreadInfectedROCRange);
  kProperties.push_back(&kSpreadRecoveredROCRange);
}

Configuration::Property<int> Configuration::kIsolationCount{
    VType::kInt, "Isolation Count", 3, pair<int, int>(1, 5)};

vec2 Configuration::kWindowSize{1200, 800};

// Individual
Configuration::Property<float> Configuration::kSymptomaticThreshold{
    VType::kFloat, "Symptomatic Threshold", 0.5f,
    pair<float, float>(0.0f, 1.0f), "%.1f"};
Configuration::Property<float> Configuration::kDyingThreshold{
    VType::kFloat, "Dying Threshold", 0.5f, pair<float, float>(0.0f, 1.0f),
    "%.1f"};
Configuration::Property<float> Configuration::kSneezeRadius{
    VType::kFloat, "Sneeze Radius", 30.0f, pair<float, float>(10.0f, 100.0f),
    "%.f"};

Configuration::Property<vec2> Configuration::kSpreadChanceRange{
    VType::kVec2, "Spread Chance Min, Range", vec2(0.01, 0.3),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.001f, 1.0f)), "%.3f"};

Configuration::Property<vec2> Configuration::kSpreadInfectedROCRange{
    VType::kVec2, "Spread (Infected) Rate of Change Min, Range",
    vec2(0.0001, 0.0005),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0001f, 1.0f)), "%.4f"};

Configuration::Property<vec2> Configuration::kSpreadRecoveredROCRange{
    VType::kVec2, "Spread (Recovered) Rate of Change Min, Range",
    vec2(-0.0005, -0.0001),
    pair<vec2, vec2>(vec2(-1.0f, 0.0f), vec2(0.0001f, 1.0f)), "%.4f"};

vec2 Configuration::kDefaultRecoveryChanceRange{0.00, 0.000001};
vec2 Configuration::kDefaultRecoveryROCRange{0.000001, 0.000001};

vec2 Configuration::kDefaultDeathChanceRange{0.0000001, 0.00001};
vec2 Configuration::kDefaultDeathROCRange{0.00000, 0.0000001};

vec2 Configuration::kDefaultHealthinessRange{-0.5, 0.3};
vec2 Configuration::kDefaultWanderlustRange{0.9, 3};
}  // namespace epidemic