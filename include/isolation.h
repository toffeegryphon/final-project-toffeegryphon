#pragma once

#include <location.h>

namespace epidemic {

class Isolation : private Location {
 public:
  Isolation(const vec2& bounds);

  // Interaction
  void Add(const vector<Individual>& individuals) override;
  vector<Individual> ExtractIndividualsAt(const vec2& position) override;

  // Lifecycle

  void Update() override;
  void Draw(const vec2& offset) const override;

  // Getters & Setters

  Type GetType() const override;
  const vec2& GetBounds() const override;
  const vector<Individual>& GetIndividuals() const override;
};

}  // namespace epidemic
