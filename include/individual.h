#pragma once

#include <cinder/gl/gl.h>

#include <queue>

namespace epidemic {

using glm::vec2;
using std::queue;

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

  Individual(const vec2& bounds);
  Individual(const vec2& bounds, Status status);
  Individual(float healthiness, float wanderlust, const vec2& bounds);
  Individual(float healthiness, float wanderlust, const vec2& bounds,
             Status status);
  Individual(float healthiness, float wanderlust, Status status,
             const vec2& position, const vec2& spread, const vec2& recovery,
             const vec2& death);

  void Infect(const Individual& infector);

  void Update();
  void Draw(const vec2& offset) const;

  size_t GetID() const;
  const vec2& GetPosition() const;
  bool IsSneezing() const;

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
  queue<vec2> destinations_;
  vec2 position_;

  void Move();
  void Sneeze();
  // TODO Non instantaneous recovery
  void RecoverOrDie();
};

}  // namespace epidemic