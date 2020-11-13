#pragma once

#include <individual.h>

#include <vector>

namespace epidemic {

using std::vector;

/*
 * TODO Optimization:
 * To consider whether copying is the best idea
 * Maybe just keep the pointer in app, then have an internal location field
 * that updates.
 */

class Hand {
 public:
  // Interaction

  void Add(const Individual& individual);
  void Add(const vector<Individual>& individuals);
  vector<Individual> Release();

  // Lifecycle
  void Update(const vec2& position);
  void Draw();

  // Getters & Setters
  const vector<Individual>& GetIndividuals() const;

 private:
  vector<Individual> individuals_;
};

}  // namespace epidemic
