#include <individual.h>
#include <utils.h>

namespace epidemic {

using utils::GetRandomDeath;
using utils::GetRandomHealthiness;
using utils::GetRandomPosition;
using utils::GetRandomRecovery;
using utils::GetRandomSpread;
using utils::GetRandomWanderlust;

// Constructors

Individual::Individual(const vec2& bounds)
    : Individual(GetRandomHealthiness(), GetRandomWanderlust(),
                 Status::kUninfected, GetRandomPosition(bounds),
                 GetRandomSpread(), GetRandomRecovery(), GetRandomDeath()) {
}

Individual::Individual(float healthiness, float wanderlust, Status status,
                       const vec2& position, const vec2& spread,
                       const vec2& recovery, const vec2& death)
    : healthiness_(healthiness),
      wanderlust_(wanderlust),
      status_(status),
      position_(position),
      spread_(spread),
      recovery_(recovery),
      death_(death),
      is_sneezing_(false),
      id_(GetNextID()) {
}

// Getters & Setters

size_t Individual::GetID() const {
  return id_;
}

// Private Methods

size_t Individual::kNextID = 0;

size_t Individual::GetNextID() {
  return kNextID++;
}

}  // namespace epidemic