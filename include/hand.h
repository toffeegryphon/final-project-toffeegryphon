#pragma once

#include <individual.h>

#include <vector>

namespace epidemic {

using std::vector;

class Hand {
 public:
  // Interaction

  /**
   * Adds single individual to this
   *
   * @param individual To be added
   */
  void Add(Individual* individual);

  /**
   * Adds all individuals to this
   *
   * @param individuals To be added
   */
  void Add(const vector<Individual*>& individuals);

  /**
   * Removes and returns all individuals in this
   *
   * @return All individuals in hand
   */
  vector<Individual*> Release();

  // Lifecycle

  /**
   * Updates position of all individuals in this to given position only
   *
   * @param position To be updated to
   */
  void Update(const vec2& position);

  /**
   * Draws each individual at their absolute positions
   */
  void Draw();

  // Getters & Setters

  const vector<Individual*>& GetIndividuals() const;

 private:
  vector<Individual*> individuals_;
};

}  // namespace epidemic
