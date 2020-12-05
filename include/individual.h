#pragma once

#include <cinder/gl/gl.h>
#include <location.h>
#include <route.h>

#include <queue>

namespace epidemic {

using glm::vec2;

class Individual {
 public:
  enum class Status {
    // Chance of spread 0
    kUninfected,
    // Chance of spread lower than kSymptomaticThreshold
    kAsymptomatic,
    // Chance of spread higher than kSymptomaticThreshold
    kSymptomatic,
    // Chance of death higher thank kDyingThreshold
    kDying,
    kDead,
    // Chance of spread decreasing
    kRecovering,
    // Chance of spread 0
    kRecovered
  };

  // Constructors

  /**
   * Initializes Individual with random position within bounds and default
   * Status of kUninfected, along with other attributes at random
   *
   * @param bounds To generate position within
   */
  explicit Individual(const vec2& bounds);

  /**
   * Initializes Individual with random position within bounds and given status
   * and other attributes at random
   *
   * @param bounds To generate position within
   * @param status To be set
   */
  Individual(const vec2& bounds, Status status);

  /**
   * Initializes Individual with given attributes
   * @param healthiness
   * @param wanderlust
   * @param bounds
   * @param status
   * @param position
   * @param spread
   * @param recovery
   * @param death
   */
  Individual(float healthiness, float wanderlust, const vec2& bounds,
             Status status, const vec2& position, const vec2& spread,
             const vec2& recovery, const vec2& death);

  // Interaction

  /**
   * Checks if by is within range, and infects this if healthiness check fails
   *
   * @param by Source that spread
   */
  void CheckAndBecomeInfected(const Individual& by);

  // Lifecycle

  /**
   * Updates this attributes, updates whether this is sneezing in this cycle,
   * and checks whether this begins recovery or dies
   *
   * @param bounds Of parent Location
   * @param location_type Of parent Location, influences proportion of attribute
   * updates
   */
  void Update(const vec2& bounds, Location::Type location_type);

  /**
   * Draws this at offset, with color depending on status
   *
   * @param offset From origin to draw this at
   */
  void Draw(const vec2& offset) const;

  // Getters & Setters

  size_t GetID() const;
  Status GetStatus() const;
  void SetStatus(Status status);

  // Route Related
  const vec2& GetPosition() const;
  void SetPosition(const vec2& position);
  const queue<vec2>& GetDestinations() const;
  void SetDestinations(const queue<vec2>& destinations);
  Route::Mode GetRouteMode() const;
  void SetRouteMode(Route::Mode mode);

  bool IsSneezing() const;

  void SetHealthiness(float healthiness);

  const vec2& GetSpread() const;
  void SetSpread(const vec2& spread_chance_roc);

  const vec2& GetRecovery() const;
  void SetRecovery(const vec2& recovery_chance_roc);

  const vec2& GetDeath() const;
  void SetDeath(const vec2& death_chance_roc);

  bool operator==(const Individual& rhs) const;
  bool operator!=(const Individual& rhs) const;

 private:
  // ID
  static size_t next_id_;
  static size_t GetNextID();

  size_t id_;
  bool is_sneezing_;
  float healthiness_;
  Status status_;
  vec2 spread_, recovery_, death_;

  float wanderlust_;
  Route route_;

  /**
   * Updates whether this will be sneezing in the current cycle and whether this
   * becomes kSymptomatic
   */
  void UpdateSneezeAndSymptoms();

  // TODO Non instantaneous recovery
  /**
   * Performs recover and death checks, influenced by location_type, and updates
   * status to recovering or death if either passes, updating spread chance and
   * rate of change (ROC) if recovering
   *
   * @param location_type Of parent Location
   */
  void RecoverOrDie(Location::Type location_type);
};

}  // namespace epidemic