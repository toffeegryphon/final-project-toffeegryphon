#include <individual.h>
#include <utils.h>

namespace epidemic {

using glm::length;
using utils::GetRandomDeath;
using utils::GetRandomHealthiness;
using utils::GetRandomPosition;
using utils::GetRandomRecovery;
using utils::GetRandomSpread;
using utils::GetRandomWanderlust;

// Constructors

Individual::Individual(const vec2& bounds)
    : Individual(bounds, Status::kUninfected) {
}

Individual::Individual(const vec2& bounds, Status status)
    : Individual(GetRandomHealthiness(), GetRandomWanderlust(), bounds, status,
                 GetRandomPosition(bounds), GetRandomSpread(),
                 GetRandomRecovery(), GetRandomDeath()) {
}

Individual::Individual(float healthiness, float wanderlust, const vec2& bounds,
                       Status status, const vec2& position, const vec2& spread,
                       const vec2& recovery, const vec2& death)
    : healthiness_(healthiness),
      wanderlust_(wanderlust),
      status_(status),
      route_(bounds, position),
      spread_(spread),
      recovery_(recovery),
      death_(death),
      is_sneezing_(false),
      id_(GetNextID()) {
}

// Lifecycle

void Individual::Update(const vec2& bounds) {
  if (status_ == Status::kDead) {
    return;
  }

  // TODO Maybe member variable
  float speed = length(bounds) / Configuration::kTraversalFrames;
  route_.Update(speed, bounds);
}

// Getters & Setters

size_t Individual::GetID() const {
  return id_;
}

const vec2& Individual::GetPosition() const {
  return route_.GetPosition();
}

// Private Methods

size_t Individual::kNextID = 0;

size_t Individual::GetNextID() {
  return kNextID++;
}

}  // namespace epidemic