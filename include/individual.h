#pragma once

#include <cinder/gl/gl.h>
#include <route.h>

#include <queue>

namespace epidemic {

using glm::vec2;

class Individual {
 public:
  enum class Status {
    kUninfected,
    kAsymptomatic,
    kSymptomatic,
    kDying,
    kDead,
    kRecovered
  };

  explicit Individual(const vec2& bounds);
  Individual(const vec2& bounds, Status status);
  Individual(float healthiness, float wanderlust, const vec2& bounds);
  Individual(float healthiness, float wanderlust, const vec2& bounds,
             Status status);
  Individual(float healthiness, float wanderlust, const vec2& bounds,
             Status status, const vec2& position, const vec2& spread,
             const vec2& recovery, const vec2& death);

  void CheckAndBecomeInfected(const Individual& by);

  void Update(const vec2& bounds);
  void Draw(const vec2& offset) const;

  size_t GetID() const;
  Status GetStatus() const;

  const vec2& GetPosition() const;
  void SetPosition(const vec2& position);
  void SetDestinations(const queue<vec2>& destinations);
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
  static size_t kNextID;
  static size_t GetNextID();

  size_t id_;
  bool is_sneezing_;
  float healthiness_;
  Status status_;
  vec2 spread_, recovery_, death_;

  float wanderlust_;
  Route route_;

  void UpdateSneezeAndSymptoms();
  // TODO Non instantaneous recovery
  void RecoverOrDie();
};

}  // namespace epidemic