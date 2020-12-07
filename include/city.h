#pragma once

#include <individual.h>
#include <location.h>

namespace epidemic {

class City : private Location {
 public:
  // Constructors

  /**
   * Initializes City with type of City, size of 0, and no individuals
   */
  City();

  /**
   * Initializes City with type of City, size of bounds, and no individuals
   *
   * @param bounds Of this
   */
  explicit City(const vec2& bounds);

  /**
   * Initializes City with type of City, size of bounds, and given individuals
   *
   * @param bounds Of this
   * @param individuals To be added to this
   */
  City(const vec2& bounds, const vector<Individual*>& individuals);

  // Interaction

  /**
   * Adds individuals to this and returns an empty vector of Individual pointers
   *
   * @param individuals To be added to this
   * @return Empty vector of Individual pointers
   */
  vector<Individual*> Add(const vector<Individual*>& individuals) override;

  /**
   * Removes and returns all Individuals within the radius (of an Individual) of
   * position
   *
   * @param position To remove Individuals from
   * @return Individuals removed
   */
  vector<Individual*> ExtractIndividualsAt(const vec2& position) override;

  // Lifecycle

  /**
   * Updates each individual, and checks and performs spreading
   */
  void Update() override;

  /**
   * Draws this with a background and all the Individuals within this at offset
   *
   * @param offset From origin to draw this at
   */
  void Draw(const vec2& offset) const override;

  // Getters & Setters
  Location::Type GetType() const override;
  const vec2& GetBounds() const override;
  const vector<Individual*>& GetIndividuals() const override;

 private:
  /**
   * Checks for sneezing individuals and performs spreading of disease between
   * individuals within cfg::kSneezeRadius range
   */
  void UpdateSpread();
};

}  // namespace epidemic
