#include <configuration.h>

namespace epidemic {

vector<Configuration::BaseProperty*> Configuration::kProperties{};

void Configuration::PopulateProperties() {
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

  kProperties.push_back(&kPopulationSize);
  kProperties.push_back(&kStartingSickCount);

  kProperties.push_back(&kIsolationCount);
  kProperties.push_back(&kIsolationCapacity);
  kProperties.push_back(&kIsolationWillSpread);
  kProperties.push_back(&kIsolationRecoveryFactor);
  kProperties.push_back(&kIsolationDeathFactor);
}

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
    VType::kVec2, "Spread Chance (Infected) Rate of Change Min, Range",
    vec2(0.0001, 0.0005),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0001f, 1.0f)), "%.4f"};

Configuration::Property<vec2> Configuration::kSpreadRecoveredROCRange{
    VType::kVec2, "Spread Chance (Recovered) Rate of Change Min, Range",
    vec2(-0.0005, -0.0001),
    pair<vec2, vec2>(vec2(-1.0f, 0.0f), vec2(0.0001f, 1.0f)), "%.4f"};

Configuration::Property<vec2> Configuration::kRecoveryChanceRange{
    VType::kVec2, "Recovery Chance Min, Range", vec2(0.0f, 0.000001),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};

Configuration::Property<vec2> Configuration::kRecoveryROCRange{
    VType::kVec2, "Recovery Chance Rate of Change Min, Range",
    vec2(0.000001, 0.000001),
    pair<vec2, vec2>(vec2(-1.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};

Configuration::Property<vec2> Configuration::kDeathChanceRange{
    VType::kVec2, "Death Chance Min, Range", vec2(0.0000001, 0.00001),
    pair<vec2, vec2>(vec2(0.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};

Configuration::Property<vec2> Configuration::kDeathROCRange{
    VType::kVec2, "Death Chance Rate of Change Min, Range",
    vec2(0.0f, 0.0000001),
    pair<vec2, vec2>(vec2(-1.0f, 1.0f), vec2(0.0000001f, 1.0f)), "%.7f"};

vec2 Configuration::kDefaultHealthinessRange{-0.5, 0.3};
vec2 Configuration::kDefaultWanderlustRange{0.9, 3};

// City
Configuration::Property<int> Configuration::kPopulationSize{
    VType::kInt, "Population Size", 30, pair<int, int>(1, 200)};
// TODO This probably requires testing
Configuration::Property<int> Configuration::kStartingSickCount{
    VType::kInt, "Starting Sick Count", 1,
    pair<int, int>(1, kPopulationSize.value)};
// TODO Need to fix overshooting or find some way to cap it

Configuration::Property<int> Configuration::kIsolationCount{
    VType::kInt, "Isolation Count", 3, pair<int, int>(1, 5)};
Configuration::Property<int> Configuration::kIsolationCapacity{
    VType::kInt, "Isolation Capacity", 3, pair<int, int>(1, 10)};
Configuration::Property<bool> Configuration::kIsolationWillSpread{
    VType::kBool, "Isolation Will Perform Spread Check if Sneeze", true,
    pair<bool, bool>(false, true)};
Configuration::Property<float> Configuration::kIsolationRecoveryFactor{
    VType::kFloat, "Isolation Recovery Chance ROC Factor", 2.0f,
    pair<float, float>(0.0f, 100.0f), "%.1f"};
Configuration::Property<float> Configuration::kIsolationDeathFactor{
    VType::kFloat, "Isolation Death Chance ROC Factor", 0.5f,
    pair<float, float>(0.0f, 1.0f), "%.2f"};
}  // namespace epidemic