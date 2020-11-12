#include <individual.h>
#include <utils.h>

namespace epidemic {

using glm::length;
using utils::GetRandom;
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

// Interaction

void Individual::CheckAndBecomeInfected(const Individual& by) {
  if (status_ == Status::kUninfected &&
      distance(by.GetPosition(), GetPosition()) <
          Configuration::kDefaultSneezeRadius &&
      GetRandom() > healthiness_) {
    status_ = Status::kAsymptomatic;
  }
}

// Lifecycle

void Individual::Update(const vec2& bounds) {
  if (status_ == Status::kDead) {
    return;
  }

  // TODO Maybe member variable
  float speed = length(bounds) / Configuration::kTraversalFrames;
  route_.Update(speed, bounds);

  if (status_ == Status::kUninfected) {
    return;
  }

  UpdateSneeze();
}

// Getters & Setters

size_t Individual::GetID() const {
  return id_;
}

Individual::Status Individual::GetStatus() const {
  return status_;
}

bool Individual::IsSneezing() const {
  return is_sneezing_;
}

const vec2& Individual::GetPosition() const {
  return route_.GetPosition();
}

void Individual::SetPosition(const vec2& position) {
  route_.SetPosition(position);
}

void Individual::SetHealthiness(float healthiness) {
  healthiness_ = healthiness;
}

void Individual::SetSpread(const vec2& spread_chance_roc) {
  spread_ = spread_chance_roc;
}

// Private Methods

size_t Individual::kNextID = 0;

size_t Individual::GetNextID() {
  return kNextID++;
}

void Individual::UpdateSneeze() {
  is_sneezing_ = GetRandom() < spread_.x;
  // TODO Possibly limit to at most some chance
  spread_.x += spread_.y;
}

}  // namespace epidemic