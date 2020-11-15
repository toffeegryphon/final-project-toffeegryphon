#include <individual.h>
#include <utils.h>

namespace epidemic {

using ci::gl::color;
using ci::gl::drawSolidCircle;
using glm::length;
using utils::GetRandom;
using utils::GetRandomDeath;
using utils::GetRandomHealthiness;
using utils::GetRandomInRange;
using utils::GetRandomPosition;
using utils::GetRandomRecovery;
using utils::GetRandomSpread;
using utils::GetRandomWanderlust;
using utils::IsInLocation;

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

  if (!IsInLocation(route_.GetPosition(), Location::Data{bounds, vec2()})) {
    route_.Update(speed * Configuration::kReturnFactor, bounds);
    return;
  }

  route_.Update(speed, bounds);

  if (status_ == Status::kUninfected) {
    return;
  }

  UpdateSneezeAndSymptoms();

  if (status_ == Status::kRecovered) {
    return;
  }

  RecoverOrDie();
}

void Individual::Draw(const vec2& offset) const {
  switch (status_) {
    case Status::kUninfected:
    case Status::kAsymptomatic:
      color(0, 0, 1);
      break;
    case Status::kSymptomatic:
      color(1, 1, 0);
      break;
    case Status::kDying:
      color(1, 0, 0);
      break;
    case Status::kDead:
      color(1, 1, 1);
      break;
    case Status::kRecovered:
      color(0, 1, 0);
      break;
  }
  drawSolidCircle(GetPosition() + offset,
                  Configuration::kDefaultIndividualRadius);
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

void Individual::SetDestinations(const queue<vec2>& destinations) {
  route_.SetDestinations(destinations);
}

void Individual::SetHealthiness(float healthiness) {
  healthiness_ = healthiness;
}

const vec2& Individual::GetSpread() const {
  return spread_;
}

void Individual::SetSpread(const vec2& spread_chance_roc) {
  spread_ = spread_chance_roc;
}

const vec2& Individual::GetRecovery() const {
  return recovery_;
}

void Individual::SetRecovery(const vec2& recovery_chance_roc) {
  recovery_ = recovery_chance_roc;
}

const vec2& Individual::GetDeath() const {
  return death_;
}

void Individual::SetDeath(const vec2& death_chance_roc) {
  death_ = death_chance_roc;
}

// Operators

bool Individual::operator==(const Individual& rhs) const {
  return id_ == rhs.id_;
}

bool Individual::operator!=(const Individual& rhs) const {
  return !(rhs == *this);
}

// Private Methods

size_t Individual::kNextID = 0;

size_t Individual::GetNextID() {
  return kNextID++;
}

void Individual::UpdateSneezeAndSymptoms() {
  is_sneezing_ = GetRandom() < spread_.x;
  // TODO Possibly limit to at most some chance
  spread_.x += spread_.y;

  if (status_ != Status::kRecovered &&
      spread_.x > Configuration::kSymptomaticThreshold) {
    status_ = Status::kSymptomatic;
  }
}

void Individual::RecoverOrDie() {
  // Check

  /*
   * |--Recover--|--Nothing--|--Death--|
   * 0                                 1
   *    ^ Pass Recover check
   *            Pass Death check ^
   *      Fail checks ^
   *
   * |------Recover------|
   *                |---Death---|
   * 0                          1
   *                |--^-| Considered recovered
   */

  float state = GetRandom();
  if (state <= recovery_.x) {
    status_ = Status::kRecovered;
    spread_.y =
        GetRandomInRange(Configuration::kDefaultSpreadRecoveredROCRange);
  } else if (state > 1 - death_.x) {
    status_ = Status::kDead;
  } else if (death_.x > Configuration::kDyingThreshold) {
    status_ = Status::kDying;
  }

  // Increment
  recovery_.x += recovery_.y;
  death_.x += death_.y;
}

}  // namespace epidemic