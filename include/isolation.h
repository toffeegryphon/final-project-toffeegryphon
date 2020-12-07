#pragma once

#include <individual.h>
#include <location.h>

namespace epidemic {

using std::unordered_map;

class Isolation : private Location {
 public:
  // Constructors

  /**
   * Initializes Isolation ward with Type of kIsolation, size of bounds, and
   * capacity provided by cfg
   *
   * @param bounds Of this
   */
  explicit Isolation(const vec2& bounds);

  /**
   * Initializes Isolation ward with Type of kIsolation, size of bounds, and
   * capacity
   *
   * @param bounds Of this
   * @param capacity Of this
   */
  Isolation(const vec2& bounds, size_t capacity);

  // Interaction

  /**
   * Adds individuals to location up to capacity, and returns individuals that
   * are rejected when capacity has been exceeded, setting destinations of
   * admitted individuals to their beds
   *
   * @param individuals To try to add to this
   * @return Individuals that exceed capacity
   */
  vector<Individual*> Add(const vector<Individual*>& individuals) override;

  /**
   * Removes and returns all Individuals within the radius (of an Individual) of
   * position, and gives them random destinations
   *
   * @param position To remove Individuals from
   * @return Individuals removed
   */
  vector<Individual*> ExtractIndividualsAt(const vec2& position) override;

  // Lifecycle

  /**
   * Updates each Individual, sets their statuses to kSymptomatic if
   * cfg::kIsolationDetectionFrames frames have passed since first
   * infection since entering Isolation, and checks and sets spread if activated
   * in cfg::kIsolationWillSpread
   */
  void Update() override;

  /**
   * Draws this with a background and all the Individuals within this at offset
   *
   * @param offset From origin to draw this at
   */
  void Draw(const vec2& offset) const override;

  // Getters & Setters

  Type GetType() const override;
  const vec2& GetBounds() const override;
  const vector<Individual*>& GetIndividuals() const override;

 private:
  size_t capacity_;
  unordered_map<size_t, int> frames_warded_;

  /**
   * Sets Route of all Individuals to kDeplete Mode and sets their destinations
   * to the bed position corresponding to their index
   */
  void UpdateAdmission();

  /**
   * Sets Route of all Individuals to kContinuous Mode and clears destinations
   *
   * @param individuals To have Routes updated
   */
  void Discharge(vector<Individual*>* individuals);

  /**
   * Spread if any Individual sneezes
   */
  void UpdateSpread();
};

}  // namespace epidemic
