#include <utils.h>

namespace epidemic {

namespace utils {

using std::rand;

float GetRandom() {
  return static_cast<float>(rand()) / RAND_MAX;
}

vec2 GetRandomPosition(const vec2& bounds) {
  return vec2{GetRandom() * bounds.x, GetRandom() * bounds.y};
}

float GetRandomInRange(const vec2& start_range) {
  return start_range.x + GetRandom() * start_range.y;
}

float GetRandomInRange(float end) {
  return GetRandomInRange(vec2{0, end});
}

float GetRandomHealthiness() {
  return GetRandomInRange(cfg::kDefaultHealthinessRange);
}

vec2 GetRandomChanceROC(const vec2& chance_range, const vec2& roc_range) {
  return vec2{GetRandomInRange(chance_range), GetRandomInRange(roc_range)};
}

vec2 GetRandomSpread() {
  return GetRandomChanceROC(cfg::kSpreadChanceRange.value,
                            cfg::kSpreadInfectedROCRange.value);
}

vec2 GetRandomRecovery() {
  return GetRandomChanceROC(cfg::kRecoveryChanceRange.value,
                            cfg::kRecoveryROCRange.value);
}

vec2 GetRandomDeath() {
  return GetRandomChanceROC(cfg::kDeathChanceRange.value,
                            cfg::kDeathROCRange.value);
}

float DistanceX(const Individual& first, const Individual& second) {
  return fabs(first.GetPosition().x - second.GetPosition().x);
}

bool CompareX(const Individual* first, const Individual* second) {
  return first->GetPosition().x < second->GetPosition().x;
}

bool IsInLocation(const vec2& position, const Location::Data& data) {
  return (
      data.offset.x < position.x && position.x < data.offset.x + data.size.x &&
      data.offset.y < position.y && position.y < data.offset.y + data.size.y);
}

vector<Individual*> ToPointers(vector<Individual>* source) {
  vector<Individual*> individual_ptrs;
  individual_ptrs.reserve(source->size());
  for (Individual& individual : *source) {
    individual_ptrs.push_back(&individual);
  }
  return individual_ptrs;
}

}  // namespace utils

}  // namespace epidemic
