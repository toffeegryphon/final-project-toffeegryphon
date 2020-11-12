#pragma once

#include <location.h>

namespace epidemic {

class City : private Location {
 public:
  explicit City(const vec2& bounds);

  // Getters & Setters
  const vec2& GetBounds() const override;
  const vector<Individual>& GetIndividuals() const override;
};

}  // namespace epidemic
