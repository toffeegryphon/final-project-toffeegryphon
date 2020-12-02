#pragma once

#include <individual.h>
#include <location.h>

namespace epidemic {

using std::unordered_map;

class Isolation : private Location {
 public:
  explicit Isolation(const vec2& bounds);
  Isolation(const vec2& bounds, size_t capacity);

  // Interaction
  vector<Individual*> Add(const vector<Individual*>& individuals) override;
  vector<Individual*> ExtractIndividualsAt(const vec2& position) override;

  // Lifecycle

  void Update() override;
  void Draw(const vec2& offset) const override;

  // Getters & Setters

  Type GetType() const override;
  const vec2& GetBounds() const override;
  const vector<Individual*>& GetIndividuals() const override;

 private:
  size_t capacity_;
  unordered_map<size_t, size_t> frames_warded_;

  void UpdateAdmission();
  void Discharge(vector<Individual*>* individuals);

  void UpdateSpread();
};

}  // namespace epidemic
