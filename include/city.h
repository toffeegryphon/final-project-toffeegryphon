#pragma once

#include <location.h>

namespace epidemic {

class City : private Location {
 public:
  explicit City(const vec2& bounds);
  City(const vec2& bounds, size_t population_size, size_t sick_count);
  City(const vec2& bounds, const vector<Individual>& individuals);

  // Interaction
  void Add(const vector<Individual>& individuals);
  vector<Individual> ExtractIndividualsAt(const vec2& position);

  // Lifecycle
  void Update() override;
  void Draw(const vec2& offset) const override;

  // Getters & Setters
  const vec2& GetBounds() const override;
  const vector<Individual>& GetIndividuals() const override;

 private:
  void UpdateSpread();
};

}  // namespace epidemic
